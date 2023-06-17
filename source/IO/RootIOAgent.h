#ifndef ROOT_IO_AGENT_H
#define ROOT_IO_AGENT_H

#include <cmath>
#include <iostream>
#include <cstring>

#include "IOAgent.h"
#include "../utils/abort.h"
#include "output/printToFiles.h"
#include "input/readFromFiles.h"

#define GRAPHICAL_OUTPUT_EXTENSION ".tga"

#define MOD(x, y) (((y) == 1)? 0 : (((x) >= 0)? ((x) % (y)) : (((y) - (((-1) * (x)) % (y))) % (y))))

class RootIOAgent : public IOAgent
{
public:
    RootIOAgent(MPI_Comm comm, int root, int rows, int cols, bool graphicalOutput, bool textualOutput, const std::string &graphicalPrefix, const std::string &textualPrefix);
    RootIOAgent(MPI_Comm comm, int root, int rows, int cols, const std::string &graphicalPrefix, const std::string &textualPrefix):
                RootIOAgent(comm, root, rows, cols, true, false, graphicalPrefix, textualPrefix){};
    void output(int iteration, GhostMatrix &matrix) override;
    void getNeighbors(Cell &cell) override;
    void reportConverge(int iterations) override{std::cout << "Converged after " << iterations << " iterations." << std::endl;}
    void getMatrix(Cell &cell) override;
    void getMyRowsCols(int &rows, int &cols) override;
    inline void readDataMatrixFromFile(std::string fileName){readMatrix(fileName, this->matrix, this->rows, this->cols);};
    void sendMatrixToAllByParts();

private:
    typedef struct _CellData
    {
        int rank;
        int rows;
        int cols;
        int leftRank0, leftRank1;
        int rightRank0, rightRank1;
        int aboveRank0, aboveRank1;
        int belowRank0, belowRank1;
        int aboveLeft, belowLeft;
        int aboveRight, belowRight;
    } _CellData;

    int size;
    int rows, cols;
    int processesRows, processesCols;
    int outputFileWidth, outputFileHeight;
    matrix_t matrix;
    std::vector<std::vector<_CellData>> mesh;
    bool graphicalOutput, textualOutput;
    std::string graphicalPrefix, textualPrefix;

    void _getBestDivision();
    void _makeMeshDecomposition();
    void _sendRowsCols();
    void _sendNeighbors();
    void _receiveMatrices(GhostMatrix &myMatrix);
    void _textualPrintToFile(std::string outputFileName);
    void _graphicalPrintToFile(std::string outputFileName, int width, int height);
};

#endif // ROOT_IO_AGENT_H