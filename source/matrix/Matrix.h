#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include "../utils/variables.h"

class Matrix
{
public:
    Matrix(int rows, int cols);
    Matrix(const Matrix &other);
    ~Matrix() = default;
    Matrix &operator=(const Matrix &other);
    inline data_t& at(int row, int col){return this->_data[row * this->_cols + col];};
    inline data_t& operator()(int row, int col){return this->at(row, col);};
    inline data_t *operator[](int row){return &this->_data[row * this->_cols];};
    inline const data_t& at(int row, int col) const{return this->_data[row * this->_cols + col];};
    inline const data_t& operator()(int row, int col) const{return this->at(row, col);};
    inline const data_t *operator[](int row) const{return &this->_data[row * this->_cols];};
    inline int getRows(){return this->_rows;};
    inline int getCols(){return this->_cols;};

private:
    int _rows, _cols;
    std::vector<data_t> _data;    
    void _copyData(const Matrix &other);
};

typedef Matrix matrix_t;

#endif // MATRIX_H