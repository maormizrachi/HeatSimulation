#include "RootIOAgent.h"

RootIOAgent::RootIOAgent(MPI_Comm comm, int root, int rows, int cols, bool graphicalOutput, bool textualOutput,
                         const std::string &graphicalPrefix, const std::string &textualPrefix):
             IOAgent(comm, root), rows(rows), cols(cols), matrix(rows, cols), graphicalOutput(graphicalOutput),
             textualOutput(textualOutput), graphicalPrefix(graphicalPrefix), textualPrefix(textualPrefix)
{
    MPI_Comm_size(this->comm, &this->size);
    this->_getBestDivision();
    this->mesh.resize(this->processesRows);
    for(int i = 0; i < this->processesRows; i++)
    {
        this->mesh[i].resize(this->processesCols);
    }
    this->_makeMeshDecomposition();
    this->_sendRowsCols();
    this->_sendNeighbors();

    this->outputFileWidth = rows;
    this->outputFileHeight = cols;
}

void RootIOAgent::_getBestDivision()
{
    this->processesRows = 1;
    this->processesCols = this->size;

    for(int i = 1; i <= sqrt(this->size); i++)
    {
        if(this->size % i == 0)
        {
            if(this->cols >= i)
            {
                this->processesCols = i;
                this->processesRows = this->size / i;
            }
            else if(this->rows >= i)
            {
                this->processesRows = i;
                this->processesCols = this->size / i;
            }
        }
    }
}

void RootIOAgent::_makeMeshDecomposition()
{
    int rowsPerProcessDefault, colsPerProcessDefault, rowsPerProcessLast, colsPerProcessLast;

    rowsPerProcessDefault = this->rows / this->processesRows;
    rowsPerProcessLast = rowsPerProcessDefault + (this->rows - rowsPerProcessDefault * this->processesRows);
    colsPerProcessDefault = this->cols / this->processesCols;
    colsPerProcessLast = colsPerProcessDefault + (this->cols - colsPerProcessDefault * this->processesCols);

    for(int i = 0, row = 0; row < this->processesRows; row++)
    {
        for(int col = 0; col < this->processesCols; col++)
        {
            this->mesh[row][col].rank = i++;
            this->mesh[row][col].rows = (row == this->processesRows - 1)? rowsPerProcessLast : rowsPerProcessDefault;
            this->mesh[row][col].cols = (col == this->processesCols - 1)? colsPerProcessLast : colsPerProcessDefault;
        }
    }
    
    for(int i = 0; i < this->processesRows; i++)
    {
        for(int j = 0; j < this->processesCols; j++)
        {
            this->mesh[i][j].leftRank0 = this->mesh[i][MOD(j - 1, this->processesCols)].rank; 
            this->mesh[i][j].leftRank1 = (this->mesh[i][MOD(j - 1, this->processesCols)].cols > 1)?
                                        this->mesh[i][MOD(j - 1, this->processesCols)].rank : this->mesh[i][MOD(j - 2, this->processesCols)].rank; 
            this->mesh[i][j].rightRank0 = this->mesh[i][MOD(j + 1, this->processesCols)].rank; 
            this->mesh[i][j].rightRank1 = (this->mesh[i][MOD(j + 1, this->processesCols)].cols > 1)? 
                                        this->mesh[i][MOD(j + 1, this->processesCols)].rank : this->mesh[i][MOD(j + 2, this->processesCols)].rank; 
            this->mesh[i][j].aboveRank0 = this->mesh[MOD(i - 1, this->processesRows)][j].rank; 
            this->mesh[i][j].aboveRank1 = (this->mesh[MOD(i - 1, this->processesRows)][j].rows > 1)? 
                                        this->mesh[MOD(i - 1, this->processesRows)][j].rank : this->mesh[MOD(i - 2, this->processesRows)][j].rank; 
            this->mesh[i][j].belowRank0 = this->mesh[MOD(i + 1, this->processesRows)][j].rank; 
            this->mesh[i][j].belowRank1 = (this->mesh[MOD(i + 1, this->processesRows)][j].rows > 1)? 
                                        this->mesh[MOD(i + 1, this->processesRows)][j].rank : this->mesh[MOD(i + 2, this->processesRows)][j].rank;
            this->mesh[i][j].aboveLeft = this->mesh[MOD(i - 1, this->processesRows)][MOD(j - 1, this->processesCols)].rank; 
            this->mesh[i][j].aboveRight = this->mesh[MOD(i - 1, this->processesRows)][MOD(j + 1, this->processesCols)].rank; 
            this->mesh[i][j].belowLeft = this->mesh[MOD(i + 1, this->processesRows)][MOD(j - 1, this->processesCols)].rank; 
            this->mesh[i][j].belowRight = this->mesh[MOD(i + 1, this->processesRows)][MOD(j + 1, this->processesCols)].rank; 
        }
    }
}

