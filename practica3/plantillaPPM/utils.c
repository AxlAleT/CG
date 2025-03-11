#include <stdio.h>
#include <math.h>
#include "utils.h"

// my image
int pixel[WIDTH][HEIGHT][3];

// sprite
int mario[16][16][3];

char* src_name  = "marioRCP.txt";
char* file_name = "out.ppm";

// colors
int piel[3] = {247, 164, 34};
int rojo[3] = {255, 23, 3};
int cafe[3] = {133, 75, 12};

void set_pixel(int i, int j, int c[3]){
	if (i<0 || j<0 || i>=WIDTH || j>= HEIGHT)
		return;
	pixel[i][j][0] = c[0];
	pixel[i][j][1] = c[1];
	pixel[i][j][2] = c[2];
}

void set_pixel_sprite(int i, int j, int c[3]){
	mario[i][j][0] = c[0];
	mario[i][j][1] = c[1];
	mario[i][j][2] = c[2];
}

int load_sprite(){
	FILE *src = fopen(src_name, "r");
	if (!src) return 0;	// error

	char c;
	for (int j=15; j>=0; j--){
		for (int i=0; i<16; i++){
			fscanf(src, "%c", &c);
			switch(c){
			case 'P':
				set_pixel_sprite(i, j, piel);
				break;
			case 'R':
				set_pixel_sprite(i, j, rojo);
				break;
			case 'C':
				set_pixel_sprite(i, j, cafe);
				break;
			default:
				mario[i][j][0] = 
				mario[i][j][1] = 
				mario[i][j][2] = 0;
				break;
			}
		}
		fscanf(src, "%c", &c);
	}

	fclose(src);
	return 1;	//success
}

void show_sprite(){
	for (int j=0; j<16; j++){
		for (int i=0; i<16; i++){
			printf("%d %d %d ", mario[i][j][0], mario[i][j][1], mario[i][j][2]);
		}
		printf("\n");
	}
}

void show_pixels(){
	for (int j=HEIGHT-1; j>=0; j--){
		for (int i=0; i<WIDTH; i++){
			printf("( ");
			for (int k=0; k<3; k++)
				printf("%d ", pixel[i][j][k]);
			printf(")");
		}
		printf("\n");
	}
}

int save_pixels(){
	FILE *dest = fopen(file_name, "w");
	if (!dest) return 0;	// error

	fprintf(dest, "P3\n");
	fprintf(dest, "%d %d\n", WIDTH, HEIGHT);
	fprintf(dest, "%d\n", 255);

	// valores RGB
	for (int j=HEIGHT-1; j>=0; j--){
		for (int i=0; i<WIDTH; i++)
			for (int k=0; k<3; k++)
				fprintf(dest, "%d ", pixel[i][j][k]);
		fprintf(dest, "\n");
	}

	fclose(dest);
	return 1;	// success
}

void draw_sprite(int tx, int ty, int sx, int sy, float ang){
	ang *= PI/180.0;
	for (int j=0; j<16; j++){
		for (int i=0; i<16; i++){
			int x = sx*(i-8);
			int y = sy*(j-8);

			float x2 = x*cos(ang) - y*sin(ang);
			float y2 = x*sin(ang) + y*cos(ang);

			x2 += tx;
			y2 += ty;

			set_pixel((int)x2, (int)y2, mario[i][j]);
			set_pixel((int)x2-1, (int)y2, mario[i][j]);
			set_pixel((int)x2+1, (int)y2, mario[i][j]);
			set_pixel((int)x2, (int)y2-1, mario[i][j]);
			set_pixel((int)x2, (int)y2+1, mario[i][j]);
		}
	}
	
}

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

void draw_rect(int x1, int y1, int x2, int y2, int color[3]){
    // swap coords if needed
    if (x1 > x2) swap(&x1, &x2);
    if (y1 > y2) swap(&y1, &y2);

    // two horizontal lines
    for (int i = x1; i <= x2; i++) {
        set_pixel(i, y1, color);
        set_pixel(i, y2, color);
    }

    // two vertical lines
    for (int j = y1; j <= y2; j++) {
        set_pixel(x1, j, color);
        set_pixel(x2, j, color);
    }
}

