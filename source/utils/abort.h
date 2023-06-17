#ifndef ABORT_H
#define ABORT_H

#include <mpi.h>

#define ABORT(error_msg); \
    do { \
       std::cerr << (error_msg) << std::endl; \
       std::cerr << "ERROR in "; \
       int mpi_initialized; \
       MPI_Initialized(&mpi_initialized); \
       if(mpi_initialized){ \
        int rank; MPI_Comm_rank(MPI_COMM_WORLD, &rank); \
        std::cerr << "rank " << rank << ","; \
       } \
       std::cerr << " function " << __FUNCTION__ << ", in file " << __FILE__ << ", line " << __LINE__ << std::endl; \
       if(mpi_initialized) \
       { \
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE); \
       } \
       else \
       { \
        exit(EXIT_FAILURE); \
       } \
    } \
    while(0);

#endif // ABORT_H