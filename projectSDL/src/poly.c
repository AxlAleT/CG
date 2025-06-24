#include <poly.h>
#include "my2d.h"

#define MAX_INTERSECT 100

void create_reg_poly(int x, int y, int r, int n, int v[][2]) {
    float x2, y2, ang = 90.0;
    for (int i = 0; i < n; i++, ang += 360.0 / n) {
        x2 = r * cos(ang * PI / 180);
        y2 = r * sin(ang * PI / 180);
        v[i][0] = (int) x2 + x;
        v[i][1] = (int) y2 + y;
    }
}

void create_reg_star(int x, int y, int r, int n, int v[][2]) {
    int polygon[n][2];

    for (int i = 0; i < n; i++) {
        polygon[i][0] = x + r * cos(2 * PI * i / n);
        polygon[i][1] = y + r * sin(2 * PI * i / n);
    }

    // Calculate k for star reordering (same as in draw_star_line)
    int k = (n % 2 == 0) ? (n / 2 + 1) : n / 2;
    for (int i = 0; i < n; i++) {
        int index = (i * k) % n;
        v[i][0] = polygon[index][0];
        v[i][1] = polygon[index][1];
    }
}


void draw_poly(int v[][2], int n, int c[3]) {
    for (int i = 0; i < n - 1; i++)
        draw_line(v[i][0], v[i][1], v[i + 1][0], v[i + 1][1], c);
    draw_line(v[n - 1][0], v[n - 1][1], v[0][0], v[0][1], c);
}


void intersect(int cords[][2], int n, int y, Intersections *isect) {
    isect->count = 0;

    // Check each edge of the polygon
    for (int i = 0; i < n; i++) {
        int x1 = cords[i][0];
        int y1 = cords[i][1];
        int x2 = cords[(i + 1) % n][0];
        int y2 = cords[(i + 1) % n][1];

        // Check if the horizontal line at y intersects with this edge
        if ((y1 <= y && y2 > y) || (y2 <= y && y1 > y)) {
            // Calculate x-coordinate of intersection
            // Using the line equation: x = x1 + (x2-x1)*(y-y1)/(y2-y1)
            if (y1 != y2) {
                // Avoid division by zero
                int x = x1 + (x2 - x1) * (y - y1) / (y2 - y1);

                // Ensure we don't exceed the array bounds
                if (isect->count < MAX_INTERSECT) {
                    isect->x[isect->count++] = x;
                }
            }
        }
    }
}

void sort(Intersections *isect) {
    for (int i = 0; i < isect->count - 1; i++) {
        for (int j = 0; j < isect->count - i - 1; j++) {
            if (isect->x[j] > isect->x[j + 1]) {
                int temp = isect->x[j];
                isect->x[j] = isect->x[j + 1];
                isect->x[j + 1] = temp;
            }
        }
    }
}

void draw_fill_poly(int v[][2], int n, int c[3]) {
    // Find min and max y-coordinates to determine the scan line range
    int min_y = v[0][1];
    int max_y = v[0][1];

    for (int i = 1; i < n; i++) {
        if (v[i][1] < min_y) min_y = v[i][1];
        if (v[i][1] > max_y) max_y = v[i][1];
    }

    // Scan line algorithm
    Intersections isect;
    for (int y = min_y; y <= max_y; y++) {
        // Find intersections
        intersect(v, n, y, &isect);

        // Sort intersections
        sort(&isect);

        // Draw horizontal lines between pairs of intersections
        for (int i = 0; i < isect.count; i += 2) {
            if (i + 1 < isect.count) {
                draw_line(isect.x[i], y, isect.x[i + 1], y, c);
            }
        }
    }

    // Optionally draw the outline of the polygon
    draw_poly(v, n, c);
}

void draw_textured_poly(int v[][2], int n, int sprite[16][16][3], float m_inv[2][2]) {
    // Find min and max y-coordinates to determine the scan line range
    int min_y = v[0][1];
    int max_y = v[0][1];
    int min_x = v[0][0];
    int max_x = v[0][0];

    // Find the bounding box of the polygon
    for (int i = 1; i < n; i++) {
        if (v[i][1] < min_y) min_y = v[i][1];
        if (v[i][1] > max_y) max_y = v[i][1];
        if (v[i][0] < min_x) min_x = v[i][0];
        if (v[i][0] > max_x) max_x = v[i][0];
    }

    // Scan line algorithm
    Intersections isect;
    for (int y = min_y; y <= max_y; y++) {
        // Find intersections with the scan line
        intersect(v, n, y, &isect);

        // Sort intersections
        sort(&isect);

        // Process pairs of intersections
        for (int i = 0; i < isect.count; i += 2) {
            if (i + 1 < isect.count) {
                // Fill between each pair of intersections
                for (int x = isect.x[i]; x <= isect.x[i + 1]; x++) {
                    // Apply inverse transformation to map screen coordinates to texture coordinates
                    float u = m_inv[0][0] * x + m_inv[0][1] * y;
                    float v = m_inv[1][0] * x + m_inv[1][1] * y;

                    // Map to texture space (0-15)
                    int tx = (int) u % 16;
                    int ty = (int) v % 16;

                    // Ensure texture coordinates are positive
                    if (tx < 0) tx += 16;
                    if (ty < 0) ty += 16;

                    // Draw the pixel with texture color
                    set_pixel(x, y, sprite[tx][ty]);
                }
            }
        }
    }
}
