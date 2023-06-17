#ifndef PRINT_TO_FILES_H_
#define PRINT_TO_FILES_H_

#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <cmath>
#include "../../matrix/Matrix.h"
#include "../../utils/abort.h"

#define WRITE_ERROR -1
#define COLORED 0.00001 // todo remove
#define DEFAULT_WIDTH 500
#define DEFAULT_HEIGHT 500

void textualPrintToFile(std::string outputFileName, const matrix_t &matrix, int rows, int cols);

void graphicalPrintToFile(std::string outputFileName, int width, int height, const matrix_t &matrix, int rows, int cols);

#endif // PRINT_TO_FILES_H_