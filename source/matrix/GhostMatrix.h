#ifndef GHOST_MATRIX_H
#define GHOST_MATRIX_H

#include "Matrix.h"
#include "../utils/variables.h"

class GhostMatrix
{
public:
    GhostMatrix(int rows, int cols): rows(rows), cols(cols),
            _realRows(this->rows + 2 * NEIGHBOR_GHOST_RADIUS), _realCols(this->cols + 2 * NEIGHBOR_GHOST_RADIUS),
             mat(this->_realRows, this->_realCols), tmpMat(this->_realRows, this->_realCols){};
    ~GhostMatrix() = default;

    inline data_t &getCell(int row, int col)
    {
        row += NEIGHBOR_GHOST_RADIUS;
        col += NEIGHBOR_GHOST_RADIUS;
        return this->mat[row][col];
    }
    
    inline data_t &operator()(int row, int col){return this->getCell(row, col);};

    inline data_t &getTempCell(int row, int col)
    {
        row += NEIGHBOR_GHOST_RADIUS;
        col += NEIGHBOR_GHOST_RADIUS;
        return this->tmpMat[row][col];
    }

    inline data_t *getCellAddress(int row, int col)
    {
        row += NEIGHBOR_GHOST_RADIUS;
        col += NEIGHBOR_GHOST_RADIUS;
        return &this->mat[row][col];
    }

    inline data_t *getTempCelllAddress(int row, int col)
    {
        row += NEIGHBOR_GHOST_RADIUS;
        col += NEIGHBOR_GHOST_RADIUS;
        return &this->tmpMat[row][col];
    }

    inline data_t *getRow(int row)
    {
        row += NEIGHBOR_GHOST_RADIUS;
        return &this->mat[row][NEIGHBOR_GHOST_RADIUS];
    }

    inline data_t *getCol(int col)
    {
        col += NEIGHBOR_GHOST_RADIUS;
        return &this->mat[NEIGHBOR_GHOST_RADIUS][col];
    }
    
    inline int getRows(){return this->rows;};
    inline int getCols(){return this->cols;};

    inline data_t *getGhostRow(int row)
    {
        if(row == 0 or row == 1) return &this->mat[row][NEIGHBOR_GHOST_RADIUS];
        else return &this->mat[NEIGHBOR_GHOST_RADIUS + this->rows + ((row == -1)? 1 : 0)][NEIGHBOR_GHOST_RADIUS];
    }

    inline data_t *getGhostCol(int col)
    {
        if(col == 0 or col == 1) return &this->mat[NEIGHBOR_GHOST_RADIUS][col];
        else return &this->mat[NEIGHBOR_GHOST_RADIUS][NEIGHBOR_GHOST_RADIUS + this->cols  + ((col == -1)? 1 : 0)];
    }

    inline data_t *getGhostCellAddress(int row, int col)
    {
        int realRow = (row == 0)? 1 : NEIGHBOR_GHOST_RADIUS + this->rows;
        int realCol = (col == 0)? 1 : NEIGHBOR_GHOST_RADIUS + this->cols;
        return &this->mat[realRow][realCol];
    }

    inline void replaceMatrices(){auto &tmp = this->mat; this->mat = this->tmpMat; this->tmpMat = tmp;};

private:
    int rows, cols;
    int _realRows, _realCols;

    matrix_t mat;
    matrix_t tmpMat;
};

#endif // GHOST_MATRIX_H