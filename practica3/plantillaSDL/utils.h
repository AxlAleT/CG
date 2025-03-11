// utils.h

#ifndef UTILS_H
#define UTILS_H

// Image size
#define WIDTH  640
#define HEIGHT 480
#define PI 3.141592


// Fill image with color
void init_image(unsigned int color);

// Change image
void change_image_test();

// Set pixel with RGBA values
void set_pixel(int i, int j, int color[4]);


// mario section
void set_pixel_sprite(int i, int j, int c[4]);

int load_sprite();

void draw_sprite();




#endif

