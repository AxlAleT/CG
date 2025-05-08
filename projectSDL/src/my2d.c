#include <stdio.h>
#include <math.h>
#include "utils.h"

void draw_rect(int x1, int y1, int x2, int y2, int color[3]) {
    // swap coords if needed
    if (x2 < x1) swap(&x1, &x2);
    if (y2 < y1) swap(&y1, &y2);

    // horizontal lines
    for (int i = x1; i <= x2; i++) {
        set_pixel(i, y1, color);
        set_pixel(i, y2, color);
    }

    // vertical lines
    for (int j = y1; j <= y2; j++) {
        set_pixel(x1, j, color);
        set_pixel(x2, j, color);
    }
}

void draw_fill_rect(int x1, int y1, int x2, int y2, int color[3]) {
    // swap coords if needed
    if (x2 < x1) swap(&x1, &x2);
    if (y2 < y1) swap(&y1, &y2);

    // set pixels
    for (int j = y1; j <= y2; j++)
        for (int i = x1; i <= x2; i++)
            set_pixel(i, j, color);
}

void draw_fill_circle(int cx, int cy, int r, int color[3]) {
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
            if (dx * dx + dy * dy <= r * r) {
                set_pixel(i, j, color);
            }
        }
    }
}


void line_incremental(int x1, int y1, int x2, int y2, int color[3]) {
    int dx = x2 - x1;
    int dy = y2 - y1;

    if (abs(dx) > abs(dy)) {
        if (x1 > x2) {
            swap(&x1, &x2);
            swap(&y1, &y2);
        }
        float m = (float) dy / dx;
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
        float m = (float) dx / dy;
        float x = x1;
        for (int y = y1; y <= y2; y++) {
            set_pixel(round(x), y, color);
            x += m;
        }
    }
}

void line_midpoint(int x1, int y1, int x2, int y2, int color[3]) {
    int steep = abs(y2 - y1) > abs(x2 - x1);
    if (steep) {
        int temp;
        // swap x and y for both endpoints
        temp = x1;
        x1 = y1;
        y1 = temp;
        temp = x2;
        x2 = y2;
        y2 = temp;
    }
    if (x1 > x2) {
        int temp;
        // swap endpoints to ensure left-to-right drawing
        temp = x1;
        x1 = x2;
        x2 = temp;
        temp = y1;
        y1 = y2;
        y2 = temp;
    }
    int dx = x2 - x1;
    int dy = abs(y2 - y1);
    int error = dx / 2;
    int ystep = (y1 < y2) ? 1 : -1;
    int y = y1;

    for (int x = x1; x <= x2; x++) {
        if (steep) {
            set_pixel(y, x, color); // swapped coordinates
        } else {
            set_pixel(x, y, color);
        }
        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
}

void draw_line(int x1, int y1, int x2, int y2, int color[3]) {
    // horizontal line
    if (y1 == y2) {
        // swap coords if needed
        if (x2 < x1) {
            swap(&x1, &x2);
            swap(&y1, &y2);
        }

        for (int i = x1; i <= x2; i++)
            set_pixel(i, y1, color);
        return;
    }

    // vertical line
    if (x1 == x2) {
        // swap coords if needed
        if (y2 < y1) {
            swap(&x1, &x2);
            swap(&y1, &y2);
        }

        for (int j = y1; j <= y2; j++)
            set_pixel(x1, j, color);
        return;
    }

    // swap coords if needed
    if (x2 < x1) {
        swap(&x1, &x2);
        swap(&y1, &y2);
    }

    // diagonal line slope 1
    if (y2 - y1 == x2 - x1) {
        int j;
        for (int i = x1, j = y1; i <= x2; i++, j++)
            set_pixel(i, j, color);
        return;
    }

    // diagonal line slope -1
    if (y2 - y1 == x1 - x2) {
        int j;
        for (int i = x1, j = y1; i <= x2; i++, j--)
            set_pixel(i, j, color);
        return;
    }

    // slope != +-1
    //line_incremental(x1, y1, x2, y2, color);
    line_midpoint(x1, y1, x2, y2, color);
}


void circle_points(int xc, int yc, int i, int j, int c[3]) {
    set_pixel(xc + i, yc + j, c);
    set_pixel(xc - i, yc + j, c);
    set_pixel(xc + i, yc - j, c);
    set_pixel(xc - i, yc - j, c);
    set_pixel(xc + j, yc + i, c);
    set_pixel(xc - j, yc + i, c);
    set_pixel(xc + j, yc - i, c);
    set_pixel(xc - j, yc - i, c);
}

void draw_circle(int cx, int cy, int radius, int color[3]) {
    int x = 0, y = radius;
    int d = 3 - 2 * radius;
    circle_points(cx, cy, x, y, color);
    while (y >= x) {
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else
            d = d + 4 * x + 6;

        x++;

        circle_points(cx, cy, x, y, color);
    }
}
