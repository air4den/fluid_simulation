#include <SFML/Graphics.hpp>
#include "fluidcube.hpp"

class Viz {
    private:
        FluidCube fluid_cube;
        sf::RenderWindow window;
    public:
        static const int num_particles;
        Viz();
        ~Viz();
        void run_viz();
};