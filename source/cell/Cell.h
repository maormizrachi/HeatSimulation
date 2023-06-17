#ifndef CELL_H
#define CELL_H

#include "../matrix/GhostMatrix.h"
#include <array>
#include <mpi.h>

#define NEIGHBORS_NUM 12

#define ROW_0_DATA_TAG 30
#define ROW_1_DATA_TAG 31
#define ROW_N_1_DATA_TAG 32
#define ROW_N_2_DATA_TAG 33
#define LEFT_0_DATA_TAG 34
#define LEFT_1_DATA_TAG 35
#define RIGHT_N_1_DATA_TAG 36
#define RIGHT_N_2_DATA_TAG 37
#define ABOVE_LEFT_DATA_TAG 38
#define BELOW_LEFT_DATA_TAG 39
#define ABOVE_RIGHT_DATA_TAG 40
#define BELOW_RIGHT_DATA_TAG 41

class Cell
{
public:
    Cell(MPI_Comm comm, int root, int rows, int cols);
    ~Cell() = default;
    inline void setAboveRanks(int rank0, int rank1){this->aboveRank0 = rank0; this->aboveRank1 = rank1;};
    inline void setBelowRanks(int rank0, int rank1){this->belowRank0 = rank0; this->belowRank1 = rank1;};
    inline void setRightRanks(int rank0, int rank1){this->rightRank0 = rank0; this->rightRank1 = rank1;};
    inline void setLeftRanks(int rank0, int rank1){this->leftRank0 = rank0; this->leftRank1 = rank1;};
    inline void setAboveRightRank(int rank){this->aboveRightRank = rank;};
    inline void setBelowRightRank(int rank){this->belowRightRank = rank;};
    inline void setBelowLeftRank(int rank){this->belowLeftRank = rank;};
    inline void setAboveLeftRank(int rank){this->aboveLeftRank = rank;};

    inline int getRows(){return this->matrix.getRows();};
    inline int getCols(){return this->matrix.getCols();};

    inline void waitForDataExchange()
    {
        MPI_Waitall(this->receiveRequests.size(), &(*(this->receiveRequests.begin())), MPI_STATUSES_IGNORE);
        MPI_Waitall(this->sendRequests.size(), &(*(this->sendRequests.begin())), MPI_STATUSES_IGNORE);
    }

    void sendData();
    void receiveData();

    inline const MPI_Comm &getCommunicator(){return this->comm;};
    inline GhostMatrix &getMatrix(){return this->matrix;};
    // inline int getRank(){return this->rank;};

private:
    MPI_Comm comm;
    int root;
    int rank;

    GhostMatrix matrix;
    int aboveRank0, aboveRank1, belowRank0, belowRank1;
    int leftRank0, leftRank1, rightRank0, rightRank1;
    int aboveRightRank, belowRightRank, belowLeftRank, aboveLeftRank;
    std::array<MPI_Request, NEIGHBORS_NUM> neighborsRequest, receiveRequests, sendRequests;
    MPI_Datatype colType, rowType;

};
#endif // CELL_H