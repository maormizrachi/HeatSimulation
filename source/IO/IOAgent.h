#ifndef IOAGENT_H
#define IOAGENT_H

#include <vector>
#include <mpi.h>

#include "../matrix/Matrix.h"
#include "../cell/Cell.h"

#define ROOT_DATA_SEND_TAG 48
#define MPI_DATA_RECEIVE_TAG 49
#define ROWS_SEND_TAG 50
#define COLS_SEND_TAG 51
#define LEFT_RANK0_NEIGHBOR_TAG 52
#define LEFT_RANK1_NEIGHBOR_TAG 53
#define RIGHT_RANK0_NEIGHBOR_TAG 54
#define RIGHT_RANK1_NEIGHBOR_TAG 55
#define ABOVE_RANK0_NEIGHBOR_TAG 56
#define ABOVE_RANK1_NEIGHBOR_TAG 57
#define BELOW_RANK0_NEIGHBOR_TAG 58
#define BELOW_RANK1_NEIGHBOR_TAG 59
#define ABOVE_LEFT_NEIGHBOR_TAG 60
#define BELOW_LEFT_NEIGHBOR_TAG 61
#define BELOW_RIGHT_NEIGHBOR_TAG 62
#define ABOVE_RIGHT_NEIGHBOR_TAG 63

class IOAgent
{
public:
    IOAgent(MPI_Comm comm, int root);
    virtual ~IOAgent() = default;
    virtual void getNeighbors(Cell &cell);
    virtual void getMatrix(Cell &cell);
    virtual void getMyRowsCols(int &rows, int &cols);
    virtual void reportConverge(int){};

    inline virtual void output(int, GhostMatrix &matrix){this->sendMatrixToRoot(matrix);};

protected:
    MPI_Comm comm;
    int root;
    int rank;

private:
    void sendMatrixToRoot(GhostMatrix &matrix);
};

#endif // IOAGENT_H