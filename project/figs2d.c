#include <stdio.h>
#include "MY2D.h"

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

    // draw sprite on image
    //draw_sprite(20, 80, 4, 4, 30.0);

    //draw_fill_rect(70, 70, 10, 10, gray);

    //draw_fill_circle(30, 30, 20, red);

    //draw_circle_v0(80, 30, 20, white);  // unfilled circle v0
    //draw_circle_v1(80, 80, 20, white);  // unfilled circle v1

   /*  draw_line(10, 50, 40, 100, green);  // slope greater than 1
    draw_line(10, 50, 100, 40, green); // slope less than 1
    draw_line(10, 50, 100, 60, green); // slope greater than -1
    draw_line(10, 50, 40, 0, green);   // slope less than -1

    draw_line(10, 50, 40, 80, red);    // slope  1
    draw_line(10, 50, 40, 20, red);    // slope -1

    draw_circle_midpoint_v0(50, 50, 10, yellow);
    draw_circle_midpoint_v1(50, 50, 20, white);
    draw_circle_midpoint_v2(50, 50, 30, blue); */

    //draw_ellipse_midpoint(50, 50, 40, 10, yellow);

    gen_polygon(10, 30, 50, 50, red); // pentagon


    //draw_triangle_v0(10, 10, 80, 80, blue);

    // Test lines in all four directions
    /*
    draw_line(10, 10, 90, 10, green);  // horizontal right
    draw_line(90, 10, 10, 10, green);  // horizontal left
    draw_line(10, 10, 10, 90, green);  // vertical up
    draw_line(10, 90, 10, 10, green);  // vertical down
    */

    // Test lines with slopes less than 1 and greater than 1 but greater than zero
    //draw_line(10, 90, 10, 30, red);   // slope less than 1
    //draw_line(10, 10, 50, 90, red);   // slope greater than 1

    // Test lines with slopes greater than -1 and less than -1 but less than zero
    //draw_line(10, 90, 90, 50, blue);   // slope greater than -1
    //draw_line(10, 90, 50, 10, blue);   // slope less than -1

    if (!save_pixels()) {
        printf("ERROR saving image\n");
        return 1;
    }

    printf("OK\n");
    return 0;
}