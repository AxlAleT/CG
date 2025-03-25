
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH  100
#define HEIGHT 100
#define PI 3.141592

void set_pixel(int i, int j, int c[3]);

void set_pixel_sprite(int i, int j, int c[3]);

int load_sprite();

void show_sprite();

void show_pixels();

int save_pixels();

void swap(int *a, int *b);

void draw_sprite(int tx, int ty, int sx, int sy, float ang);

#endif