#include <stdio.h>
#include "utils.h"

int main(){
	// load sprite
	if (!load_sprite()){
		printf("ERROR loading sprite\n");
		return 1;
	}

	// draw sprite on image
	draw_sprite(20, 80, 4, 4, 30.0);

	int gris[3] = {100,100,100};
	draw_fill_rect(70, 70, 10, 10, gris);

	int rojo[3] = {255,0,0};
	draw_fill_circle(30, 30, 20, rojo);

	int blanco[3] = {255,255,255};
	draw_circle0(80, 30, 20, blanco);	// unfilled circle v0
	draw_circle1(80, 80, 20, blanco);	// unfilled circle v1

	int verde[3] = {0, 255, 0};
	//draw_line(10, 10, 80, 10, verde);	// horizontal line
	//draw_line(10, 10, 10, 80, verde);	// vertical line

	//draw_line(10, 50, 40, 80, verde);	// slope 1
	//draw_line(10, 50, 40, 20, verde);	// slope -1

	draw_line(10, 50, 80, 80, verde);	// slope less than 1
	draw_line(50, 10, 80, 80, verde);	// slope less than 1

	//draw_triangle0(10, 10, 80, 80, verde);

	if (!save_pixels()){
		printf("ERROR saving image\n");
		return 1;
	}

	printf("OK\n");
	return 0;
}



