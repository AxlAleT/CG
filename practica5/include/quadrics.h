#ifndef QUADRICS_H
#define QUADRICS_H

#include "utils.h"

// 4x4 matrix structure for quadric representation
typedef struct {
    float m[4][4]; // Row-major order
} mat4x4;

// Quadric surface structure
typedef struct {
    mat4x4 Q;       // Coefficient matrix
    material mat;   // Material properties
} quadric;

// Matrix operations
void identity_matrix(mat4x4 *m);
void zero_matrix(mat4x4 *m);
void multiply_vec4(mat4x4 m, float v[4], float result[4]);
float quadric_formula(quadric q, vec3 p);
void quadric_normal(quadric q, vec3 p, vec3 *n);

// Quadric type creation functions
void create_sphere(quadric *q, vec3 center, float radius, material mat);
void create_ellipsoid(quadric *q, vec3 center, float a, float b, float c, material mat);
void create_cylinder(quadric *q, vec3 center, float radius, char axis, material mat);
void create_cone(quadric *q, vec3 center, float a, float b, float c, material mat);
void create_hyperboloid_one_sheet(quadric *q, vec3 center, float a, float b, float c, material mat);
void create_hyperboloid_two_sheet(quadric *q, vec3 center, float a, float b, float c, material mat);
void create_elliptic_paraboloid(quadric *q, vec3 center, float a, float b, material mat);
void create_hyperbolic_paraboloid(quadric *q, vec3 center, float a, float b, material mat);

// Ray-quadric intersection
int hit_quadric(quadric q, ray r, float *t);
int hit_quadrics(quadric q[], int num_quad, ray r, float *t, vec3 *n, material **mat);

#endif // QUADRICS_H