void draw_fill_rect(int x1, int y1, int x2, int y2, int color[3]){
    // swap coords if needed
    if (x1 > x2) swap(&x1, &x2);
    if (y1 > y2) swap(&y1, &y2);

    // set pixels
    for (int j = y1; j <= y2; j++) {
        for (int i = x1; i <= x2; i++) {
            set_pixel(i, j, color);
        }
    }
}

//all the draw cirlce functions are based on equation of a circle centered at a point

void draw_fill_circle(int cx, int cy, int r, int color[3]){
    // define boundary
    int x1 = cx - r;
    int y1 = cy - r;
    int x2 = cx + r;
    int y2 = cy + r;

    // set pixels
    for (int j = y1; j <= y2; j++) {
        for (int i = x1; i <= x2; i++) {
            int dx = i - cx;
            int dy = j - cy;
            if (dx*dx + dy*dy <= r*r) {
                set_pixel(i, j, color);
            }
        }
    }
}

void draw_circle0(int cx, int cy, int r, int color[3]){
    // draw filled circle with color
    draw_fill_circle(cx, cy, r, color);

    int bgc[] = {0,0,0};    // background color
    // draw filled circle with background color
    draw_fill_circle(cx, cy, r-1, bgc);
}

void draw_circle1(int cx, int cy, int r, int color[3]){
    // define boundary
    int x1 = cx - r;
    int y1 = cy - r;
    int x2 = cx + r;
    int y2 = cy + r;

    // set pixels
    float r2 = (r - 0.5) * (r - 0.5);
    float r3 = (r + 0.5) * (r + 0.5);

    for (int j = y1; j <= y2; j++) {
        for (int i = x1; i <= x2; i++) {
            float dx = i - cx;
            float dy = j - cy;
            float d2 = dx*dx + dy*dy;
            if (d2 >= r2 && d2 <= r3) {
                set_pixel(i, j, color);
            }
        }
    }
}

void draw_line_incremental(int x1, int y1, int x2, int y2, int color[3]){
    int dx = x2 - x1;
    int dy = y2 - y1;
    
    if (abs(dx) > abs(dy)) {
        if (x1 > x2) {
            swap(&x1, &x2);
            swap(&y1, &y2);
        }
        float m = (float)dy / dx;
        float y = y1;
        for (int x = x1; x <= x2; x++) {
            set_pixel(x, round(y), color);
            y += m;
        }
    } else {
        if (y1 > y2) {
            swap(&x1, &x2);
            swap(&y1, &y2);
        }
        float m = (float)dx / dy;
        float x = x1;
        for (int y = y1; y <= y2; y++) {
            set_pixel(round(x), y, color);
            x += m;
        }
    }
}

void draw_line(int x1, int y1, int x2, int y2, int color[3]){
    // check if line is horizontal and draw it optimally
    if (y1 == y2) {
        if (x1 > x2) swap(&x1, &x2);
        for (int i = x1; i <= x2; i++) {
            set_pixel(i, y1, color);
        }
        return;
    }

    // check if line is vertical and draw it optimally
    if (x1 == x2) {
        if (y1 > y2) swap(&y1, &y2);
        for (int j = y1; j <= y2; j++) {
            set_pixel(x1, j, color);
        }
        return;
    }

    // check if line is diagonal with slope 1 and draw it optimally
    if (x2 - x1 == y2 - y1) {
        if (x1 > x2) {
            swap(&x1, &x2);
            swap(&y1, &y2);
        }
        int j = y1;
        for (int i = x1; i <= x2; i++, j++) {
            set_pixel(i, j, color);
        }
        return;
    }

    // check if line is diagonal with slope -1 and draw it optimally
    if (x2 - x1 == y1 - y2) {
        if (x1 > x2) {
            swap(&x1, &x2);
            swap(&y1, &y2);
        }
        int j = y1;
        for (int i = x1; i <= x2; i++, j--) {
            set_pixel(i, j, color);
        }
        return;
    }

    // any other case
    draw_line_incremental(x1, y1, x2, y2, color);
}












