#include <stdio.h>
#include <math.h>
#include "../include/utils.h"
#include "../include/quadrics.h"

int main() {
    // Simple camera setup looking down slightly
    cam c = {15.0, 0.0, 0, 20};  // D, d, theta, phi - camera at z=16, looking down at 20°
    
    // Light source from above and to the left
    vec3 luz = {-4.0, 6.0, 4.0};
    make_unit_vec(&luz);

    int col[3];
    ray r;
    r.ori.x = 0.0;
    r.ori.y = 0.0;
    r.ori.z = c.D;

    // Define materials with distinct colors
    material green_mat = {0, {100, 200, 100}};     // Green
    material blue_mat = {0, {100, 200, 255}};      // Blue
    material orange_mat = {0, {255, 150, 0}};      // Orange
    material lime_mat = {0, {150, 255, 50}};       // Lime
    material red_mat = {0, {255, 50, 50}};         // Red
    material purple_mat = {0, {180, 100, 250}};    // Purple
    material yellow_mat = {0, {255, 255, 50}};     // Yellow
    material cyan_mat = {0, {0, 255, 255}};        // Cyan

    // Define positions for each quadric
    vec3 ground_center = {0.0, -10.0, 0.0};        // Ground plane
    vec3 sphere_center = {-6.0, 0.0, 0.0};         // Sphere
    vec3 ellipsoid_center = {0.0, 0.0, 0.0};      // Ellipsoid (position in view)
    vec3 cylinder_center = {0.0, 0.0, 0.0};        // Cylinder
    vec3 cone_center = {0.0, 0.0, 1.0};            // Cone
    vec3 hyperboloid1_center = {6.0, 0.0, 0.0};    // One-sheet hyperboloid
    vec3 hyperboloid2_center = {-4.5, 0.0, -6.0};  // Two-sheet hyperboloid
    vec3 paraboloid1_center = {0.0, 0.0, -6.0};    // Elliptic paraboloid
    vec3 paraboloid2_center = {4.5, 0.0, -6.0};    // Hyperbolic paraboloid

    // Create array of all quadrics (only sphere initialized for testing)
    int num_quad = 1;
    quadric q[1];
    
    // Create all quadric surfaces defined in the readme - with reduced sizes
    //create_sphere(&q[0], sphere_center, 0.75, blue_mat);                   // Sphere (reduced from 1.5)
    //create_ellipsoid(&q[0], ellipsoid_center, 0.6, 0.3, 0.4, red_mat);    // Ellipsoid (reduced from 1.0, 1.5, 0.8)
    //create_cylinder(&q[0], cylinder_center, 0.6, 'y', green_mat);          // Cylinder (reduced from 1.2)
    create_cone(&q[0], cone_center, 0.5, 0.5, 0.75, orange_mat);           // Cone (reduced from 1.0, 1.0, 1.5)
    //create_hyperboloid_one_sheet(&q[4], hyperboloid1_center, 0.4, 0.4, 0.6, purple_mat); // One-sheet hyperboloid (reduced from 0.8, 0.8, 1.2)
    //create_hyperboloid_two_sheet(&q[5], hyperboloid2_center, 0.4, 0.4, 0.5, lime_mat);   // Two-sheet hyperboloid (reduced from 0.8, 0.8, 1.0)
    //create_elliptic_paraboloid(&q[6], paraboloid1_center, 0.5, 0.5, yellow_mat);         // Elliptic paraboloid (reduced from 1.0, 1.0)
    //create_hyperbolic_paraboloid(&q[7], paraboloid2_center, 0.5, 0.5, cyan_mat);         // Hyperbolic paraboloid (reduced from 1.0, 1.0)
    //create_sphere(&q[8], ground_center, 9.0, green_mat);                   // Ground sphere (kept the same size)

    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            // Calculate ray direction with proper aspect ratio and field of view
            float aspect = (float)WIDTH / (float)HEIGHT;
            float fov = 45.0f * PI / 180.0f; // 45-degree field of view
            
            // Normalized pixel coordinates (from -1 to 1)
            float x = (2.0f * ((float)i + 0.5f) / (float)WIDTH - 1.0f) * aspect * tan(fov/2.0f);
            float y = (1.0f - 2.0f * ((float)j + 0.5f) / (float)HEIGHT) * tan(fov/2.0f);
            
            // Ray direction
            r.dir.x = x;
            r.dir.y = y;
            r.dir.z = -1.0f; // Looking down the negative z-axis
            
            // Normalize the direction
            make_unit_vec(&r.dir);
            
            // Apply camera rotations if needed
            ray r_aux = r; // Make a copy before rotation
            
            if (fabs(c.phi) > EPSILON) {
                rotateX(&(r_aux.dir), c.phi);
            }
            
            if (fabs(c.theta) > EPSILON) {
                rotateY(&(r_aux.dir), c.theta);
            }
            
            float t;
            vec3 n;
            material *mat;
            
            // Ray-quadric intersection test
            if (hit_quadrics(q, num_quad, r_aux, &t, &n, &mat)) {
                // Calculate hit point for debugging
                vec3 hitPoint;
                shoot_ray(r_aux, t, &hitPoint);
                
                // Get color using Phong shading
                if (make_unit_vec(&n)) {
                    shade_phong(r_aux, n, luz, mat->col, col);
                } else {
                    // Fallback for degenerate normals
                    col[0] = 255; col[1] = 0; col[2] = 0;
                }
            } else {
                // Simple gradient background
                float t = 0.5f * (r.dir.y + 1.0f);
                col[0] = (int)(255.99f * ((1.0f - t) * 1.0f + t * 0.5f));
                col[1] = (int)(255.99f * ((1.0f - t) * 1.0f + t * 0.7f));
                col[2] = (int)(255.99f * ((1.0f - t) * 1.0f + t * 1.0f));
            }
            
            set_pixel(i, j, col);
        }
    }

    save_pixels();
    return 0;
}
