#ifndef MY2D_H
#define MY2D_H

#include <math.h>
#include "utils.h"

#define MAX_INTERSECT 100  // Maximum number of intersections

typedef struct {
    int x[MAX_INTERSECT];  // Array of x-coordinates of intersections
    int count;             // Count of intersections
} Intersections;

void draw_rect(int x1, int y1, int x2, int y2, int color[3]);

void draw_fill_rect(int x1, int y1, int x2, int y2, int color[3]);

void draw_fill_circle(int cx, int cy, int r, int color[3]);

void draw_circle_v0(int cx, int cy, int r, int color[3]);

void draw_circle_v1(int cx, int cy, int r, int color[3]);

void draw_line_naive(int x1, int y1, int x2, int y2, int color[3]);

void draw_line_incremental(int x1, int y1, int x2, int y2, int color[3]);

void draw_line(int x1, int y1, int x2, int y2, int color[3]);

void draw_triangle_v0(int x1, int y1, int x2, int y2, int color[3]);

void draw_line_midpoint(int x1, int y1, int x2, int y2, int color[3]);

void circle_points(int xc, int yc, int x, int y, int color[3]);

void draw_circle_midpoint_v0(int cx, int cy, int r, int color[3]);

void draw_circle_midpoint_v1(int cx, int cy, int r, int color[3]);

void draw_circle_midpoint_v2(int cx, int cy, int r, int color[3]);

void draw_ellipse_midpoint(int cx, int cy, int a, int b, int color[3]);

void draw_polygon_line(int cords[][2], int n, int color[3]);

// New function to generate coordinates only
void gen_polygon_coords(int n, int r, int cx, int cy, int cords[][2]);

// New function to generate star coordinates
void gen_star_coords(int n, int r, int cx, int cy, int cords[][2]);

// New functions
void intersect(int cords[][2], int n, int y, Intersections *isect);
void bubble_sort(Intersections *isect);
void draw_fill_polygon(int cords[][2], int n, int color[3]);

#endif