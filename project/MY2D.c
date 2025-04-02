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

void draw_circle_v0(int cx, int cy, int r, int color[3])
{
    // draw filled circle with color
    draw_fill_circle(cx, cy, r, color);

    int bgc[] = {0, 0, 0}; // background color
    // draw filled circle with background color
    draw_fill_circle(cx, cy, r - 1, bgc);
}

void draw_circle_v1(int cx, int cy, int r, int color[3])
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

void draw_line_naive(int x1, int y1, int x2, int y2, int color[3])
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

void draw_line_midpoint(int x1, int y1, int x2, int y2, int color[3]){
    int steep = abs(y2 - y1) > abs(x2 - x1);
    if (steep) {
        int temp;
        // swap x and y for both endpoints
        temp = x1; x1 = y1; y1 = temp;
        temp = x2; x2 = y2; y2 = temp;
    }
    if (x1 > x2) {
        int temp;
        // swap endpoints to ensure left-to-right drawing
        temp = x1; x1 = x2; x2 = temp;
        temp = y1; y1 = y2; y2 = temp;
    }
    int dx = x2 - x1;
    int dy = abs(y2 - y1);
    int error = dx / 2;
    int ystep = (y1 < y2) ? 1 : -1;
    int y = y1;
    
    for (int x = x1; x <= x2; x++){
        if(steep) {
            set_pixel(y, x, color);  // swapped coordinates
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

void draw_triangle_v0(int x1, int y1, int x2, int y2, int color[3])
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


void circle_points(int xc, int yc, int x, int y, int color[3]){
    set_pixel(xc+x, yc+y, color);
    set_pixel(xc-x, yc+y, color);
    set_pixel(xc+x, yc-y, color);
    set_pixel(xc-x, yc-y, color);
    set_pixel(xc+y, yc+x, color);
    set_pixel(xc-y, yc+x, color);
    set_pixel(xc+y, yc-x, color);
    set_pixel(xc-y, yc-x, color);
}

void draw_circle_midpoint_v0(int xc, int yc, int r, int color[3]){
    int x = 0, y = r;
    float d = 1.25f - r;
    while(x <= y) {
        circle_points(xc, yc, x, y, color);
        if(d < 0) {
            d += 2 * x + 3;
        } else {
            d += 2 * (x - y) + 5;
            y--;
        }
        x++;
    }
}

void draw_circle_midpoint_v1(int xc, int yc, int r, int color[3]){

    int y = r, x = 0;
        
    // Printing the initial point on the axes 
    // after translation
    circle_points(xc, yc, x, y, color);
        
    // Initialising the value of P
    int P = 1 - r;
    while (y > x)
    { 
        x++;
            
        // Mid-point is inside or on the perimeter
        if (P <= 0)
            P = P + 2*x + 1;
                
        // Mid-point is outside the perimeter
        else
        {
            y--;
            P = P + 2*(x - y) + 1;
        }
            
        // All the perimeter points have already been printed
        if (y < x)
            break;
            
        circle_points(xc, yc, x, y, color);

    } 

}

void draw_circle_midpoint_v2(int xc, int yc, int r, int color[3]){
    int x = 0, y = r;
    int d = 3 - 2 * r;
    circle_points(xc, yc, x, y, color);
    while (y >= x){
      
        if (d > 0) {
            y--; 
            d = d + 4 * (x - y) + 10;
        }
        else
            d = d + 4 * x + 6;

        x++;
        
        circle_points(xc, yc, x, y, color);
    }
}

void draw_ellipse_midpoint(int cx, int cy, int a, int b, int color[3]){
    int x = 0, y = b;
    float a2 = (float)(a * a);
    float b2 = (float)(b * b);
    float d1 = b2 - a2 * b + 0.25f * a2;
    float dx = 2 * b2 * x;
    float dy = 2 * a2 * y;
    
    // Region 1: slope > -1
    while(dx < dy){
        set_pixel(cx + x, cy + y, color);
        set_pixel(cx - x, cy + y, color);
        set_pixel(cx + x, cy - y, color);
        set_pixel(cx - x, cy - y, color);
        
        if(d1 < 0){
            x++;
            dx = 2 * b2 * x;
            d1 += dx + b2;
        } else {
            x++;
            y--;
            dx = 2 * b2 * x;
            dy = 2 * a2 * y;
            d1 += dx - dy + b2;
        }
    }
    
    float d2 = b2 * (x + 0.5f) * (x + 0.5f) + a2 * (y - 1) * (y - 1) - a2 * b2;
    // Region 2: slope <= -1
    while(y >= 0){
        set_pixel(cx + x, cy + y, color);
        set_pixel(cx - x, cy + y, color);
        set_pixel(cx + x, cy - y, color);
        set_pixel(cx - x, cy - y, color);
        
        if(d2 > 0){
            y--;
            dy = 2 * a2 * y;
            d2 += -dy + a2;
        } else {
            x++;
            y--;
            dx = 2 * b2 * x;
            dy = 2 * a2 * y;
            d2 += dx - dy + a2;
        }
    }
}


void draw_polygon_line(int cords[][2], int n, int color[3]){
    for (int i = 0; i < n; i++)
    {
        int x1 = cords[i][0];
        int y1 = cords[i][1];
        int x2 = cords[(i + 1) % n][0];
        int y2 = cords[(i + 1) % n][1];
        draw_line(x1, y1, x2, y2, color);
    }
}


// Modified gen_star_coords using gen_polygon_coords and star reordering logic
void gen_star_coords(int n, int r, int cx, int cy, int cords[][2]){
    int polygon[n][2];
    
    // Generate regular polygon coordinates using outer radius 'r'
    gen_polygon_coords(n, r, cx, cy, polygon);
    
    // Calculate k for star reordering (same as in draw_star_line)
    int k = (n % 2 == 0) ? (n / 2 + 1) : n / 2;
    for (int i = 0; i < n; i++){
        int index = (i * k) % n;
        cords[i][0] = polygon[index][0];
        cords[i][1] = polygon[index][1];
    }
}

// New function that only generates coordinates without drawing
void gen_polygon_coords(int n, int r, int cx, int cy, int cords[][2]){
    for (int i = 0; i < n; i++)
    {
        cords[i][0] = cx + r * cos(2 * PI * i / n);
        cords[i][1] = cy + r * sin(2 * PI * i / n);
    }
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
            if (y1 != y2) { // Avoid division by zero
                int x = x1 + (x2 - x1) * (y - y1) / (y2 - y1);
                
                // Ensure we don't exceed the array bounds
                if (isect->count < MAX_INTERSECT) {
                    isect->x[isect->count++] = x;
                }
            }
        }
    }
}

void bubble_sort(Intersections *isect) {
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

void draw_fill_polygon(int cords[][2], int n, int color[3]) {
    // Find min and max y-coordinates to determine the scan line range
    int min_y = cords[0][1];
    int max_y = cords[0][1];
    
    for (int i = 1; i < n; i++) {
        if (cords[i][1] < min_y) min_y = cords[i][1];
        if (cords[i][1] > max_y) max_y = cords[i][1];
    }
    
    // Scan line algorithm
    Intersections isect;
    for (int y = min_y; y <= max_y; y++) {
        // Find intersections
        intersect(cords, n, y, &isect);
        
        // Sort intersections
        bubble_sort(&isect);
        
        // Draw horizontal lines between pairs of intersections
        for (int i = 0; i < isect.count; i += 2) {
            if (i + 1 < isect.count) {
                draw_line(isect.x[i], y, isect.x[i + 1], y, color);
            }
        }
    }
    
    // Optionally draw the outline of the polygon
    draw_polygon_line(cords, n, color);
}
