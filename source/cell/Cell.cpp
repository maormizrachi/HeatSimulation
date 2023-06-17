#include "Cell.h"

Cell::Cell(MPI_Comm comm, int root, int rows, int cols): comm(comm), root(root), matrix(GhostMatrix(rows, cols))
{
    MPI_Comm_rank(this->comm, &this->rank);
    for(int i = 0; i < NEIGHBORS_NUM; i++)
    {
        this->neighborsRequest[i] = MPI_REQUEST_NULL;
    }
    MPI_Type_vector(cols, sizeof(data_t), 1 * sizeof(data_t), MPI_BYTE, &this->rowType);
    MPI_Type_commit(&this->rowType);
    MPI_Type_vector(rows, sizeof(data_t), (cols + 2 * NEIGHBOR_GHOST_RADIUS) * sizeof(data_t), MPI_BYTE, &this->colType);
    MPI_Type_commit(&this->colType);
}

void Cell::sendData()
{
    int rows = this->matrix.getRows();
    int cols = this->matrix.getCols();

    MPI_Isend(this->matrix.getRow(0), 1, this->rowType, this->aboveRank1, ROW_0_DATA_TAG, this->comm, &this->sendRequests[0]);
    MPI_Isend(this->matrix.getRow(1), 1, this->rowType, this->aboveRank0, ROW_1_DATA_TAG, this->comm, &this->sendRequests[1]);
    MPI_Isend(this->matrix.getRow(rows - 1), 1, this->rowType, this->belowRank1, ROW_N_1_DATA_TAG, this->comm, &this->sendRequests[2]);
    MPI_Isend(this->matrix.getRow(rows - 2), 1, this->rowType, this->belowRank0, ROW_N_2_DATA_TAG, this->comm, &this->sendRequests[3]);
    MPI_Isend(this->matrix.getCol(0), 1, this->colType, this->leftRank1, LEFT_0_DATA_TAG, this->comm, &this->sendRequests[4]);
    MPI_Isend(this->matrix.getCol(1), 1, this->colType, this->leftRank0, LEFT_1_DATA_TAG, this->comm, &this->sendRequests[5]);
    MPI_Isend(this->matrix.getCol(cols - 1), 1, this->colType, this->rightRank1, RIGHT_N_1_DATA_TAG, this->comm, &this->sendRequests[6]);
    MPI_Isend(this->matrix.getCol(cols - 2), 1, this->colType, this->rightRank0, RIGHT_N_2_DATA_TAG, this->comm,  &this->sendRequests[7]);

    MPI_Isend(this->matrix.getCellAddress(0, 0), sizeof(data_t), MPI_BYTE, this->aboveLeftRank, ABOVE_LEFT_DATA_TAG, this->comm, &this->sendRequests[8]);
    MPI_Isend(this->matrix.getCellAddress(rows - 1, 0), sizeof(data_t), MPI_BYTE, this->belowLeftRank, BELOW_LEFT_DATA_TAG, this->comm, &this->sendRequests[9]);
    MPI_Isend(this->matrix.getCellAddress(0, cols - 1), sizeof(data_t), MPI_BYTE, this->aboveRightRank, ABOVE_RIGHT_DATA_TAG, this->comm, &this->sendRequests[10]);
    MPI_Isend(this->matrix.getCellAddress(rows - 1, cols - 1), sizeof(data_t), MPI_BYTE, this->belowRightRank, BELOW_RIGHT_DATA_TAG, this->comm, &this->sendRequests[11]);
}

void Cell::receiveData()
{
    MPI_Irecv(this->matrix.getGhostRow(-2), 1, this->rowType, this->belowRank1, ROW_0_DATA_TAG, this->comm, &this->receiveRequests[0]);
    MPI_Irecv(this->matrix.getGhostRow(-1), 1, this->rowType, this->belowRank0, ROW_1_DATA_TAG, this->comm, &this->receiveRequests[1]);
    MPI_Irecv(this->matrix.getGhostRow(1), 1, this->rowType, this->aboveRank1, ROW_N_1_DATA_TAG, this->comm, &this->receiveRequests[3]);    
    MPI_Irecv(this->matrix.getGhostRow(0), 1, this->rowType, this->aboveRank0, ROW_N_2_DATA_TAG, this->comm, &this->receiveRequests[2]);

    MPI_Irecv(this->matrix.getGhostCol(-2), 1, this->colType, this->rightRank1, LEFT_0_DATA_TAG, this->comm, &this->receiveRequests[4]);
    MPI_Irecv(this->matrix.getGhostCol(-1), 1, this->colType, this->rightRank0, LEFT_1_DATA_TAG, this->comm, &this->receiveRequests[5]);
    MPI_Irecv(this->matrix.getGhostCol(1), 1, this->colType, this->leftRank0, RIGHT_N_1_DATA_TAG, this->comm, &this->receiveRequests[7]);
    MPI_Irecv(this->matrix.getGhostCol(0), 1, this->colType, this->leftRank1, RIGHT_N_2_DATA_TAG, this->comm, &this->receiveRequests[6]);

    MPI_Irecv(this->matrix.getGhostCellAddress(0, 0), sizeof(data_t), MPI_BYTE, this->aboveLeftRank, BELOW_RIGHT_DATA_TAG, this->comm, &this->receiveRequests[8]);
    MPI_Irecv(this->matrix.getGhostCellAddress(-1, 0), sizeof(data_t), MPI_BYTE, this->belowLeftRank, ABOVE_RIGHT_DATA_TAG, this->comm, &this->receiveRequests[9]);
    MPI_Irecv(this->matrix.getGhostCellAddress(0, -1), sizeof(data_t), MPI_BYTE, this->aboveRightRank, BELOW_LEFT_DATA_TAG, this->comm, &this->receiveRequests[10]);
    MPI_Irecv(this->matrix.getGhostCellAddress(-1, -1), sizeof(data_t), MPI_BYTE, this->belowRightRank, ABOVE_LEFT_DATA_TAG, this->comm, &this->receiveRequests[11]);
}
