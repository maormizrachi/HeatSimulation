#include "readFromFiles.h"

void readRowsCols(std::string fileName, int &rows, int &cols)
{
    std::fstream file(fileName, std::ios::in);
    if(!file.is_open())
    {
        ABORT("The file '" + fileName + "' could not be opened.");
    }
    std::string line;
    rows = 0;
    cols = 0;

    while(std::getline(file, line))
    {
        rows++;
        int colsInLine = std::count(line.begin(), line.end(), ' ');
        if(cols == 0)
        {
            cols = colsInLine + 1;
        }
        if(colsInLine + 1 != cols)
        {
            file.close();
            ABORT("Illegal number of numbers given in line " + std::to_string(rows) + ": Given " + std::to_string(colsInLine) + " in that line, while in previous lines given " + std::to_string(cols));
        }
    }
    if(rows == 0 or cols == 0)
    {
        file.close();
        ABORT("The given file '" + fileName + "' is empty");
    }
    file.close();
}

void readMatrix(std::string fileName, matrix_t &matrix, int rows, int cols)
{
    std::fstream file(fileName, std::ios::in);
    if(!file.is_open())
    {
        ABORT("The file '" + fileName + "' could not be opened.");
    }
    std::string line;
    for(int i = 0; i < rows; i++)
    {
        if(!std::getline(file, line))
        {
            file.close();
            ABORT("Unmatched number of lines given in the file '" + fileName + "' (expected " + std::to_string(rows) + ", given " + std::to_string(i) + ")");
        }
        std::stringstream stream(line);
        for(int j = 0; j < cols; j++)
        {
            if(stream.eof())
            {
                file.close();
                ABORT("Unmatched number of numbers given in the file '" + fileName + "' (expected " + std::to_string(cols) + ", given " + std::to_string(j) + ")");
            }
            std::string input;
            stream >> input;
            if(input == INF)
            {
                matrix[i][j] = INFINITY_DATA_T;
            }
            else if(input == MINUS_INF)
            {
                matrix[i][j] = MINUS_INFINITY_DATA_T;
            }
            else
            {
                try
                {
                    matrix[i][j] = CAST_FUNCTION(input);
                }
                catch(std::exception const &e)
                {
                    file.close();
                    ABORT("File '" + fileName + "': Illegal input in line " + std::to_string(i) + " and column " + std::to_string(j) + ": '" + input + "'");
                }
            }
        }  
    }
    file.close();
}

