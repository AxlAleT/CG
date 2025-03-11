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
void rotation_transform(int angle);
void cleanPixel();

// Nuevas funciones para la composición de transformaciones:
void composite_transform_bottom_left(int scaleFactor, int dx, int dy, int angle);
void composite_transform_center(int scaleFactor, int dx, int dy, int angle);
void apply_simple_transformations(int scaleFactor, int dx, int dy, int angle);

// draw_fill_rect
// draw_fill_circle

#endif
