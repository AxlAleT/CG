#include <stdio.h>
#include <math.h>
#include "utils.h"

// my image
int pixel[WIDTH][HEIGHT][3];

// sprite
int mario[16][16][3];

char* src_name  = "marioRCP.txt";
char* file_name = "out.ppm";

// colors
int piel[3] = {247, 164, 34};
int rojo[3] = {255, 23, 3};
int cafe[3] = {133, 75, 12};

void set_pixel(int i, int j, int c[3]){
	if (i<0 || j<0 || i>=WIDTH || j>= HEIGHT)
		return;
	pixel[i][j][0] = c[0];
	pixel[i][j][1] = c[1];
	pixel[i][j][2] = c[2];
}

void set_pixel_sprite(int i, int j, int c[3]){
	mario[i][j][0] = c[0];
	mario[i][j][1] = c[1];
	mario[i][j][2] = c[2];
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
				mario[i][j][0] = 
				mario[i][j][1] = 
				mario[i][j][2] = 0;
				break;
			}
		}
		fscanf(src, "%c", &c);
	}

	fclose(src);
	return 1;	//success
}

void show_sprite(){
	for (int j=15; j>=0; j--){
		for (int i=0; i<16; i++){
			printf("%d %d %d ", mario[i][j][0], mario[i][j][1], mario[i][j][2]);
		}
		printf("\n");
	}
}

void show_pixels(){
	for (int j=HEIGHT-1; j>=0; j--){
		for (int i=0; i<WIDTH; i++){
			printf("( ");
			for (int k=0; k<3; k++)
				printf("%d ", pixel[i][j][k]);
			printf(")");
		}
		printf("\n");
	}
}

int save_pixels(){
	FILE *dest = fopen(file_name, "w");
	if (!dest) return 0;	// error

	fprintf(dest, "P3\n");
	fprintf(dest, "%d %d\n", WIDTH, HEIGHT);
	fprintf(dest, "%d\n", 255);

	// valores RGB
	for (int j=HEIGHT-1; j>=0; j--){
		for (int i=0; i<WIDTH; i++)
			for (int k=0; k<3; k++)
				fprintf(dest, "%d ", pixel[i][j][k]);
		fprintf(dest, "\n");
	}

	fclose(dest);
	return 1;	// success
}

void draw_sprite(){
	for (int j=15; j>=0; j--){
		for (int i=0; i<16; i++){
			int x = i;
			int y = j;
			set_pixel(x, y, mario[i][j]);
		}
	}
	
}

// draw_fill_rect
// draw_fill_circle

void cleanPixel(){
	for (int j=0; j<HEIGHT; j++){
		for (int i=0; i<WIDTH; i++){
			pixel[i][j][0] = 0;
			pixel[i][j][1] = 0;
			pixel[i][j][2] = 0;
		}
	}
}

void copy_pixels(int from[WIDTH][HEIGHT][3], int to[WIDTH][HEIGHT][3]){
	for(int j=0; j<HEIGHT; j++){
		for(int i=0; i<WIDTH; i++){
			to[i][j][0] = from[i][j][0];
			to[i][j][1] = from[i][j][1];
			to[i][j][2] = from[i][j][2];
		}
	}
}

void scale_transform(int scaleFactor){
	int pixelCopy[WIDTH][HEIGHT][3];
	copy_pixels(pixel, pixelCopy);
	cleanPixel();
	for(int j=0; j<HEIGHT; j++){
		for(int i=0; i<WIDTH; i++){
			for(int dy=0; dy<scaleFactor; dy++){
				for(int dx=0; dx<scaleFactor; dx++){
					set_pixel(i*scaleFactor + dx, j*scaleFactor + dy, pixelCopy[i][j]);
				}
			}
		}
	}
}

void translation_transform(int dx, int dy){
	int pixelCopy[WIDTH][HEIGHT][3];

	copy_pixels(pixel, pixelCopy);

	cleanPixel();

	for (int j=0; j<HEIGHT; j++){
		for (int i=0; i<WIDTH; i++){
			set_pixel(i+dx, j+dy, pixelCopy[i][j]);
		}
	}
}


void rotation_transform(int angle){
	int pixelCopy[WIDTH][HEIGHT][3];
	copy_pixels(pixel, pixelCopy);
	cleanPixel();
	double rad = angle * PI / 180.0;
	for(int j=0; j<HEIGHT; j++){
		for(int i=0; i<WIDTH; i++){
			int x1 = (int)round(i*cos(rad) - j*sin(rad));
			int y1 = (int)round(i*sin(rad) + j*cos(rad));
			set_pixel(x1, y1, pixelCopy[i][j]);
		}
	}
}