#include "IOAgent.h"

IOAgent::IOAgent(MPI_Comm comm, int root): comm(comm), root(root)
{
    MPI_Comm_rank(MPI_COMM_WORLD, &this->rank);
}

void IOAgent::sendMatrixToRoot(GhostMatrix &matrix)
{
    for(int i = 0; i < matrix.getRows(); i++)
    {
        MPI_Send(matrix.getRow(i), sizeof(data_t) * matrix.getCols(), MPI_BYTE, this->root, ROOT_DATA_SEND_TAG, this->comm);
    }
}

void IOAgent::getNeighbors(Cell &cell)
{
    int aboveRank1, aboveRank0, belowRank0, belowRank1, leftRank0, leftRank1, rightRank0, rightRank1;
    int aboveLeft, aboveRight, belowLeft, belowRight;
    
    MPI_Recv(&leftRank0, 1, MPI_INT, this->root, LEFT_RANK0_NEIGHBOR_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&leftRank1, 1, MPI_INT, this->root, LEFT_RANK1_NEIGHBOR_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&rightRank0, 1, MPI_INT, this->root, RIGHT_RANK0_NEIGHBOR_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&rightRank1, 1, MPI_INT, this->root, RIGHT_RANK1_NEIGHBOR_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&aboveRank0, 1, MPI_INT, this->root, ABOVE_RANK0_NEIGHBOR_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&aboveRank1, 1, MPI_INT, this->root, ABOVE_RANK1_NEIGHBOR_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&belowRank0, 1, MPI_INT, this->root, BELOW_RANK0_NEIGHBOR_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&belowRank1, 1, MPI_INT, this->root, BELOW_RANK1_NEIGHBOR_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&aboveLeft, 1, MPI_INT, this->root, ABOVE_LEFT_NEIGHBOR_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&belowLeft, 1, MPI_INT, this->root, BELOW_LEFT_NEIGHBOR_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&belowRight, 1, MPI_INT, this->root, BELOW_RIGHT_NEIGHBOR_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&aboveRight, 1, MPI_INT, this->root, ABOVE_RIGHT_NEIGHBOR_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    cell.setAboveRanks(aboveRank0, aboveRank1);
    cell.setBelowRanks(belowRank0, belowRank1);
    cell.setRightRanks(rightRank0, rightRank1);
    cell.setLeftRanks(leftRank0, leftRank1);
    cell.setAboveRightRank(aboveRight);
    cell.setBelowRightRank(belowRight);
    cell.setBelowLeftRank(belowLeft);
    cell.setAboveLeftRank(aboveLeft);
}

void IOAgent::getMyRowsCols(int &rows, int &cols)
{
    MPI_Recv(&rows, 1, MPI_INT, this->root, ROWS_SEND_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&cols, 1, MPI_INT, this->root, COLS_SEND_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

void IOAgent::getMatrix(Cell &cell)
{
    for(int i = 0; i < cell.getRows(); i++)
    {
        MPI_Recv(cell.getMatrix().getRow(i), sizeof(data_t) * cell.getCols(), MPI_BYTE, this->root, MPI_DATA_RECEIVE_TAG, this->comm, MPI_STATUS_IGNORE);
    }
}