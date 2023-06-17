#ifndef ARGS_PARSER
#define ARGS_PARSER

#include <iostream>
#include <map>
#include "../../utils/abort.h"

#define HELP_ARGUMENT "help"
#define FILE_NAME_ARGUMENT "inputFile"
#define STENCIL_COEFFICIENTS_FILE "stencilCoeff"
#define ACCURACY_ARGUMENT "accuracy"
#define MAXITERS_ARGUMENT "maxIters"
#define GRAPHICAL_OUTPUT_FLAG_ARGUMENT "graphical"
#define TEXTUAL_OUTPUT_FLAG_ARGUMENT "textual"
#define GRAPHICAL_OUTPUT_DIR_ARGUMENT "graphicalOutputDir"
#define TEXTUAL_OUTPUT_DIR_ARGUMENT "textualOutputDir"
#define EXECUTABLE_NAME_ARGUMENT "executable"

#define TRUE_STR "true"
#define FALSE_STR "false"

#define DEFAULT_COEFFICIENTS_FILE "coefficients/coefficients"
#define DEFAULT_TEXTUAL_OUTPUT_ARGUMENT_VALUE FALSE_STR
#define DEFAULT_GRAPHICAL_OUTPUT_ARGUMENT_VALUE TRUE_STR
#define DEFAULT_TEXTUAL_OUTPUT_DIRECTORY_PREFIX "output/textual_output/"
#define DEFAULT_GRAPHICAL_OUTPUT_DIRECTORY_PREFIX "output/graphical_output/"

class ArgumentParser
{
public:
    static ArgumentParser &getInstance()
    {
        static ArgumentParser instance;
        return instance;
    }

    void printHelp();
    void setArguments(int argc, char *argv[]);
    void ensureNecessaryArguments();
    inline std::string &getArgument(std::string arg){return this->arguments[arg];};
    inline std::string &operator[](std::string arg){return this->getArgument(arg);};

private:
    ArgumentParser() = default;
    ~ArgumentParser() = default;

    void _fillArguments();

    std::map<std::string, std::string> arguments;
};

#endif // ARGS_PARSER