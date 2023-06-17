#ifndef VARIABLES_H_
#define VARIABLES_H_

#include <limits>
#include <vector>

#define NEIGHBOR_GHOST_RADIUS 2

typedef long double data_t;
#define CAST_FUNCTION(str) (std::stold(str)) // used to cast between a string to data_t
#define MPI_SEND_TYPE MPI_LONG_DOUBLE

#define INFINITY_DATA_T (std::numeric_limits<data_t>::infinity())
#define MINUS_INFINITY_DATA_T (std::numeric_limits<data_t>::lowest())

#endif // VARIABLES_H_