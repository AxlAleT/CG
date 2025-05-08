#include <stdio.h>
#include <math.h>
#include "utils.h"


extern unsigned int pixel[WIDTH*HEIGHT];


void swap(int *a, int *b){
	int aux = *a;
	*a = *b;
	*b = aux;
}

char full_path[256];



/********* MARIO section *********/

// Mario colors
int piel[3] = {247, 164, 34};
int rojo[3] = {255, 23, 3};
int cafe[3] = {133, 75, 12};
int fondo[3] = {0, 0, 0};

// Mario sprite
extern int mario[16][16][3];
char* resources_dir = "../resources/";
char* src_name  = "marioRCP.txt";

void set_pixel_sprite(int i, int j, int c[3]){
	mario[i][j][0] = c[0];
	mario[i][j][1] = c[1];
	mario[i][j][2] = c[2];
}

int load_mario_sprite(){
	snprintf(full_path, sizeof(full_path), "%s%s", resources_dir, src_name);
	FILE *src = fopen(full_path, "r");
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

void draw_mario_sprite(int tx, int ty, int sx, int sy, float ang){
	ang *= PI/180.0;
	for (int j=0; j<16; j++){
		for (int i=0; i<16; i++){
			int x = sx*(i-8);
			int y = sy*(j-8);

			float x2 = x*cos(ang) - y*sin(ang);
			float y2 = x*sin(ang) + y*cos(ang);

			x = tx+(int)x2+8;
			y = ty+(int)y2+8;

			set_pixel(x, y, mario[i][j]);
			set_pixel(x-1, y, mario[i][j]);
			set_pixel(x+1, y, mario[i][j]);
			set_pixel(x, y-1, mario[i][j]);
			set_pixel(x, y+1, mario[i][j]);
		}
	}
	
}

/********* MARIO section *********/


//extern int cube_top   [16][16][3];
//extern int cube_side  [16][16][3];
//extern int cube_bottom[16][16][3];

int load_ppm(char* file_name, int sprite[16][16][3]){
	FILE *archi = fopen(file_name, "r");
	if (!archi) return 0;	// error

	int w, h, aux;
	fscanf(archi, "P3\n");
	fscanf(archi, "%d %d\n", &w, &h);
	fscanf(archi, "%d\n", &aux);

	// valores RGB
	for (int j=h-1; j>=0; j--)
		for (int i=0; i<w; i++)
			for (int k=0; k<3; k++)
				fscanf(archi, "%d ", &sprite[i][j][k]);

	fclose(archi);
	return 1;	// success
}



// Fill image with color
void init_image(unsigned int color) {
	for (int j=HEIGHT-1; j>=0; j--)
		for (int i=0; i<WIDTH; i++)
			pixel[i+j*WIDTH] = color; // color in ARGB format for Fedora
}

// Change image
void change_image_test() {
	for (int j=HEIGHT-1; j>=0; j--)
		for (int i=0; i<WIDTH; i++)
			pixel[i+j*WIDTH] += 0x100;
}

// Set pixel with RGB values (alpha is always 255)
void set_pixel(int i, int j, int c[3]){
	if (i<0 || j<0 || i>=WIDTH || j>= HEIGHT)
		return;
	j = HEIGHT-j-1;
	// ARGB format (alpha first, then RGB)
	pixel[i+j*WIDTH] = (255<<24) | (c[0]<<16) | (c[1]<<8) | c[2];
}

// Draw text on screen with simple font rendering
void draw_text(int x, int y, const char* text, int color[3]) {
    int len = strlen(text);
    int char_width = 8;
    int char_height = 12;
    
    for (int i = 0; i < len; i++) {
        char c = text[i];
        
        // Very simple font rendering - just rectangular shapes for now
        for (int cy = 0; cy < char_height; cy++) {
            for (int cx = 0; cx < char_width; cx++) {
                // Draw only specific pixels to form simple letter shapes
                if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || 
                    (c >= '0' && c <= '9') || c == ':' || c == ',' || c == '.' || 
                    c == '-' || c == '_' || c == ' ') {
                    
                    // Skip drawing for spaces
                    if (c == ' ') continue;
                    
                    // Very basic font rendering - just a filled rectangle for each character
                    // with some simple patterns based on the character shape
                    if (cx == 0 || cx == char_width-1 || cy == 0 || cy == char_height-1) {
                        set_pixel(x + i * char_width + cx, y + cy, color);
                    }
                    
                    // For some characters, add specific lines or points
                    if ((c == 'A' || c == 'E' || c == 'F' || c == 'H') && cy == char_height/2) {
                        set_pixel(x + i * char_width + cx, y + cy, color);
                    }
                    
                    if (c == ':' && (cy == char_height/3 || cy == 2*char_height/3) && 
                        cx > char_width/3 && cx < 2*char_width/3) {
                        set_pixel(x + i * char_width + cx, y + cy, color);
                    }
                }
            }
        }
    }
}
