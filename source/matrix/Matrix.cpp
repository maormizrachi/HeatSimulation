#include "Matrix.h"

Matrix::Matrix(int rows, int cols): _rows(rows), _cols(cols)
{
    this->_data.resize(this->_rows * this->_cols);
    for(int i = 0; i < this->_rows; i++)
    {
        for(int j = 0; j < this->_cols; j++)
        {
            this->_data[i * this->_cols + j] = 0;
        }
    }
}

void Matrix::_copyData(const Matrix &other)
{
    this->_rows = other._rows;
    this->_cols = other._cols;
    this->_data.resize(this->_rows * this->_cols);
    for(int i = 0; i < this->_rows; i++)
    {
        for(int j = 0; j < this->_cols; j++)
        {
            this->_data[i * this->_cols + j] = other._data[i * this->_cols + j];
        }
    }
}

Matrix &Matrix::operator=(const Matrix &other)
{
    this->_copyData(other);
    return *this;
}

Matrix::Matrix(const Matrix &other)
{
    _copyData(other);
}