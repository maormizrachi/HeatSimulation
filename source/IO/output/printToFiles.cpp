#include "printToFiles.h"

namespace
{
    /**
     * maps a number, between minVal and maxVal, to a color.
     * procedure was taken from here: https://www.particleincell.com/2014/colormap/
    */
    void calculate_rgb(const data_t &value, unsigned char rgb[3], data_t minVal, data_t maxVal)
    {
        if(minVal == maxVal)
        {
            return;
        }
        data_t strength = ((value - minVal) / (maxVal - minVal)) / 0.25;
        int X = std::floor(strength);	//this is the integer part
        int Y = std::floor(255*(strength-X)); //fractional part from 0 to 255
        switch(X)
        {
            case 0: rgb[0] = 255; rgb[1] = Y; rgb[0] = 0; break;
            case 1: rgb[0] = 255-Y; rgb[1] = 255; rgb[0] = 0; break;
            case 2: rgb[0] = 0; rgb[1] = 255; rgb[0] = Y; break;
            case 3: rgb[0] = 0; rgb[1] = 255-Y; rgb[0] = 255; break;
            case 4: rgb[0] = 0; rgb[1] = 0; rgb[0] = 255; break;
        }
    }
}

void textualPrintToFile(std::string outputFileName, const matrix_t &matrix, int rows, int cols)
{
    std::fstream file(outputFileName, std::ios::out);
    if(!file.is_open())
    {
        ABORT("The file '" + std::string(outputFileName) + "' could not be opened (are you sure the directory exists?).");
    }
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            if(j != rows - 1)
            {
                file << matrix[i][j] << " ";
            }
            else
            {
                file << matrix[i][j];
            }
        }
        file << std::endl;
    }
    file.close();
}

void graphicalPrintToFile(std::string outputFileName, int width, int height, const matrix_t &matrix, int rows, int cols)
{
    if(width * height < rows * cols)
    {
        ABORT("Error: The specified width and height (" + std::to_string(width) + " x " + std::to_string(height) + ") are too small for " + std::to_string(rows * cols) + " pixels.");
    }
    unsigned char prefix[] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                               static_cast<unsigned char>(width % 256) , static_cast<unsigned char>(width / 256), 
                               static_cast<unsigned char>(height % 256) , static_cast<unsigned char>(height / 256), 24 , 0
                               };
    int fd = open(outputFileName.c_str(), O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    if(fd < 0)
    {
        ABORT("The file '" + std::string(outputFileName) + "' could not be opened (are you sure the directory exists?).");
    }

    if(write(fd, prefix, sizeof(prefix)) == WRITE_ERROR)
    {
        close(fd);
        ABORT("Unsuccessful write");
    }

    data_t minVal = INFINITY_DATA_T, maxVal = MINUS_INFINITY_DATA_T;
    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            if(matrix[i][j] < minVal and matrix[i][j] != MINUS_INFINITY_DATA_T) minVal = matrix[i][j];
            if(matrix[i][j] > maxVal and matrix[i][j] != INFINITY_DATA_T) maxVal = matrix[i][j];
        }
    }

    for(int i = 0; i < rows; i++)
    {
        for(int j = 0; j < cols; j++)
        {
            unsigned char rgb[3] = {255, 255, 255}; // white
            if(matrix[i][j] != 0)
            {
                calculate_rgb(matrix[i][j], rgb, minVal, maxVal);
            }
            if(write(fd, rgb, sizeof(rgb)) == WRITE_ERROR)
            {
                close(fd);
                ABORT("Unsuccessful write");
            }
        }
    }
    close(fd);
}

