#include "viz.hpp"
#include "util.hpp"

#include <SFML/Graphics.hpp>
#include <iostream>

Viz::Viz() : fluid_cube(FluidCube(0.2f, 0, 1e-7f)) {
    this->window.create(
        sf::VideoMode(CUBE_SIZE * CELL_SCALE, CUBE_SIZE * CELL_SCALE),
        "Incompressible Fluid Simulation"
    );
}

Viz::~Viz() {}

void Viz::run_viz() {
    sf::Vector2i mouse_prev = sf::Mouse::getPosition(this->window);
    sf::Vector2i mouse_curr = mouse_prev;

    while (this->window.isOpen())  {
        sf::Event e;
        while (this->window.pollEvent(e)) {
            switch (e.type) {
                case sf::Event::Closed:
                    this->window.close();
                    break;
                default:
                    break;
            }
        }

        mouse_curr = sf::Mouse::getPosition(this->window);
        printf("Pos: (%d, %d)\n", mouse_curr.x, mouse_curr.y);

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            this->fluid_cube.addDensity(mouse_curr.x / CELL_SCALE, mouse_curr.y / CELL_SCALE, 500);
        }

        float velX = float(mouse_curr.x - mouse_prev.x);
        float velY = float(mouse_curr.y - mouse_prev.y);

        if (mouse_curr.x >= 0 && mouse_curr.x <= (CUBE_SIZE * CELL_SCALE) && 
            mouse_curr.y >= 0 && mouse_curr.y <= (CUBE_SIZE * CELL_SCALE)) {
            this->fluid_cube.addVelocity(mouse_curr.x / CELL_SCALE, mouse_curr.y / CELL_SCALE, velX/10, velY/10);
        }
        
        mouse_prev = mouse_curr;

        this->fluid_cube.step();
        this->fluid_cube.render(window); 

        this->window.display();
        this->fluid_cube.fade();
    }
}
