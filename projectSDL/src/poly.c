
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utils.h"
#include "my2d.h"

#define MAX_INTERSECT 100

// intersections list
typedef struct {
    int y;
    int x[MAX_INTERSECT];
    int count;
}scanline;


void create_reg_poly(int x, int y, int r, int n, int v[][2]){
	float x2, y2, ang=90.0;
	for (int i=0; i<n; i++, ang+=360.0/n){
		x2 = r*cos(ang*PI/180);
		y2 = r*sin(ang*PI/180);
		v[i][0] = (int)x2 + x;
		v[i][1] = (int)y2 + y;
	}
}

void create_reg_star(int x, int y, int r, int n, int v[][2]){
	// TO DO
}


void draw_poly(int v[][2], int n, int c[3]){
	for (int i=0; i<n-1; i++)
		draw_line(v[i][0], v[i][1], v[i+1][0], v[i+1][1], c);
	draw_line(v[n-1][0], v[n-1][1], v[0][0], v[0][1], c);
}


int intersect(int p1[2], int p2[2], int y){
	int x1 = p1[0];
	int y1 = p1[1];
	int x2 = p2[0];
	int y2 = p2[1];

	if (y1 == y2) return -1;

	if (y2<y1){
		swap(&y1, &y2);
		swap(&x1, &x2);
	}

	if (y1<y && y<y2) {
		float x = x1 + (y-y1)*(x2-x1)/(y2-y1);
		return (int)x;
	}

	return -1;
}

void sort(int a[], int b[], int n) {
	// TO DO
}

void draw_fill_poly(int v[][2], int n, int c[3]){
	// TO DO
}

void draw_textured_poly(int v[][2], int n, int sprite[16][16][3], float m_inv[2][2]){
	// TO DO
}






