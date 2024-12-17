#include "util.hpp"
#include "fluidphys.hpp"

void set_bounds_vx(Array2D vx) {
    
    for (int i = 1; i < CUBE_SIZE; i++) {
        // Top & Bottom Rows: VX is same
        vx[i][0]= vx[i][1];
        vx[i][CUBE_SIZE-1] = vx[i][CUBE_SIZE-2];

        // Left & Right Cols: VX is opposite 
        vx[0][i] = -vx[1][i];
        vx[CUBE_SIZE-1][i] = -vx[CUBE_SIZE-2][i];
    }

    // Corners
    set_corners(vx);
}

void set_bounds_vy(Array2D vy) {
    
    for (int i = 1; i < CUBE_SIZE - 1; i++) {
        // Top & Bottoms Rows: VY is opposite
        vy[i][0] = -vy[i][1];
        vy[i][CUBE_SIZE-1] = -vy[i][CUBE_SIZE-2];

        // Left & Right Cols: VY is same
        vy[0][i] = vy[1][i];
        vy[CUBE_SIZE -1][i] = vy[CUBE_SIZE-2][i];   
    }

    // Corners
    set_corners(vy);
}

void set_bounds_density(Array2D d) {
    
    for (int i = 1; i < CUBE_SIZE - 1; i++) {
        // Top & Bottoms Rows: density is same
        d[i][0] = d[i][1];
        d[i][CUBE_SIZE-1] = d[i][CUBE_SIZE-2];

        // Left & Right Cols: density is same
        d[0][i] = d[1][i];
        d[CUBE_SIZE -1][i] = d[CUBE_SIZE-2][i];   
    }

    // Corners
    set_corners(d);
}

void set_corners(Array2D x) {
    // Corners are set to avg of 2 direct neighbors
    x[0][0] = 0.5f * (x[1][0] + x[0][1]);
    x[CUBE_SIZE-1][0] = 0.5f * (x[CUBE_SIZE - 1][1] + x[CUBE_SIZE - 2][0]);
    x[0][CUBE_SIZE-1] = 0.5f * (x[0][CUBE_SIZE - 2] + x[1][CUBE_SIZE - 1]);
    x[CUBE_SIZE-1][CUBE_SIZE-1] =  0.5f * (x[CUBE_SIZE - 1][CUBE_SIZE - 2] + x[CUBE_SIZE - 2][CUBE_SIZE - 1]);
}
void set_bounds(ArrayData b, Array2D x) {
    for (int i = 1; i < CUBE_SIZE - 1; i++) {
        x[0][i] = b == VX ? -x[1][i] : x[1][i];              // Left boundary
        x[CUBE_SIZE-1][i] = b == VX ? -x[CUBE_SIZE-2][i] : x[CUBE_SIZE-2][i]; // Right boundary
        x[i][0] = b == VY ? -x[i][1] : x[i][1];              // Bottom boundary
        x[i][CUBE_SIZE-1] = b == VY ? -x[i][CUBE_SIZE-2] : x[i][CUBE_SIZE-2]; // Top boundary
    }

    // Fix corners as average of neighbors
    x[0][0] = 0.5f * (x[1][0] + x[0][1]);
    x[0][CUBE_SIZE-1] = 0.5f * (x[1][CUBE_SIZE-1] + x[0][CUBE_SIZE-2]);
    x[CUBE_SIZE-1][0] = 0.5f * (x[CUBE_SIZE-2][0] + x[CUBE_SIZE-1][1]);
    x[CUBE_SIZE-1][CUBE_SIZE-1] = 0.5f * (x[CUBE_SIZE-2][CUBE_SIZE-1] + x[CUBE_SIZE-1][CUBE_SIZE-2]);
}
/*
void set_bounds(ArrayData b, Array2D x) {
    switch (b) {
        case DENSITY: 
            set_bounds_density(x);
            break;
        case VX:
            set_bounds_vx(x);
            break;
        case VY:
            set_bounds_vy(x);
            break;
    }
}




void lin_solve(ArrayData b, Array2D x, Array2D prev_x, float a, float c, int iter) {
    float cRecip = 1.0 / c;
    for (int k = 0; k < iter; k++) {
        for (int j = 1; j < CUBE_SIZE-1; j++) {
            for (int i = 1; i < CUBE_SIZE-1; i++) {
                x[i][j] = 
                    (prev_x[i][j]
                        + a*( x[i+1][j]
                            + x[i-1][j]  
                            + x[i][j+1]
                            + x[i][j-1]  
                        )
                    ) * cRecip;
            }
        }
        set_bounds(b, x);
    }
}
*/

