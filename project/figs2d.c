#include <stdio.h>
#include "MY2D.h"

int main(){
    // load sprite
    if (!load_sprite()){
        printf("ERROR loading sprite\n");
        return 1;
    }

    // draw sprite on image
    //draw_sprite(20, 80, 4, 4, 30.0);

    int gris[3] = {100,100,100};
    //draw_fill_rect(70, 70, 10, 10, gris);

    int rojo[3] = {255,0,0};
    //draw_fill_circle(30, 30, 20, rojo);

    int blanco[3] = {255,255,255};
    //draw_circle0(80, 30, 20, blanco);	// unfilled circle v0
    //draw_circle1(80, 80, 20, blanco);	// unfilled circle v1

    int verde[3] = {0, 255, 0};

    draw_line(10, 50, 40, 100, verde);	// slope greater than 1
    draw_line(10, 50, 100, 40, verde);	// slope less than 1
    draw_line(10, 50, 100, 60, verde);	// slope greater than -1
    draw_line(10, 50, 40, 0, verde);	// slope less than -1
    
    draw_line(10, 50, 40, 80, rojo);	// slope  1
    draw_line(10, 50, 40, 20, rojo);	// slope -1

    int azul[3] = {0, 0, 255};
    //draw_triangle0(10, 10, 80, 80, azul);

    // Test lines in all four directions
	/*
    draw_line(10, 10, 90, 10, verde);  // horizontal right
    draw_line(90, 10, 10, 10, verde);  // horizontal left
    draw_line(10, 10, 10, 90, verde);  // vertical up
    draw_line(10, 90, 10, 10, verde);  // vertical down
	*/

    // Test lines with slopes less than 1 and greater than 1 but greater than zero
    //draw_line(10, 90, 10, 30, rojo);   // slope less than 1
    //draw_line(10, 10, 50, 90, rojo);   // slope greater than 1

    // Test lines with slopes greater than -1 and less than -1 but less than zero
    //draw_line(10, 90, 90, 50, azul);   // slope greater than -1
    //draw_line(10, 90, 50, 10, azul);   // slope less than -1

    if (!save_pixels()){
        printf("ERROR saving image\n");
        return 1;
    }

    printf("OK\n");
    return 0;
}