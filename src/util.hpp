#include <array>

#if !defined UTILS
#define UTILS

const int CUBE_SIZE = 50; 
const int CELL_SCALE = 5;
const int MAX_DENSITY = 500;

#endif

using Array2D = std::array<std::array<float, CUBE_SIZE>, CUBE_SIZE>;    // put in utils file
