#include <SFML/Graphics.hpp>
#include "util.hpp"

class FluidCube {

    private:
        int size;
        float dt;
        float diff;
        float visc;

        Array2D density;
        Array2D prev_density;

        Array2D vx;
        Array2D vy;

        Array2D prev_vx;
        Array2D prev_vy;

    public:
        FluidCube(float dt, float diffusion, float viscocity);
        ~FluidCube();

        void addDensity(int x, int y, float amount);
        void addVelocity(int x, int y, float amountX, float amountY);
        void step();
        void render(sf::RenderWindow &window);
        void fade();

};
