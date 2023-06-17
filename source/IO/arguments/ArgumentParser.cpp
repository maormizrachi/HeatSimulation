#include "ArgumentParser.h"

void ArgumentParser::printHelp()
{
    std::cout << "Heat Simulation\n" << std::endl;
    std::cout << "This simulation is a heat simulation, intended to solve the heat equations using an ISL (iterative stencil loop) program." << std::endl;
    std::cout << "The simulations halts when reaching to this accuracy or reaching a pre-determined number of iterations, both can be supplied by the user (see below)." << std::endl;
    std::cout << "Usage: " << this->arguments[EXECUTABLE_NAME_ARGUMENT] << " <ARGUMENTS>\n" << std::endl;
    std::cout << "The possible arguments are:" << std::endl;
    std::cout << "--" << HELP_ARGUMENT << "\t\tShows this message." << std::endl;
    std::cout << "\nThe following arguments are non-optional:\n" << std::endl;
    std::cout << "--" << FILE_NAME_ARGUMENT << "\t\tA path to a file that contains an initial heat map." << std::endl;
    std::cout << "--" << ACCURACY_ARGUMENT << "\t\tA number, representing the requested accuracy to reach convergence." << std::endl;
    std::cout << "--" << MAXITERS_ARGUMENT << "\t\tThe maximal allowed number of iterations." << std::endl;
    std::cout << "\nThe following arguments are optional:\n" << std::endl;
    std::cout << "--" << STENCIL_COEFFICIENTS_FILE << "\t\tA path to a file that contains the initial stencil coefficients." << std::endl;
    std::cout << "\t\t\tIf one does not supply a coefficients file, the coefficient file is assumed to be " << DEFAULT_COEFFICIENTS_FILE << "." << std::endl;
    std::cout << "--" << GRAPHICAL_OUTPUT_FLAG_ARGUMENT << "\t\tDetermines whether a graphical output should be produced. " << std::endl;
    std::cout << "\t\t\tIf one does not supply an additional directory for output (see below), the output will be stored in " << DEFAULT_GRAPHICAL_OUTPUT_DIRECTORY_PREFIX << "." << std::endl;
    std::cout << "--" << TEXTUAL_OUTPUT_FLAG_ARGUMENT << "\t\tDetermines whether a textual (heat map matrix) output should be produced. " << std::endl;
    std::cout << "\t\t\tIf one does not supply an additional directory for output (see below), the output will be stored in " << DEFAULT_TEXTUAL_OUTPUT_DIRECTORY_PREFIX << "." << std::endl;
    std::cout << "--" << GRAPHICAL_OUTPUT_DIR_ARGUMENT << "\tA path to store the graphical output in." << std::endl;
    std::cout << "--" << TEXTUAL_OUTPUT_DIR_ARGUMENT << "\tA path to store the textual output in." << std::endl;

}

void ArgumentParser::setArguments(int argc, char *argv[])
{
    this->arguments[EXECUTABLE_NAME_ARGUMENT] = std::string(argv[0]);
    int i = 1;
    while(i < argc)
    {
        std::string argumentName = std::string(argv[i]);
        if(argumentName[0] != '-' or argumentName[1] != '-')
        {
            ABORT(std::string("Illegal argument: ") + argv[i] + std::string("."));
        }

        std::string argumentValue;
        if((i == argc - 1) or std::string(argv[i + 1])[0] == '-')
        {
            argumentValue = TRUE_STR;
            i += 1;
        }
        else
        {
            argumentValue = std::string(argv[i + 1]);
            i += 2;        
        }
        this->arguments[std::string(argumentName.begin() + 2, argumentName.end())] = argumentValue;
    }
    this->_fillArguments();
}

/**
 * fills the ungiven arguments with their default value
*/
void ArgumentParser::_fillArguments()
{
    if(this->arguments.find(GRAPHICAL_OUTPUT_FLAG_ARGUMENT) == this->arguments.end())
    {
        if(this->arguments.find(GRAPHICAL_OUTPUT_DIR_ARGUMENT) != this->arguments.end())
        {
            this->arguments[GRAPHICAL_OUTPUT_FLAG_ARGUMENT] = TRUE_STR;
        }
        else
        {
            this->arguments[GRAPHICAL_OUTPUT_FLAG_ARGUMENT] = DEFAULT_TEXTUAL_OUTPUT_ARGUMENT_VALUE;
        }
    }
    if(this->arguments.find(TEXTUAL_OUTPUT_FLAG_ARGUMENT) == this->arguments.end())
    {
        if(this->arguments.find(TEXTUAL_OUTPUT_DIR_ARGUMENT) != this->arguments.end())
        {
            this->arguments[TEXTUAL_OUTPUT_FLAG_ARGUMENT] = TRUE_STR;
        }
        else
        {
            this->arguments[TEXTUAL_OUTPUT_FLAG_ARGUMENT] = DEFAULT_TEXTUAL_OUTPUT_ARGUMENT_VALUE;
        }
    }
    if(this->arguments.find(GRAPHICAL_OUTPUT_DIR_ARGUMENT) == this->arguments.end())
    {
        this->arguments[GRAPHICAL_OUTPUT_DIR_ARGUMENT] = DEFAULT_GRAPHICAL_OUTPUT_DIRECTORY_PREFIX;
    }
    if(this->arguments.find(TEXTUAL_OUTPUT_DIR_ARGUMENT) == this->arguments.end())
    {
        this->arguments[TEXTUAL_OUTPUT_DIR_ARGUMENT] = DEFAULT_TEXTUAL_OUTPUT_DIRECTORY_PREFIX;
    }
    if(this->arguments.find(STENCIL_COEFFICIENTS_FILE) == this->arguments.end())
    {
        this->arguments[STENCIL_COEFFICIENTS_FILE] = DEFAULT_COEFFICIENTS_FILE;
    }
}

void ArgumentParser::ensureNecessaryArguments()
{
    const std::string necessaryArguments[] = {FILE_NAME_ARGUMENT, ACCURACY_ARGUMENT, MAXITERS_ARGUMENT};
    for(const std::string &arg : necessaryArguments)
    {
        if(this->arguments.find(arg) == this->arguments.end())
        {
            ABORT("The argument --" + arg + " is necessary, and was not found.");
        }
    }
}
