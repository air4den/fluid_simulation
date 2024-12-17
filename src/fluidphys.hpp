#include "fluidcube.hpp"

enum ArrayData {
    DENSITY = 0, 
    VX = 1, 
    VY = 2,
};

void set_bounds_vx(Array2D vx);
void set_bounds_vy(Array2D vy);
void set_bounds_density(Array2D d);
void set_corners(Array2D x);

void set_bounds(ArrayData b, Array2D x);

//void lin_solve(ArrayData b, Array2D x, Array2D prev_x, float a, float c, int iter);

void diffuse(ArrayData b, Array2D x, Array2D prev_x, float diff, float dt, int iter);

void project(Array2D vx, Array2D vy, Array2D p, Array2D div, int iter);

void advect(ArrayData b, Array2D density, Array2D prev_density, Array2D vx, Array2D vy, float dt);