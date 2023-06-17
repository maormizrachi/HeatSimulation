# compiler & linker arguments
CXX=mpicxx
CXX_STANDARD=c++17
CXXFLAGS=-Wall -Wextra -Werror -O3 -std=$(CXX_STANDARD)
CXX_EXECUTABLES=$(shell find . -name '*.cpp')
CXX_OBJS=$(CXX_EXECUTABLES:.cpp=.o)
EXECUTABLE=heat

# mpi arguments
MPI_NP?=$(shell grep -c ^processor /proc/cpuinfo)
MPI_EXECUTE=mpiexec
MPI_DEFAULT_HOSTS="localhost:$(MPI_NP)"
MPI_HOSTS?=$(MPI_DEFAULT_HOSTS)
MPI_EXECUTE_ARGS=-np $(MPI_NP) --host $(MPI_HOSTS)

# default arguments
EXAMPLE_FILE=examples/2
INPUT_FILE?=$(EXAMPLE_FILE)
EXAMPLE_COEFF=coefficients/coefficients
COEFF_FILE?=$(EXAMPLE_COEFF)
EXAMPLE_MAX_ITERS=200
MAX_ITERS?=$(EXAMPLE_MAX_ITERS)
EXAMPLE_ACCURACY=100
ACCURACY?=$(EXAMPLE_ACCURACY)
EXAMPLE_OUTPUT_DIR=output/graphical_output
OUTPUT_DIR?=$(EXAMPLE_OUTPUT_DIR)

# output arguments
OUTPUT_GIF_DELAY?=2
OUTPUT_GIF_FILENAME?=animation.gif

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $< -c -o $@ 

$(EXECUTABLE): $(CXX_OBJS)
	$(CXX) $(CXXFLAGS) $(CXX_OBJS) -o $(EXECUTABLE)

.PHONY: help
help: $(EXECUTABLE)
	./$(EXECUTABLE) --help

.PHONY: run
run: $(EXECUTABLE)
	mkdir -p $(OUTPUT_DIR)
	$(MPI_EXECUTE) $(MPI_EXECUTE_ARGS) ./$(EXECUTABLE) --inputFile $(INPUT_FILE) --stencilCoeff $(COEFF_FILE) --maxIters $(MAX_ITERS) --accuracy $(ACCURACY) --graphicalOutputDir $(OUTPUT_DIR)

.PHONY: example_run
example_run: $(EXECUTABLE)
	mkdir -p $(EXAMPLE_OUTPUT_DIR)
	$(MPI_EXECUTE) $(MPI_EXECUTE_ARGS) ./$(EXECUTABLE) --inputFile $(EXAMPLE_FILE) --maxIters $(EXAMPLE_MAX_ITERS) --accuracy $(EXAMPLE_ACCURACY) --graphical

.PHONY: gif
gif:
	convert -loop 0 -delay $(OUTPUT_GIF_DELAY) `find $(EXAMPLE_OUTPUT_DIR) -type f | grep tga | sort -V` $(OUTPUT_GIF_FILENAME)

.PHONY: example
example: example_run gif

.PHONY: clean
clean:
	rm -f $(EXECUTABLE)
	rm -f $(CXX_OBJS)