void diffuse(ArrayData b, Array2D arr, Array2D prev_arr, float diff, float dt, int iter) {
    float a = dt * diff * (CUBE_SIZE - 2) * (CUBE_SIZE - 2);
    for (int k=0; k<20  ; k++) {
        for (int i=1; i<CUBE_SIZE-1; i++) {
            for (int j=1; j<CUBE_SIZE-1; j++) {
                arr[i][j] = (prev_arr[i][j] + a * (arr[i-1][j] + arr[i+1][j] +
                            arr[i][j-1] + arr[i][j+1])) / (1+4*a);
            } 
        }
        set_bounds(b,arr);
    }
}


void project(Array2D vx, Array2D vy, Array2D p, Array2D div, int iter) {
    float h = 1.0/CUBE_SIZE;
    for (int i = 1; i < CUBE_SIZE-1; i++) {
        for (int j = 1; j < CUBE_SIZE-1; j++) {
            div[i][j] = -0.5 * h * (vx[i+1][j] - vx[i-1][j] +
                            vy[i][j+1] - vy[i][j-1]);
            p[i][j]=0;
        }
    }
    set_bounds(DENSITY, div);
    set_bounds(DENSITY, p);

    for (int k = 0; k<20; k++) {
        for (int i = 1; i < CUBE_SIZE-1; i++) {
            for (int j = 1; j < CUBE_SIZE-1; j++) {
                p[i][j] = (div[i][j] + p[i-1][j] + p[i+1][j] + p[i][j-1] + p[i][j+1])/4;
            }
        }
        set_bounds(DENSITY, p);
    }

    for (int i=1; i<CUBE_SIZE-1; i++) {
        for (int j=1; j<CUBE_SIZE-1; j++) {
            vx[i][j] -= 0.5 * (p[i+1][j] - p[i-1][j])/h;
            vy[i][j] -= 0.5 * (p[i][j+1] - p[i][j-1])/h;
        }
    }
    set_bounds(VX, vx);
    set_bounds(VY, vy);

}


void advect(ArrayData b, Array2D d, Array2D d0, Array2D u, Array2D v, float dt) {
    
    int i, j, i0, j0, i1, j1;
    float x, y, s0, t0, s1, t1, dt0;
    dt0 = dt * CUBE_SIZE;
    for ( i=0 ; i < CUBE_SIZE ; i++ ) {
        for ( j=0 ; j< CUBE_SIZE ; j++) {
            x = i - dt0 * u[i][j]; 
            y = j - dt0 * v[i][j];

            if (x<0.5) x=0.5; 
            if (x>CUBE_SIZE+0.5) x=CUBE_SIZE+0.5; 
            i0 = (int)x; 
            i1 = i0+1; 
            if (y<0.5) y=0.5; 
            if (y>CUBE_SIZE+0.5) y=CUBE_SIZE+0.5; 
            j0 = (int)y; 
            j1 = j0+1; 
            s1 = x-i0; 
            s0 = 1-s1; 
            t1 = y-j0; 
            t0 = 1-t1;
            d[i][j] =   s0 * (t0 * d0[i0][j0] + t1 * d0[i0][j1]) +
                        s1 * (t0 * d0[i1][j0] + t1 * d0[i1][j1]);
        } 
    }
    set_bounds(b, d);
}