#include <stdio.h>
#include <math.h>

#include "utils.h"	// Include the header for utils functions
#include "my2d.h"
#include "poly.h"
#include "my3d.h"

//extern camera cam;
//extern transform obj;

extern int mario[16][16][3];

extern int cube_top[16][16][3];
extern int cube_side[16][16][3];
extern int cube_bottom[16][16][3];


void draw_vert(int i, int j, int c[3]) {
    //draw_circle(i, j, 5, c);
    set_pixel(i, j, c);
    set_pixel(i - 1, j, c);
    set_pixel(i + 1, j, c);
    set_pixel(i, j - 1, c);
    set_pixel(i, j + 1, c);
}


// Basic transforms
void scale(float v[3], float esc) {
    v[0] *= esc;
    v[1] *= esc;
    v[2] *= esc;
}

void scale2(float v[3], float escX, float escY, float escZ) {
    v[0] *= escX;
    v[1] *= escY;
    v[2] *= escZ;
}

void translate(float v[3], float x, float y, float z) {
    v[0] += x;
    v[1] += y;
    v[2] += z;
}

void rotateX(float v[3], float ang) {
    float rad = ang * M_PI / 180.0;
    float y = v[1];
    float z = v[2];
    v[1] = y * cos(rad) - z * sin(rad);
    v[2] = y * sin(rad) + z * cos(rad);
}

void rotateY(float v[3], float ang) {
    float rad = ang * M_PI / 180.0;
    float x = v[0];
    float z = v[2];
    v[0] = x * cos(rad) + z * sin(rad);
    v[2] = -x * sin(rad) + z * cos(rad);
}

void rotateZ(float v[3], float ang) {
    float rad = ang * M_PI / 180.0;
    float x = v[0];
    float y = v[1];
    v[0] = x * cos(rad) - y * sin(rad);
    v[1] = x * sin(rad) + y * cos(rad);
}


// find hidden faces

/*
	| i	j	k  |
	| ax	ay	az | =
	| bx	by	bz |

	(ay*bz - az*by)i - (ax*bz - az*bx)j + (ax*by - ay*bx)k
*/
void cross_prod(float a[3], float b[3], float c[3]) {
    c[0] = a[1] * b[2] - a[2] * b[1];
    c[1] = a[2] * b[0] - a[0] * b[2];
    c[2] = a[0] * b[1] - a[1] * b[0];
}

float dot_prod(float a[3], float b[3]) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

int points_towards_cam(mesh *m, int face_idx, camera cam, float n[3]) {
    float ab[3], ad[3], aux[3];

    float *A, *B, *D; // Removed unused variable C
    int *idxs = (int *) &(m->faces[face_idx]);
    A = (float *) &(m->verts[idxs[0]]);
    B = (float *) &(m->verts[idxs[1]]);
    D = (float *) &(m->verts[idxs[3]]);

    ab[0] = B[0] - A[0];
    ab[1] = B[1] - A[1];
    ab[2] = B[2] - A[2];

    ad[0] = D[0] - A[0];
    ad[1] = D[1] - A[1];
    ad[2] = D[2] - A[2];

    // calculate normal
    cross_prod(ab, ad, n);

    aux[0] = n[0];
    aux[1] = n[1];
    aux[2] = n[2];

    // apply camera rotation
    rotateY(aux, cam.angY);
    rotateX(aux, cam.angX);
    rotateZ(aux, cam.angZ);

    // using flat projection
    return (aux[2] > 0.0);
}

// map image to quad (asuming flat projection)
void map_img2quad(int quad[4][2], int sprite[16][16][3]) {
    float ab[2], ad[2], delta = 1.0 / 16.0;

    ab[0] = quad[1][0] - quad[0][0];
    ab[1] = quad[1][1] - quad[0][1];

    ad[0] = quad[3][0] - quad[0][0];
    ad[1] = quad[3][1] - quad[0][1];

    for (int j = 0; j < 16; j++) {
        for (int i = 0; i < 16; i++) {
            float u = i * delta;
            float v = j * delta;
            int x = quad[0][0] + u * ab[0] + v * ad[0];
            int y = quad[0][1] + u * ab[1] + v * ad[1];
            set_pixel(x, y, sprite[j][i]);
        }
    }
}

