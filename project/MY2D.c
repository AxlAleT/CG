
#include "MY2D.h"

void draw_rect(int x1, int y1, int x2, int y2, int color[3])
{
    // swap coords if needed
    if (x1 > x2)
        swap(&x1, &x2);
    if (y1 > y2)
        swap(&y1, &y2);

    // two horizontal lines
    for (int i = x1; i <= x2; i++)
    {
        set_pixel(i, y1, color);
        set_pixel(i, y2, color);
    }

    // two vertical lines
    for (int j = y1; j <= y2; j++)
    {
        set_pixel(x1, j, color);
        set_pixel(x2, j, color);
    }
}

void draw_fill_rect(int x1, int y1, int x2, int y2, int color[3])
{
    // swap coords if needed
    if (x1 > x2)
        swap(&x1, &x2);
    if (y1 > y2)
        swap(&y1, &y2);

    // set pixels
    for (int j = y1; j <= y2; j++)
    {
        for (int i = x1; i <= x2; i++)
        {
            set_pixel(i, j, color);
        }
    }
}

// all the draw cirlce functions are based on equation of a circle centered at a point

void draw_fill_circle(int cx, int cy, int r, int color[3])
{
    // define boundary
    int x1 = cx - r;
    int y1 = cy - r;
    int x2 = cx + r;
    int y2 = cy + r;

    // set pixels
    for (int j = y1; j <= y2; j++)
    {
        for (int i = x1; i <= x2; i++)
        {
            int dx = i - cx;
            int dy = j - cy;
            if (dx * dx + dy * dy <= r * r)
            {
                set_pixel(i, j, color);
            }
        }
    }
}

void draw_circle0(int cx, int cy, int r, int color[3])
{
    // draw filled circle with color
    draw_fill_circle(cx, cy, r, color);

    int bgc[] = {0, 0, 0}; // background color
    // draw filled circle with background color
    draw_fill_circle(cx, cy, r - 1, bgc);
}

void draw_circle1(int cx, int cy, int r, int color[3])
{
    // define boundary
    int x1 = cx - r;
    int y1 = cy - r;
    int x2 = cx + r;
    int y2 = cy + r;

    // set pixels
    float r2 = (r - 0.5) * (r - 0.5);
    float r3 = (r + 0.5) * (r + 0.5);

    for (int j = y1; j <= y2; j++)
    {
        for (int i = x1; i <= x2; i++)
        {
            float dx = i - cx;
            float dy = j - cy;
            float d2 = dx * dx + dy * dy;
            if (d2 >= r2 && d2 <= r3)
            {
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

void draw_line_midpoint(int x1, int y1, int x2, int y2, int color[3]) {
    int dx = x2 - x1;
    int dy = y2 - y1;

    if (abs(dx) > abs(dy)) {
        if (x1 > x2) {
            swap(&x1, &x2);
            swap(&y1, &y2);
        }

        int d = 2 * dy - dx;
        int incrE = 2 * dy;
        int incrNE = 2 * (dy - dx);
        
        int x = x1;
        int y = y1;
        set_pixel(x, y, color);
        while (x < x2) {
            if (d <= 0) {
                d += incrE;
                x++;
            } else {
                d += incrNE;
                x++;
                y++;
            }
            set_pixel(x, y, color);
        }
        
    } else {
        if (y1 > y2) {
            swap(&x1, &x2);
            swap(&y1, &y2);
        }

        int d = 2 * dx - dy;
        int incrE = 2 * dx;
        int incrNE = 2 * (dx - dy);
        
        int x = y1;
        int y = x1;
        set_pixel(x, y, color);
        while (x < y2) {
            if (d <= 0) {
                d += incrE;
                x++;
            } else {
                d += incrNE;
                x++;
                y++;
            }
            set_pixel(y, x, color);
        }
        
    }
}

void draw_line_function(int x1, int y1, int x2, int y2, int color[3])
{
    float m = (float)(y2 - y1) / (x2 - x1);
    float b = y1 - m * x1;

    if (abs(x2 - x1) > abs(y2 - y1))
    {
        if (x1 > x2)
        {
            swap(&x1, &x2);
            swap(&y1, &y2);
        }
        for (int i = x1; i <= x2; i++)
        {
            set_pixel(i, round(m * i + b), color);
        }
    }
    else
    {
        if (y1 > y2)
        {
            swap(&x1, &x2);
            swap(&y1, &y2);
        }
        for (int j = y1; j <= y2; j++)
        {
            set_pixel(round((j - b) / m), j, color);
        }
    }
}

void draw_line(int x1, int y1, int x2, int y2, int color[3])
{
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (1) {
        set_pixel(x1, y1, color);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void draw_triangle0(int x1, int y1, int x2, int y2, int color[3])
{

    // swap cords if needed
    if (x2 < x1)
    {
        swap(&x1, &x2);
        swap(&y1, &y2);
    }

    int m = (y2 - y1) / (x2 - x1);
    // set pixels
    for (int i = x1; i <= x2; i++)
    {
        int y;
        for (int j = y1; j <= y2; j++)
        {
            y = y1 + (i - x1) * m;
            set_pixel(i, y, color);
        }
    }
}
