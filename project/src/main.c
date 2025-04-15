#include <stdio.h>
#include "MY3D.h"

int main() {
    // load sprite
    if (!load_sprite()) {
        printf("ERROR loading sprite\n");
        return 1;
    }

    int blue[3] = {0, 0, 255};
    int yellow[3] = {255, 255, 0};
    int green[3] = {0, 255, 0};
    int gray[3] = {100, 100, 100};
    int red[3] = {255, 0, 0};
    int white[3] = {255, 255, 255};

    // Draw a rotating cube at the center of the screen
    draw_cube(0, 0, -10, 2.0, 30.0, red, gray);

    // Draw a 10x10 grid, positioned behind the cube, rotated 20 degrees
    draw_3d_grid(10, 10, 0, 0, -15, 1.0, -20.0, green);

    if (!save_pixels()) {
        printf("ERROR saving image\n");
        return 1;
    }

    printf("OK\n");
    return 0;
}