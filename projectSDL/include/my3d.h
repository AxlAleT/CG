#ifndef MY3D_H
#define MY3D_H

// Viewport settings
#define SEMI_WIDTH  (WIDTH>>1)
#define SEMI_HEIGHT (HEIGHT>>1)
#define VIEW_SCALE  200//100

// camera transform
typedef struct {
    float D, d;
    float angX, angY, angZ;
    int is_flat_proj;
} camera;

// object transform
typedef struct {
    float tx, ty, tz;
    float angX, angY, angZ;
    float esc;
} transform;

// Mesh
#define N 30
#define M 15

typedef struct {
    int nv, ne, nf;
    int wireframe_color[3]; // Added wireframe_color field
    float base_verts[M * N][3];
    float verts[M * N][3];
    int edges[2 * M * N - M - N][2];
    int faces[(M - 1) * (N - 1)][4];
} mesh;


// Interpolate figures (morphing)
enum Fig {
    GRID,
    CYLINDER,
    CONE,
    SPHERE,
    TORUS
};


void draw_vert(int i, int j, int c[3]);


// Basic transforms
void scale(float v[3], float esc);

void scale2(float v[3], float escX, float escY, float escZ);

void translate(float v[3], float x, float y, float z);

void rotateX(float v[3], float ang);

void rotateY(float v[3], float ang);

void rotateZ(float v[3], float ang);


void project(float vv[3], int scr[2], camera cam);

void transform_verts(transform obj, mesh *m);

void draw_verts(mesh *m, int c[3], camera cam);

void draw_edges(mesh *m, camera cam);

void draw_faces(mesh *m, int c[3], camera cam, int sprite[16][16][3]);


// shapes

void create_grid_verts(int n, int m, float v[][3]);

void create_grid_edges(int n, int m, int e[][2]);

void create_grid_faces(int n, int m, int f[][4]);

void create_cylinder_verts(int n, int m, float v[][3]);

void create_cone_verts(int n, int m, float v[][3]);

void create_sphere_verts(int n, int m, float v[][3]);

#define TORUS_RATIO 0.5

void create_torus_verts(int n, int m, float ratio, float v[][3]);

void interpolate(float curr[][3], float next[][3], float t, float result[][3], int size);


#endif
