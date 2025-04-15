#ifndef MY3D_H
#define MY3D_H

#include "MY2D.h"
#include <math.h>

// Draw a single vertex
void draw_vert(int i, int j, int c[3]);

// Draw multiple vertices
void draw_verts(float tx, float ty, float tz, float esc, float v[][3], int c[3], int n);

// Transform a vertex from 3D to 2D
void transform_vert(float *x, float *y, float *z, float tx, float ty, float tz, float ang, int *screen_x, int *screen_y);

// Draw 3D faces (quads)
void draw_faces(float v[][3], int f[][4], int c[3], int n);

// Draw a 3D cube with rotation (angle in degrees)
void draw_cube(float tx, float ty, float tz, float scale, float angle_degrees, int vert_color[3], int face_color[3]);

void create_grid(int n, int m, float v[][3], int e[][2]);

void draw_edges(float v[][3], int e[][2], int c[3], int n);

void draw_3d_grid(int n, int m, float tx, float ty, float tz, float scale, float angle_degrees, int color[3]);

#endif //MY3D_H
