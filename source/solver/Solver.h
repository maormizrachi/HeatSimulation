#ifndef SOLVER_H_
#define SOLVER_H_

#include <cmath>
#include "../IO/IOAgent.h"
#include "../cell/Cell.h"

#define STENCIL_ROWS 5
#define STENCIL_COLS 5
#define STENCIL_CENTER_ROW (STENCIL_ROWS / 2)
#define STENCIL_CENTER_COL (STENCIL_COLS / 2)

class Solver
{
public:
    Solver(IOAgent *agent, Cell *cell, matrix_t &coefficients);
    ~Solver() = default;

    void solve(data_t accuracy, int maxIters);

    inline const MPI_Comm &getCommunicator(){return this->cell->getCommunicator();};

private:
    IOAgent *agent;
    Cell *cell;
    matrix_t coefficients;
    int rank, size;

    inline void sendDataToNeighbors(){this->cell->sendData();};
    inline void receiveDataFromNeighbors(){this->cell->receiveData();};
    inline GhostMatrix &getMyMatrix(){return this->cell->getMatrix();};
    inline void replaceMatrices(){this->getMyMatrix().replaceMatrices();};
    inline void printResults(int iteration){this->agent->output(iteration, this->cell->getMatrix());};
    data_t calculateStencilCoefficients(int row, int col);
    data_t makeStep();
};

#endif // SOLVER_H_