int calc_m_inv(int quad[4][2], float m_inv[2][2]) {
    float ab[2], ad[2];

    ab[0] = quad[1][0] - quad[0][0];
    ab[1] = quad[1][1] - quad[0][1];

    ad[0] = quad[3][0] - quad[0][0];
    ad[1] = quad[3][1] - quad[0][1];

    float detM = ab[0] * ad[1] - ab[1] * ad[0];
    // check if detM is close to zero
    if (detM <= 0.000001 && detM >= -0.000001) {
        //printf("WARNING: Determinant zero\n");
        return 0;
    }

    // So the inverse of m is:
    m_inv[0][0] = ad[1] / detM;
    m_inv[0][1] = -ad[0] / detM;
    m_inv[1][0] = -ab[1] / detM;
    m_inv[1][1] = ab[0] / detM;

    // now we can transform from 3D coords to sprite (texture) coords
    return 1;
}


// vertices management

void project(float vv[3], int scr[2], camera cam) {
    float v[3];
    v[0] = vv[0];
    v[1] = vv[1];
    v[2] = vv[2];

    // camera rotation
    rotateY(v, cam.angY);
    rotateX(v, cam.angX);
    rotateZ(v, cam.angZ);

    // projection
    float x_scr = v[0];
    float y_scr = v[1];
    if (!cam.is_flat_proj) {
        x_scr = cam.d * x_scr / (v[2] + cam.D);
        y_scr = cam.d * y_scr / (v[2] + cam.D);
    }

    // viewport transform
    scr[0] = SEMI_WIDTH + VIEW_SCALE * x_scr;
    scr[1] = SEMI_HEIGHT + VIEW_SCALE * y_scr;
}

void transform_verts(transform obj, mesh *m) {
    for (int i = 0; i < m->nv; i++) {
        m->verts[i][0] = m->base_verts[i][0];
        m->verts[i][1] = m->base_verts[i][1];
        m->verts[i][2] = m->base_verts[i][2];

        scale(m->verts[i], obj.esc);

        rotateY(m->verts[i], obj.angY);
        rotateX(m->verts[i], obj.angX);
        rotateZ(m->verts[i], obj.angZ);

        translate(m->verts[i], obj.tx, obj.ty, obj.tz);
    }
}

void draw_verts(mesh *m, int c[3], camera cam) {
    for (int i = 0; i < m->nv; i++) {
        if (m->verts[i][2] + cam.D < -0.1) {
            int scr[2];
            project(m->verts[i], scr, cam);
            draw_vert(scr[0], scr[1], c);
        }
    }
}

void draw_edges(mesh *m, camera cam) {
    for (int i = 0; i < m->ne; i++) {
        int idx1 = m->edges[i][0];
        int idx2 = m->edges[i][1];
        if (m->verts[idx1][2] + cam.D < -0.1 &&
            m->verts[idx2][2] + cam.D < -0.1
        ) {
            int scr1[2], scr2[2];
            project(m->verts[idx1], scr1, cam);
            project(m->verts[idx2], scr2, cam);
            draw_line(scr1[0], scr1[1], scr2[0], scr2[1], m->wireframe_color);
        }
    }
}

void draw_faces(mesh *m, int c[3], camera cam, int sprite[16][16][3]) {
    float n[3];
    for (int f = 0; f < m->nf; f++) {
        if (points_towards_cam(m, f, cam, n)) {
            int *idxs = (int *) &(m->faces[f]);
            int quad[4][2];

            // Project the 4 vertices of the face
            for (int i = 0; i < 4; i++) {
                int idx = idxs[i];
                project(m->verts[idx], quad[i], cam);
            }

            if (sprite != NULL) {
                // Use texture mapping
                map_img2quad(quad, sprite);
            } else {
                // Use solid color
                draw_poly(quad, 4, c);
            }
        }
    }
}


