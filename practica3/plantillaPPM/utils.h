

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

void draw_sprite(int tx, int ty, int sx, int sy, float ang);

void swap(int *a, int *b);

void draw_rect(int x1, int y1, int x2, int y2, int color[3]);

void draw_fill_rect(int x1, int y1, int x2, int y2, int color[3]);

void draw_fill_circle(int cx, int cy, int r, int color[3]);

void draw_circle0(int cx, int cy, int r, int color[3]);

void draw_circle1(int cx, int cy, int r, int color[3]);

void draw_line_incremental(int x1, int y1, int x2, int y2, int color[3]);

void draw_line(int x1, int y1, int x2, int y2, int color[3]);

void draw_triangle0(int x1, int y1, int x2, int y2, int color[3]);

#endif


