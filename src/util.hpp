#include <array>

#if !defined UTILS
#define UTILS

const int CUBE_SIZE = 85; 
const int CELL_SCALE = 7;
const int MAX_DENSITY = 500;
const float FADE_RATE = 0.025f;

#endif

using Array2D = std::array<std::array<float, CUBE_SIZE>, CUBE_SIZE>;    // put in utils file