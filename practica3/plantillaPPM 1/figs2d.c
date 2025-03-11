#include <stdio.h>
#include "utils.h"

int main(){
	// load sprite
	if (!load_sprite()){
		printf("ERROR loading sprite\n");
		return 1;
	}

	// draw sprite on image
	draw_sprite(60, 60, 4, 4, 30.0);

	int gris[3] = {100,100,100};
	//draw_fill_rect(50, 50, 10, 10, gris);

	int rojo[3] = {255,0,0};
	//draw_fill_circle(50, 50, 20, rojo);

	int blanco[3] = {255,255,255};
	//draw_circle0(50, 50, 20, blanco);	// unfilled circle v0
	//draw_circle1(50, 50, 20, blanco);	// unfilled circle v1

	int verde[3] = {0, 255, 0};
	//draw_line(10, 10, 80, 10, verde);	// horizontal line
	//draw_line(10, 10, 10, 80, verde);	// vertical line

	//draw_line(10, 50, 40, 80, verde);	// slope 1
	//draw_line(10, 50, 40, 20, verde);	// slope -1

	//draw_line(10, 50, 80, 80, verde);	// slope less than 1

	if (!save_pixels()){
		printf("ERROR saving image\n");
		return 1;
	}

	printf("OK\n");
	return 0;
}



