// utils.c

#include <stdio.h>
#include <math.h>
#include "utils.h"

extern unsigned int pixel[WIDTH*HEIGHT];

// Some colors
int   red[4] = {255, 0, 0, 255};
int green[4] = {0, 255, 0, 255};
int  blue[4] = {0, 0, 255, 255};
int black[4] = {0, 0, 0, 255};
int white[4] = {255, 255, 255, 255};
int    cyan[4] = {0, 255, 255, 255};
int magenta[4] = {255, 0, 255, 255};
int yellow [4] = {255, 255, 0, 255};
int fondo [4] = {0, 0, 0, 0};



/********* MARIO section *********/

// Mario colors
int piel[4] = {247, 164, 34, 255};
int rojo[4] = {255, 23, 3, 255};
int cafe[4] = {133, 75, 12, 255};

// Mario sprite
int mario[16][16][4];
char* src_name  = "marioRCP.txt";

void set_pixel_sprite(int i, int j, int c[4]){
	mario[i][j][0] = c[0];
	mario[i][j][1] = c[1];
	mario[i][j][2] = c[2];
	mario[i][j][3] = c[3];
}

int load_sprite(){
	FILE *src = fopen(src_name, "r");
	if (!src) return 0;	// error

	char c;
	for (int j=15; j>=0; j--){
		for (int i=0; i<16; i++){
			fscanf(src, "%c", &c);
			switch(c){
			case 'P':
				set_pixel_sprite(i, j, piel);
				break;
			case 'R':
				set_pixel_sprite(i, j, rojo);
				break;
			case 'C':
				set_pixel_sprite(i, j, cafe);
				break;
			default:
				set_pixel_sprite(i, j, fondo);
				break;
			}
		}
		fscanf(src, "%c", &c);
	}

	fclose(src);
	return 1;	//success
}

void draw_sprite(){
	for (int j=0; j<16; j++){
		for (int i=0; i<16; i++){
			int x = i;
			int y = j;

			set_pixel(x, y, mario[i][j]);
		}
	}
	
}

/********* MARIO section *********/



// Fill image with color
void init_image(unsigned int color) {
	for (int j=HEIGHT-1; j>=0; j--)
		for (int i=0; i<WIDTH; i++)
			pixel[i+j*WIDTH] = color; // color in RGBA
}

// Change image
void change_image_test() {
	for (int j=HEIGHT-1; j>=0; j--)
		for (int i=0; i<WIDTH; i++)
			pixel[i+j*WIDTH] += 0x100;
}

// Set pixel with RGBA values
void set_pixel(int i, int j, int c[4]){
	if (i<0 || j<0 || i>=WIDTH || j>= HEIGHT)
		return;
	j = HEIGHT-j-1;
	unsigned int p = c[0];
	pixel[i+j*WIDTH] = ((p<<8|c[1])<<8|c[2])<<8|c[3];
}




