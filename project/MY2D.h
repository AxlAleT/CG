#ifndef MY2D_H
#define MY2D_H

#include <math.h>
#include "utils.h"

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

#endif