// shapes

void create_grid_verts(int n, int m, float v[][3]) {
    for (int j = 0; j < m; j++)
        for (int i = 0; i < n; i++) {
            int idx = j * n + i;
            v[idx][0] = i - n / 2;
            v[idx][1] = j - m / 2;
            v[idx][0] /= n / 2;
            v[idx][1] /= m / 2;
            v[idx][2] = 0.0;
        }
}

void create_grid_edges(int n, int m, int e[][2]) {
    int k = 0;
    int idx;
    for (int j = 0; j < m; j++)
        for (int i = 0; i < n - 1; i++) {
            idx = j * n + i;
            e[k][0] = idx;
            e[k++][1] = idx + 1;
        }
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m - 1; j++) {
            idx = j * n + i;
            e[k][0] = idx;
            e[k++][1] = idx + n;
        }
}

void create_grid_faces(int n, int m, int f[][4]) {
    int k = 0;
    for (int j = 0; j < m - 1; j++) {
        for (int i = 0; i < n - 1; i++) {
            int idx = j * n + i;
            f[k][0] = idx;
            f[k][1] = idx + 1;
            f[k][2] = idx + n + 1;
            f[k++][3] = idx + n;
        }
    }
}

void create_cylinder_verts(int n, int m, float v[][3]) {
    float rho = 1;
    float ang = 0.0;
    float delta = 2.0 * PI / n;
    for (int j = 0; j < m; j++) {
        for (int i = 0; i < n; i++) {
            int idx = j * n + i;
            v[idx][2] = rho * cos(ang);
            v[idx][0] = rho * sin(ang);
            v[idx][1] = (float) j / m - 0.5;
            ang += delta;
        }
    }
}

void create_cone_verts(int n, int m, float v[][3]) {
    float delta = 2.0 * PI / n;
    for (int j = 0; j < m; j++) {
        float rho = 1 - (float) j / m;
        float ang = 0.0;
        for (int i = 0; i < n; i++) {
            int idx = j * n + i;
            v[idx][2] = rho * cos(ang);
            v[idx][0] = rho * sin(ang);
            v[idx][1] = (float) j / m - 0.5;
            ang += delta;
        }
    }
}

void create_sphere_verts(int n, int m, float v[][3]) {
    float delta_theta = 2.0 * PI / n;
    float delta_phi = PI / m;

    for (int j = 0; j < m; j++) {
        float phi = -PI / 2.0 + j * delta_phi;
        float rho = cos(phi);

        for (int i = 0; i < n; i++) {
            float theta = i * delta_theta;
            int idx = j * n + i;
            v[idx][2] = rho * cos(theta);
            v[idx][0] = rho * sin(theta);
            v[idx][1] = sin(phi);
        }
    }
}

void create_torus_verts(int n, int m, float ratio, float v[][3]) {
    float delta = 2 * PI / n;
    float delta2 = 2 * PI / m;

    float r1 = 0.75;
    float r2 = r1 * ratio;

    for (int j = 0; j < m; j++) {
        float phi = j * delta2;
        float cos_phi = cos(phi);
        float sin_phi = sin(phi);
        for (int i = 0; i < n; i++) {
            float theta = i * delta;
            int idx = j * n + i;
            float rho = r1 + r2 * cos_phi;
            v[idx][2] = rho * cos(theta);
            v[idx][0] = rho * sin(theta);
            v[idx][1] = r2 * sin_phi;
        }
    }
}

// Add the interpolation function to handle shape transitions
void interpolate(float curr[][3], float next[][3], float t, float result[][3], int size) {
    float tt = 1.0 - t;
    for (int i = 0; i < size; i++) {
        result[i][0] = tt * curr[i][0] + t * next[i][0];
        result[i][1] = tt * curr[i][1] + t * next[i][1];
        result[i][2] = tt * curr[i][2] + t * next[i][2];
    }
}
