#ifndef POLY_H
#define POLY_H

#define MAX_INTERSECT 100  // Maximum number of intersections

#include <math.h>
#include "utils.h"

typedef struct {
    int x[MAX_INTERSECT]; // Array of x-coordinates of intersections
    int count; // Count of intersections
} Intersections;


void draw_poly(int v[][2], int n, int c[3]);

void draw_fill_poly(int v[][2], int n, int c[3]);

void draw_textured_poly(int v[][2], int n, int sprite[16][16][3], float m_inv[2][2]);


#endif
