#include "Solver.h"

#include <iostream> // todo remove

Solver::Solver(IOAgent *agent, Cell *cell, matrix_t &coefficients): agent(agent), cell(cell), coefficients(coefficients)
{
    MPI_Comm_rank(cell->getCommunicator(), &this->rank);
    MPI_Comm_size(cell->getCommunicator(), &this->size);
}

/**
 * given a row and a column, calculates and updates the new value, according to the stencil's coefficients.
 * @param row the row of the cell
 * @param col the column of the cell
 * @return the difference between the old and then new values
*/
data_t Solver::calculateStencilCoefficients(int row, int col)
{
    GhostMatrix &mat = this->getMyMatrix();
    data_t oldData = mat(row, col);
    data_t &newData = mat.getTempCell(row, col);
    newData = 0;
    for(int i = 0; i < STENCIL_ROWS; i++)
    {
        for(int j = 0; j < STENCIL_COLS; j++)
        {
            if(this->coefficients[i][j] == INFINITY_DATA_T)
            {
                continue;
            }
            newData += this->coefficients[i][j] * mat(row + i - STENCIL_ROWS / 2, col + j - STENCIL_COLS / 2);
        }
    }
    return oldData - newData;
}

/**
 * performs a single computation step of the simulation (assumes the ghost rows/cols/cells are updated)
*/
data_t Solver::makeStep()
{
    GhostMatrix &mat = this->getMyMatrix();
    data_t accuracy = 0;

    for(int i = 0; i < mat.getRows(); i++)
    {
        for(int j = 0; j < mat.getCols(); j++)
        {
            accuracy += std::abs(this->calculateStencilCoefficients(i, j));
        }
    }
    this->replaceMatrices();
    return accuracy;
}

/**
 * Solves the simulation, given the desired accuracy and max iterations number.
 * @param accuracy the accuracy parameter
 * @param maxIters the maximal number of iterations
*/
void Solver::solve(data_t accuracy, int maxIters)
{
    this->printResults(0);

    for(int i = 1; i <= maxIters; i++)
    {
        this->sendDataToNeighbors();
        this->receiveDataFromNeighbors();
        this->cell->waitForDataExchange();
        data_t error = std::abs(this->makeStep());
        this->printResults(i);

        data_t globalError;
        MPI_Allreduce(&error, &globalError, 1, MPI_SEND_TYPE, MPI_MAX, this->getCommunicator());

        if(std::abs(globalError) <= std::abs(accuracy))
        {
            this->agent->reportConverge(i);
            break;
        }
        MPI_Barrier(this->getCommunicator());
    }

}
