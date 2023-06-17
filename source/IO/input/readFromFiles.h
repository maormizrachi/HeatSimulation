#ifndef READ_FROM_FILES_H
#define READ_FROM_FILES_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "../../utils/abort.h"
#include "../../matrix/Matrix.h"

#define INF "INF"
#define MINUS_INF "-INF"

void readRowsCols(std::string fileName, int &rows, int &cols);

void readMatrix(std::string fileName, matrix_t &matrix, int rows, int cols);

#endif // READ_FROM_FILES_H