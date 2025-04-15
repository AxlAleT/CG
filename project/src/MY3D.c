#include "MY3D.h"

//viewport settings
#define SEMI_WIDTH (WIDTH >> 1)
#define SEMI_HEIGHT (HEIGHT >> 1)
#define VIEW_SCALE 30

//projection params
float D = -5.0;
float d = -4.0;

int n = 5;
int m = 5;

// Define cube vertices (2x2x2 cube centered at origin)
float cube_verts[8][3] = {
    {-1, -1, -1}, // 0: left-bottom-back
    {-1, -1, 1}, // 1: left-bottom-front
    {-1, 1, -1}, // 2: left-top-back
    {-1, 1, 1}, // 3: left-top-front
    {1, -1, -1}, // 4: right-bottom-back
    {1, -1, 1}, // 5: right-bottom-front
    {1, 1, -1}, // 6: right-top-back
    {1, 1, 1} // 7: right-top-front
};

// Define cube faces (6 faces, each with 4 vertices)
int cube_faces[6][4] = {
    {0, 1, 3, 2}, // Left face
    {4, 6, 7, 5}, // Right face
    {0, 4, 5, 1}, // Bottom face
    {2, 3, 7, 6}, // Top face
    {0, 2, 6, 4}, // Back face
    {1, 5, 7, 3} // Front face
};

void draw_vert(int i, int j, int c[3]) {
    set_pixel(i, j, c);
}

void draw_verts(float tx, float ty, float tz, float esc, float v[][3], int c[3], int n) {
    for (int i = 0; i < n; i++) {
        float x = v[i][0] * esc;
        float y = v[i][1] * esc;
        float z = v[i][2] * esc;

        int screen_x, screen_y;
        transform_vert(&x, &y, &z, tx, ty, tz, 0, &screen_x, &screen_y);

        // Only draw if vertex is in front of the camera (z < -1)
        if (z < -1) {
            draw_vert(screen_x, screen_y, c);
        }
    }
}

void transform_vert(float *x, float *y, float *z, float tx, float ty, float tz, float ang, int *screen_x,
                    int *screen_y) {
    // Initialize x2, y2, z2 with input values (this was missing)
    float x2 = *x;
    float y2 = *y;
    float z2 = *z;

    // Rotation around Z axis (uncomment if needed)

    float x_temp_z = x2;
    x2 = x2 * cos(ang) - y2 * sin(ang);
    y2 = x_temp_z * sin(ang) + y2 * cos(ang);


    // Rotate around Y axis
    float x_temp = x2;
    x2 = x2 * cos(ang) - z2 * sin(ang);
    z2 = x_temp * sin(ang) + z2 * cos(ang);

    // Rotate around X axis
    float y_temp = y2;
    y2 = y2 * cos(ang) - z2 * sin(ang);
    z2 = y_temp * sin(ang) + z2 * cos(ang);

    // Apply translation
    *x = x2 + tx;
    *y = y2 + ty;
    *z = z2 + tz;

    // Apply projection if vertex is in front of camera
    if (*z < -1) {
        *screen_x = SEMI_WIDTH + VIEW_SCALE * d * (*x) / (*z);
        *screen_y = SEMI_HEIGHT + VIEW_SCALE * d * (*y) / (*z);
    } else {
        // Set values outside the visible area if vertex is behind camera
        *screen_x = -1;
        *screen_y = -1;
    }
}

void draw_faces(float v[][3], int f[][4], int c[3], int n) {
    for (int i = 0; i < n; i++) {
        int quad[4][2];
        int all_visible = 1; // Flag to check if all vertices are visible
        
        // First check if all vertices of this face are visible
        for (int j = 0; j < 4; j++) {
            float x = v[f[i][j]][0];
            float y = v[f[i][j]][1];
            float z = v[f[i][j]][2];
            
            if (z >= -1) {
                all_visible = 0; // At least one vertex is not visible
                break;
            }
        }
        
        // Only process faces where all vertices are visible
        if (all_visible) {
            for (int j = 0; j < 4; j++) {
                float x = v[f[i][j]][0];
                float y = v[f[i][j]][1];
                float z = v[f[i][j]][2];
                
                // Since we already checked z < -1, this is safe
                quad[j][0] = SEMI_WIDTH + VIEW_SCALE * d * x / z;
                quad[j][1] = SEMI_HEIGHT + VIEW_SCALE * d * y / z;
            }
            draw_poly(quad, 4, c);
        }
    }
}

