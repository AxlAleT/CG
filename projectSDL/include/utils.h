
#ifndef UTILS_H
#define UTILS_H

// Image size
#define WIDTH  640
#define HEIGHT 480
#define PI 3.141592


void swap(int *a, int *b);


// Fill image with color
void init_image(unsigned int color);

// Change image
void change_image_test();

// Set pixel with RGBA values
void set_pixel(int i, int j, int color[3]);


// mario section
void set_pixel_sprite(int i, int j, int c[3]);

int load_mario_sprite();

void draw_mario_sprite(int tx, int ty, int sx, int sy, float ang);

int load_ppm(char* file_name, int sprite[16][16][3]);




#endif

