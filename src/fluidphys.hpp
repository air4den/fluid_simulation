#include "fluidcube.hpp"

enum ArrayData {
    DENSITY = 0, 
    VX = 1, 
    VY = 2
};

void set_bounds(ArrayData b, Array2D& x);

//void lin_solve(ArrayData b, Array2D x, Array2D prev_x, float a, float c, int iter);

void diffuse(ArrayData b, Array2D& arr, Array2D& prev_arr, float diff, float dt, int iter);

void project(Array2D& vx, Array2D& vy, Array2D& p, Array2D& div, int iter);

void advect(ArrayData b, Array2D& density, Array2D& prev_density, Array2D& vx, Array2D& vy, float dt);

sf::Color hsvToRgb(float h, float s);