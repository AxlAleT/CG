#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846
#define EPSILON 0.0001
#define WIDTH 800
#define HEIGHT 600

// Camera structure
typedef struct {
    float D, d;
    float theta, phi;
} cam;

// 3D Vector structure
typedef struct {
    float x, y, z;
} vec3;

// Ray structure
typedef struct {
    vec3 ori, dir;
} ray; // origin & direction

// Material structure
typedef struct {
    int type;
    // 0: diffuse
    // 1: metal
    // 2: transparent
    int col[3];
} material;

// Sphere structure
typedef struct {
    vec3 center;
    float radius;
    material mat;
} sphere;

// Vector operations
void shoot_ray(ray r, float t, vec3 *p);
float dot(vec3 a, vec3 b);
void cross(vec3 a, vec3 b, vec3 *c);
float mag(vec3 v);
int unit_vec(vec3 v, vec3 *u);
int make_unit_vec(vec3 *v);
void add(vec3 a, vec3 b, vec3 *c);
void sub(vec3 a, vec3 b, vec3 *c);
void porK(float k, vec3 *v);
void rotateX(vec3 *v, float ang);
void rotateY(vec3 *v, float ang);
void rotateZ(vec3 *v, float ang);

// Rendering functions
void bg_color(ray r, int c[3]);
int hit_sphere(sphere s, ray r, float *t);
int hit_spheres(sphere s[], int num_sph, ray r, float *t, vec3 *n, material **mat);
float shade(vec3 n, vec3 luz, float col_obj);
void shade_phong(ray r, vec3 n, vec3 luz, int col_obj[3], int col[3]);

// Image saving
void set_pixel(int x, int y, int color[3]);
int save_pixels();

#endif // UTILS_H
