#include "util.hpp"
#include "fluidphys.hpp"
#include <iostream>
#include <SFML/Graphics.hpp>

FluidCube::FluidCube(float dt, float diffusion, float viscosity)
    : 
    // initialize all Array2Ds to 0
    density{}, prev_density{},
    vx{}, vy{},
    prev_vx{}, prev_vy{}
{
    this->dt = dt;
    this->diff = diffusion;
    this->visc = viscosity;
    this->curr_hue = 0.0f;
    this->color_mode = HUE_ROTATE;
}

FluidCube::~FluidCube() {}

void FluidCube::addDensity(int x, int y, float amount) {
    // this is the density of the DYE, not the density of the fluid
    // the density of the FLUID is CONSTANT in an incompressible fluid
    this->density[x][y] += amount;
    if (this->density[x][y] > MAX_DENSITY) {
        this->density[x][y] = MAX_DENSITY;
    }
}

void FluidCube::addVelocity(int x, int y, float amountX, float amountY) {
    this->vx[x][y] += amountX;
    this->vy[x][y] += amountY;
}


void FluidCube::step() {
    
    // Velocity Solver
    std::swap(vx, prev_vx);
    diffuse(VX, vx, prev_vx, visc, dt, 4);
    std::swap(vy, prev_vy);
    diffuse(VY, vy, prev_vy, visc, dt, 4);

    project(vx, vy, prev_vx, prev_vy, 4);

    std::swap(vx, prev_vx);
    std::swap(vy, prev_vy);
    advect(VX, vx, prev_vx, prev_vx, prev_vy, dt);
    advect(VY, vy, prev_vy, prev_vx, prev_vy, dt);

    project(vx, vy, prev_vx, prev_vy, 4);

    // Density Solver
    std::swap(density, prev_density);
    diffuse(DENSITY, density, prev_density, diff, dt, 4);
    std::swap(density, prev_density);
    advect(DENSITY, density, prev_density, vx, vy, dt);
}

void FluidCube::render(sf::RenderWindow &window) {
    window.clear();
    for (int i=0; i < CUBE_SIZE; i++) {
        for (int j=0; j < CUBE_SIZE; j++) {
            sf::RectangleShape cell;
            cell.setSize(sf::Vector2f(CELL_SCALE, CELL_SCALE));
            cell.setPosition(i * CELL_SCALE, j * CELL_SCALE);

            switch(this->color_mode){
                case BW:  {
                    float alpha = 255 - (this->density[i][j] > 255 ? 255 : this->density[i][j]);
                    cell.setFillColor(sf::Color(255, 255, 255, (sf::Uint8)alpha));
                    break;
                }
                case HUE_ROTATE: {
                    float s = this->density[i][j] / MAX_DENSITY;        // Saturation depends on density
                    sf::Color color = hsvToRgb(this->curr_hue, s);
                    cell.setFillColor(color);
                    break;
                }
            }
            
            window.draw(cell);
        }
    }
}

void FluidCube::fade() {
    for (int i=0; i < CUBE_SIZE; i++) {
        for (int j=0; j < CUBE_SIZE; j++) {
            float d = this->density[i][j];
            this->density[i][j] = std::max(0.0f, d - FADE_RATE);
            this->curr_hue = fmod(this->curr_hue + 1e-4, 360.0f);
        }
    }
}