void RootIOAgent::_sendRowsCols()
{
    for(int i = 0; i < this->processesRows; i++)
    {
        for(int j = 0; j < this->processesCols; j++)
        {
            int rankToSend = this->mesh[i][j].rank;
            if(rankToSend == this->rank)
            {
                continue;
            }
            MPI_Send(&this->mesh[i][j].rows, 1, MPI_INT, rankToSend, ROWS_SEND_TAG, MPI_COMM_WORLD);
            MPI_Send(&this->mesh[i][j].cols, 1, MPI_INT, rankToSend, COLS_SEND_TAG, MPI_COMM_WORLD);
        }
    }
}

void RootIOAgent::_sendNeighbors()
{
    for(int i = 0; i < this->processesRows; i++)
    {
        for(int j = 0; j < this->processesCols; j++)
        {
            int rankToSend = this->mesh[i][j].rank;
            if(rankToSend == this->rank)
            {
                continue;
            }
            MPI_Send(&this->mesh[i][j].leftRank0, 1, MPI_INT, rankToSend, LEFT_RANK0_NEIGHBOR_TAG, MPI_COMM_WORLD);
            MPI_Send(&this->mesh[i][j].leftRank1, 1, MPI_INT, rankToSend, LEFT_RANK1_NEIGHBOR_TAG, MPI_COMM_WORLD);
            MPI_Send(&this->mesh[i][j].rightRank0, 1, MPI_INT, rankToSend, RIGHT_RANK0_NEIGHBOR_TAG, MPI_COMM_WORLD);
            MPI_Send(&this->mesh[i][j].rightRank1, 1, MPI_INT, rankToSend, RIGHT_RANK1_NEIGHBOR_TAG, MPI_COMM_WORLD);
            MPI_Send(&this->mesh[i][j].aboveRank0, 1, MPI_INT, rankToSend, ABOVE_RANK0_NEIGHBOR_TAG, MPI_COMM_WORLD);
            MPI_Send(&this->mesh[i][j].aboveRank1, 1, MPI_INT, rankToSend, ABOVE_RANK1_NEIGHBOR_TAG, MPI_COMM_WORLD);
            MPI_Send(&this->mesh[i][j].belowRank0, 1, MPI_INT, rankToSend, BELOW_RANK0_NEIGHBOR_TAG, MPI_COMM_WORLD);
            MPI_Send(&this->mesh[i][j].belowRank1, 1, MPI_INT, rankToSend, BELOW_RANK1_NEIGHBOR_TAG, MPI_COMM_WORLD);
            MPI_Send(&this->mesh[i][j].aboveLeft, 1, MPI_INT, rankToSend, ABOVE_LEFT_NEIGHBOR_TAG, MPI_COMM_WORLD);
            MPI_Send(&this->mesh[i][j].belowLeft, 1, MPI_INT, rankToSend, BELOW_LEFT_NEIGHBOR_TAG, MPI_COMM_WORLD);
            MPI_Send(&this->mesh[i][j].belowRight, 1, MPI_INT, rankToSend, BELOW_RIGHT_NEIGHBOR_TAG, MPI_COMM_WORLD);
            MPI_Send(&this->mesh[i][j].aboveRight, 1, MPI_INT, rankToSend, ABOVE_RIGHT_NEIGHBOR_TAG, MPI_COMM_WORLD);
        }
    }
}

void RootIOAgent::sendMatrixToAllByParts()
{
    int startRow = 0;
    for(int i = 0; i < this->processesRows; i++)
    {
        int startCol = 0;
        for(int j = 0; j < this->processesCols; j++)
        {
            if(this->mesh[i][j].rank != this->rank)
            {
                for(int k = 0; k < this->mesh[i][j].rows; k++)
                {
                    MPI_Send(&matrix[startRow + k][startCol], sizeof(data_t) * this->mesh[i][j].cols, MPI_BYTE, this->mesh[i][j].rank, MPI_DATA_RECEIVE_TAG, this->comm);
                }
            }
            startCol += this->mesh[i][j].cols;
        }
        startRow += this->mesh[i][0].rows;
    }
}

