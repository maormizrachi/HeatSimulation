#include <iostream>
#include "utils/abort.h"
#include "solver/Solver.h"
#include "IO/IOAgent.h"
#include "IO/RootIOAgent.h"
#include "IO/arguments/ArgumentParser.h"

#define ROOT 0

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    IOAgent *agent;
    matrix_t stencil(STENCIL_ROWS, STENCIL_COLS);

    int rows = 0, cols = 0;

    ArgumentParser &parser = ArgumentParser::getInstance();
    parser.setArguments(argc, argv);

    if(argc == 1 or parser[HELP_ARGUMENT] == TRUE_STR)
    {
        if(rank == ROOT)
        {
            parser.printHelp();
        }
        MPI_Finalize();
        return EXIT_SUCCESS;
    }

    if(rank == ROOT)
    {
        parser.ensureNecessaryArguments();
        
        std::string fileName(parser[FILE_NAME_ARGUMENT]), stencilFileName(parser[STENCIL_COEFFICIENTS_FILE]);
        readRowsCols(fileName, rows, cols); // determines the rows and columns of the matrix

        int size;
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        if(size > rows * cols)
        {
            ABORT("Too much processes were allocated: the grid is in size of " + std::to_string(rows) + " rows and " + std::to_string(cols) + " columns, while " + std::to_string(size) + " processes allocated.");
        }
        readMatrix(stencilFileName, stencil, STENCIL_ROWS, STENCIL_COLS); // reads the stencil

        bool graphicalOutput = (parser[GRAPHICAL_OUTPUT_FLAG_ARGUMENT] == TRUE_STR);
        bool textualOutput = (parser[TEXTUAL_OUTPUT_FLAG_ARGUMENT] == TRUE_STR);

        agent = new RootIOAgent(MPI_COMM_WORLD, ROOT, rows, cols, graphicalOutput, textualOutput,
                                parser[GRAPHICAL_OUTPUT_DIR_ARGUMENT], parser[TEXTUAL_OUTPUT_DIR_ARGUMENT]);
        dynamic_cast<RootIOAgent*>(agent)->readDataMatrixFromFile(fileName); // reads the matrix
        dynamic_cast<RootIOAgent*>(agent)->sendMatrixToAllByParts(); // distributes the matrix
    }
    else
    {
        agent = new IOAgent(MPI_COMM_WORLD, ROOT);
    }
    
    for(int i = 0; i < STENCIL_ROWS; i++)
    {
        MPI_Bcast(stencil[i], sizeof(data_t) * STENCIL_COLS, MPI_BYTE, ROOT, MPI_COMM_WORLD);
    }

    agent->getMyRowsCols(rows, cols);
    Cell myCell(MPI_COMM_WORLD, ROOT, rows, cols);

    agent->getNeighbors(myCell);
    agent->getMatrix(myCell);

    data_t accuracy = CAST_FUNCTION(parser[ACCURACY_ARGUMENT]);
    int maxIters = std::stoi(parser[MAXITERS_ARGUMENT]);

    Solver solver(agent, &myCell, stencil);
    solver.solve(accuracy, maxIters);

    delete agent;

    MPI_Finalize();
    return EXIT_SUCCESS;
}