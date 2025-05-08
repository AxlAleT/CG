#include <stdio.h>
#include <math.h>
#include "utils.h"

void draw_rect(int x1, int y1, int x2, int y2, int color[3]){
	// swap coords if needed
	if (x2<x1) swap(&x1, &x2);
	if (y2<y1) swap(&y1, &y2);

	// horizontal lines
	for (int i=x1; i<=x2; i++){
		set_pixel(i, y1, color);
		set_pixel(i, y2, color);
	}

	// vertical lines
	for (int j=y1; j<=y2; j++){
		set_pixel(x1, j, color);
		set_pixel(x2, j, color);
	}
}

void draw_fill_rect(int x1, int y1, int x2, int y2, int color[3]){
	// swap coords if needed
	if (x2<x1) swap(&x1, &x2);
	if (y2<y1) swap(&y1, &y2);

	// set pixels
	for (int j=y1; j<=y2; j++)
		for (int i=x1; i<=x2; i++)
			set_pixel(i, j, color);
}

void draw_fill_circle(int cx, int cy, int r, int color[3]){
	// TO DO
}



void line_incremental(int x1, int y1, int x2, int y2, int color[3]){
	// TO DO
}

void line_midpoint(int x1, int y1, int x2, int y2, int color[3]){
	// TO DO
}

void draw_line(int x1, int y1, int x2, int y2, int color[3]){
	// horizontal line
	if (y1 == y2){
		// swap coords if needed
		if (x2<x1){
			swap(&x1, &x2);
			swap(&y1, &y2);
		}

		for (int i=x1; i<=x2; i++)
			set_pixel(i, y1, color);
		return;
	}

	// vertical line
	if (x1 == x2){
		// swap coords if needed
		if (y2<y1){
			swap(&x1, &x2);
			swap(&y1, &y2);
		}

		for (int j=y1; j<=y2; j++)
			set_pixel(x1, j, color);
		return;
	}

	// swap coords if needed
	if (x2<x1){
		swap(&x1, &x2);
		swap(&y1, &y2);
	}

	// diagonal line slope 1
	if (y2-y1 == x2-x1){
		int j;
		for (int i=x1, j=y1; i<=x2; i++, j++)
			set_pixel(i, j, color);
		return;
	}

	// diagonal line slope -1
	if (y2-y1 == x1-x2){
		int j;
		for (int i=x1, j=y1; i<=x2; i++, j--)
			set_pixel(i, j, color);
		return;
	}

	// slope != +-1
	//line_incremental(x1, y1, x2, y2, color);
	line_midpoint(x1, y1, x2, y2, color);
}


void circle_points(int xc, int yc, int i, int j, int c[3]){
	// TO DO
}

void draw_circle(int cx, int cy, int radius, int color[3]){
	// TO DO
}








