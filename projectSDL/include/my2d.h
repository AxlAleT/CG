#ifndef MY2D_H
#define MY2D_H

#include "utils.h"

void draw_rect(int x1, int y1, int x2, int y2, int color[3]);

void draw_fill_rect(int x1, int y1, int x2, int y2, int color[3]);

void draw_fill_circle(int cx, int cy, int r, int color[3]);

void line_incremental(int x1, int y1, int x2, int y2, int color[3]);

void line_midpoint(int x1, int y1, int x2, int y2, int color[3]);

void draw_line(int x1, int y1, int x2, int y2, int color[3]);


void draw_circle(int cx, int cy, int radius, int color[3]);


#endif