// Main function to draw a 3D cube
void draw_cube(float tx, float ty, float tz, float scale, float angle_degrees, int vert_color[3], int face_color[3]) {
    // Convert degrees to radians
    float angle_radians = angle_degrees * (M_PI / 180.0);
    
    // Create temporary vertices array for transformations
    float transformed_verts[8][3];

    // Copy original vertices and apply scaling
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 3; j++) {
            transformed_verts[i][j] = cube_verts[i][j] * scale;
        }
    }

    // Apply rotations to each vertex
    for (int i = 0; i < 8; i++) {
        float x = transformed_verts[i][0];
        float y = transformed_verts[i][1];
        float z = transformed_verts[i][2];

        // Rotate around Y axis
        float x_temp = x;
        x = x * cos(angle_radians) - z * sin(angle_radians);
        z = x_temp * sin(angle_radians) + z * cos(angle_radians);

        // Rotate around X axis
        float y_temp = y;
        y = y * cos(angle_radians) - z * sin(angle_radians);
        z = y_temp * sin(angle_radians) + z * cos(angle_radians);

        // Store rotated coordinates
        transformed_verts[i][0] = x;
        transformed_verts[i][1] = y;
        transformed_verts[i][2] = z;
    }

    // Apply translation
    for (int i = 0; i < 8; i++) {
        transformed_verts[i][0] += tx;
        transformed_verts[i][1] += ty;
        transformed_verts[i][2] += tz;

    }

    // Draw faces
    draw_faces(transformed_verts, cube_faces, face_color, 6);
    
    // Draw vertices for debugging
    for (int i = 0; i < 8; i++) {
        float x = transformed_verts[i][0];
        float y = transformed_verts[i][1];
        float z = transformed_verts[i][2];
        
        // Only draw vertices in front of the camera
        if (z < -1) {
            int screen_x = SEMI_WIDTH + VIEW_SCALE * d * x / z;
            int screen_y = SEMI_HEIGHT + VIEW_SCALE * d * y / z;
            draw_vert(screen_x, screen_y, vert_color);
        }
    }
}

void create_grid(int n, int m, float v[][3], int e[][2]) {
    int num_vertices = n * m;
    int num_edges = (n-1)*m + n*(m-1);  // Horizontal + vertical edges

    // Create vertices
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            int idx = j * n + i;
            v[idx][0] = i - (n-1)/2.0;  // Center the grid
            v[idx][1] = j - (m-1)/2.0;
            v[idx][2] = 0;              // Z=0 plane
        }
    }

    int k = 0;
    // Horizontal edges
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n-1; i++) {
            int idx = j * n + i;
            e[k][0] = idx;
            e[k][1] = idx + 1;
            k++;
        }
    }

    // Vertical edges
    for (int j = 0; j < m-1; j++) {
        for (int i = 0; i < n; i++) {
            int idx = j * n + i;
            e[k][0] = idx;
            e[k][1] = idx + n;
            k++;
        }
    }
}

void draw_edges(float v[][3], int e[][2], int c[3], int n) {
    for (int i = 0; i < n; i++) {
        int idx1 = e[i][0];
        int idx2 = e[i][1];

        float x1 = v[idx1][0];
        float y1 = v[idx1][1];
        float z1 = v[idx1][2];

        float x2 = v[idx2][0];
        float y2 = v[idx2][1];
        float z2 = v[idx2][2];

        // Only draw if both vertices are in front of camera
        if (z1 < -1 && z2 < -1) {
            int screen_x1 = SEMI_WIDTH + VIEW_SCALE * d * x1 / z1;
            int screen_y1 = SEMI_HEIGHT + VIEW_SCALE * d * y1 / z1;

            int screen_x2 = SEMI_WIDTH + VIEW_SCALE * d * x2 / z2;
            int screen_y2 = SEMI_HEIGHT + VIEW_SCALE * d * y2 / z2;

            draw_line(screen_x1, screen_y1, screen_x2, screen_y2, c);
        }
    }
}

void draw_3d_grid(int n, int m, float tx, float ty, float tz, float scale, float angle_degrees, int color[3]) {
    // Calculate how many vertices and edges we need
    int num_vertices = n * m;
    int num_edges = (n-1)*m + n*(m-1);  // Horizontal + vertical edges

    // Allocate memory for vertices and edges
    float (*grid_verts)[3] = malloc(num_vertices * sizeof(float[3]));
    int (*grid_edges)[2] = malloc(num_edges * sizeof(int[2]));

    // Generate the grid
    create_grid(n, m, grid_verts, grid_edges);

    // Convert degrees to radians
    float angle_radians = angle_degrees * (M_PI / 180.0);

    // Apply scale, rotation, and translation to vertices
    for (int i = 0; i < num_vertices; i++) {
        // Scale
        float x = grid_verts[i][0] * scale;
        float y = grid_verts[i][1] * scale;
        float z = grid_verts[i][2] * scale;

        // Rotate around Y axis
        float x_temp = x;
        x = x * cos(angle_radians) - z * sin(angle_radians);
        z = x_temp * sin(angle_radians) + z * cos(angle_radians);

        // Rotate around X axis
        float y_temp = y;
        y = y * cos(angle_radians) - z * sin(angle_radians);
        z = y_temp * sin(angle_radians) + z * cos(angle_radians);

        // Store rotated and scaled coordinates
        grid_verts[i][0] = x + tx;
        grid_verts[i][1] = y + ty;
        grid_verts[i][2] = z + tz;
    }

    // Draw the edges
    draw_edges(grid_verts, grid_edges, color, num_edges);

    // Free allocated memory
    free(grid_verts);
    free(grid_edges);
}