void RootIOAgent::output(int iteration, GhostMatrix &matrix)
{
    this->_receiveMatrices(matrix);
    if(this->textualOutput)
    {
        textualPrintToFile(this->textualPrefix + "/" + std::to_string(iteration), this->matrix, this->rows, this->cols);
    }
    if(this->graphicalOutput)
    {
        graphicalPrintToFile(this->graphicalPrefix + "/" + std::to_string(iteration) + GRAPHICAL_OUTPUT_EXTENSION,
                             this->outputFileWidth, this->outputFileHeight, this->matrix, this->rows, this->cols);
    }
}

void RootIOAgent::getNeighbors(Cell &cell)
{
    // find my cell, then copy the relevant data
    for(int i = 0; i < this->processesRows; i++)
    {
        for(int j = 0; j < this->processesCols; j++)
        {
            if(this->mesh[i][j].rank == this->rank)
            {
                cell.setAboveRanks(this->mesh[i][j].aboveRank0, this->mesh[i][j].aboveRank1);
                cell.setBelowRanks(this->mesh[i][j].belowRank0, this->mesh[i][j].belowRank1);
                cell.setRightRanks(this->mesh[i][j].rightRank0, this->mesh[i][j].rightRank1);
                cell.setLeftRanks(this->mesh[i][j].leftRank0, this->mesh[i][j].leftRank1);
                cell.setAboveRightRank(this->mesh[i][j].aboveRight);
                cell.setBelowRightRank(this->mesh[i][j].belowRight);
                cell.setBelowLeftRank(this->mesh[i][j].belowLeft);
                cell.setAboveLeftRank(this->mesh[i][j].aboveLeft);
                return;
            }
        }
    }
}

void RootIOAgent::getMatrix(Cell &cell)
{
    // find my cell, then copy the relevant data
    int startRow = 0;
    for(int i = 0; i < this->processesRows; i++)
    {
        int startCol = 0;
        for(int j = 0; j < this->processesCols; j++)
        {
            if(this->mesh[i][j].rank == this->rank)
            {
                for(int k = 0; k < this->mesh[i][j].rows; k++)
                {
                    std::memcpy(cell.getMatrix().getRow(k), &this->matrix[startRow + k][startCol], sizeof(data_t) * this->mesh[i][j].cols);
                }
                return;
            }
            startCol += this->mesh[i][j].cols;
        }
        startRow += this->mesh[i][0].rows;
    }
}

void RootIOAgent::getMyRowsCols(int &rows, int &cols)
{
    // find my cell, then copy the relevant data
    for(int i = 0; i < this->processesRows; i++)
    {
        for(int j = 0; j < this->processesCols; j++)
        {
            if(this->mesh[i][j].rank == this->rank)
            {
                rows = this->mesh[i][j].rows;
                cols = this->mesh[i][j].cols;
                return;
            }
        }
    }
}

/**
receives the big matrix, after a computation step
*/
void RootIOAgent::_receiveMatrices(GhostMatrix &myMatrix)
{
    // find my cell, then copy the relevant data
    int startRow = 0;
    for(int i = 0; i < this->processesRows; i++)
    {
        int startCol = 0;
        for(int j = 0; j < this->processesCols; j++)
        {
            // std::cout << "pasting processor " << this->mesh[i][j].rank << "'s data in " << "(" << startRow << ", " << startCol << ")" << std::endl;
            if(this->mesh[i][j].rank != this->rank)
            {
                for(int k = 0; k < this->mesh[i][j].rows; k++)
                {
                    MPI_Recv(&this->matrix[startRow + k][startCol], sizeof(data_t) * this->mesh[i][j].cols, MPI_BYTE, this->mesh[i][j].rank, ROOT_DATA_SEND_TAG, this->comm, MPI_STATUS_IGNORE);
                }
            }
            else
            {
                for(int k = 0; k < myMatrix.getRows(); k++)
                {
                    std::memcpy(&this->matrix[startRow + k][startCol], myMatrix.getRow(k), sizeof(data_t) * this->mesh[i][j].cols);
                }
            }
            startCol += this->mesh[i][j].cols;
        }
        startRow += this->mesh[i][0].rows;
    }
}