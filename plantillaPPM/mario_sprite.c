#include <stdio.h>
#include "utils.h"


int main(){
	// load sprite
	if (!load_sprite()){
		printf("ERROR loading sprite\n");
		return 1;
	}



	// draw sprite on image
	draw_sprite();

	scale_transform(3);
	rotation_transform(45);	
	translation_transform(24, 24);


	if (!save_pixels()){
		printf("ERROR saving image\n");
		return 1;
	}

	printf("OK\n");
	return 0;
}



