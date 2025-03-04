

#ifndef UTILS_H
#define UTILS_H

#define WIDTH  100
#define HEIGHT 100
#define PI 3.141592

void set_pixel(int i, int j, int c[3]);

void set_pixel_sprite(int i, int j, int c[3]);

int load_sprite();

void show_sprite();

void show_pixels();

int save_pixels();

void draw_sprite();

void scale_transform(int scaleFactor);

void translation_transform(int dx, int dy);

void cleanPixel();

void rotation_transform(int angle);


// draw_fill_rect
// draw_fill_circle


#endif


