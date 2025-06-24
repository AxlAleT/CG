#include "../include/quadrics.h"

// Initialize identity matrix
void identity_matrix(mat4x4 *m) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m->m[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

// Initialize zero matrix
void zero_matrix(mat4x4 *m) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            m->m[i][j] = 0.0f;
        }
    }
}

// Multiply 4x4 matrix by 4D vector
void multiply_vec4(mat4x4 m, float v[4], float result[4]) {
    for (int i = 0; i < 4; i++) {
        result[i] = 0;
        for (int j = 0; j < 4; j++) {
            result[i] += m.m[i][j] * v[j];
        }
    }
}

// Evaluate quadric formula at point p: p^T * Q * p
float quadric_formula(quadric q, vec3 p) {
    float v[4] = {p.x, p.y, p.z, 1.0f};
    float result = 0.0f;
    
    // Calculate v^T * Q * v
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result += v[i] * q.Q.m[i][j] * v[j];
        }
    }
    
    return result;
}

// Calculate normal at point p on quadric surface: n = ∇f = 2*Q*[x,y,z,1] (first 3 components)
void quadric_normal(quadric q, vec3 p, vec3 *n) {
    float v[4] = {p.x, p.y, p.z, 1.0f};
    float result[4];
    
    // Calculate Q*[x,y,z,1]
    multiply_vec4(q.Q, v, result);
    
    // Normal is the gradient: first 3 components of the result, multiplied by 2
    n->x = result[0];
    n->y = result[1];
    n->z = result[2];
    
    // Make sure the normal is pointing outward
    // For a sphere, the normal points away from center
    // For general quadrics, we need to check if the normal points in the same half-space as the viewing ray
    // This is a simplification, and for some quadrics might need to be inverted
    
    // Normalize the normal vector
    if (!make_unit_vec(n)) {
        // If normal can't be normalized, provide a default
        n->x = 0.0f;
        n->y = 1.0f;
        n->z = 0.0f;
    }
}

// Create a sphere quadric: (x-cx)² + (y-cy)² + (z-cz)² - r² = 0
void create_sphere(quadric *q, vec3 center, float radius, material mat) {
    // Start with identity matrix (represents unit sphere at origin)
    identity_matrix(&q->Q);
    
    // Set diagonal elements for unit sphere
    q->Q.m[0][0] = 1.0f;
    q->Q.m[1][1] = 1.0f;
    q->Q.m[2][2] = 1.0f;
    
    // Translate center (calculate quadratic form for translated sphere)
    q->Q.m[0][3] = -center.x;
    q->Q.m[1][3] = -center.y;
    q->Q.m[2][3] = -center.z;
    q->Q.m[3][0] = -center.x;
    q->Q.m[3][1] = -center.y;
    q->Q.m[3][2] = -center.z;
    q->Q.m[3][3] = center.x*center.x + center.y*center.y + center.z*center.z - radius*radius;
    
    // Set material
    q->mat = mat;
}

// Create an ellipsoid: (x/a)² + (y/b)² + (z/c)² - 1 = 0
void create_ellipsoid(quadric *q, vec3 center, float a, float b, float c, material mat) {
    // Start with zero matrix
    zero_matrix(&q->Q);
    
    // Set diagonal elements for ellipsoid
    q->Q.m[0][0] = 1.0f/(a*a);
    q->Q.m[1][1] = 1.0f/(b*b);
    q->Q.m[2][2] = 1.0f/(c*c);
    
    // Translate center
    q->Q.m[0][3] = -2.0f * center.x / (a*a);
    q->Q.m[1][3] = -2.0f * center.y / (b*b);
    q->Q.m[2][3] = -2.0f * center.z / (c*c);
    q->Q.m[3][0] = -2.0f * center.x / (a*a);
    q->Q.m[3][1] = -2.0f * center.y / (b*b);
    q->Q.m[3][2] = -2.0f * center.z / (c*c);
    
    // Set constant term after translation
    q->Q.m[3][3] = center.x*center.x/(a*a) + center.y*center.y/(b*b) + center.z*center.z/(c*c) - 1.0f;
    
    // Set material
    q->mat = mat;
}

// Create a cylinder aligned with the specified axis
void create_cylinder(quadric *q, vec3 center, float radius, char axis, material mat) {
    // Start with zero matrix
    zero_matrix(&q->Q);
    
    if (axis == 'x') {
        // Cylinder along x-axis: y² + z² - r² = 0
        q->Q.m[1][1] = 1.0f;
        q->Q.m[2][2] = 1.0f;
        q->Q.m[3][3] = -radius*radius;
        
        // Translate center for x-axis cylinder
        q->Q.m[1][3] = -center.y;
        q->Q.m[2][3] = -center.z;
        q->Q.m[3][1] = -center.y;
        q->Q.m[3][2] = -center.z;
        q->Q.m[3][3] += center.y*center.y + center.z*center.z;
    } else if (axis == 'y') {
        // Cylinder along y-axis: x² + z² - r² = 0
        q->Q.m[0][0] = 1.0f;
        q->Q.m[2][2] = 1.0f;
        q->Q.m[3][3] = -radius*radius;
        
        // Translate center for y-axis cylinder
        q->Q.m[0][3] = -center.x;
        q->Q.m[2][3] = -center.z;
        q->Q.m[3][0] = -center.x;
        q->Q.m[3][2] = -center.z;
        q->Q.m[3][3] += center.x*center.x + center.z*center.z;
    } else { // z-axis
        // Cylinder along z-axis: x² + y² - r² = 0
        // This matches the readme: diag(1, 1, 0, -r²)
        q->Q.m[0][0] = 1.0f;
        q->Q.m[1][1] = 1.0f;
        q->Q.m[3][3] = -radius*radius;
        
        // Translate center for z-axis cylinder
        q->Q.m[0][3] = -center.x;
        q->Q.m[1][3] = -center.y;
        q->Q.m[3][0] = -center.x;
        q->Q.m[3][1] = -center.y;
        q->Q.m[3][3] += center.x*center.x + center.y*center.y;
    }
    
    // Set material
    q->mat = mat;
}

// Create a cone
void create_cone(quadric *q, vec3 center, float a, float b, float c, material mat) {
    // Start with zero matrix
    zero_matrix(&q->Q);
    
    // Set diagonal elements for cone
    q->Q.m[0][0] = 1.0f/(a*a);
    q->Q.m[1][1] = 1.0f/(b*b);
    q->Q.m[2][2] = -1.0f/(c*c);
    
    // Translate center
    q->Q.m[0][3] = -2.0f * center.x / (a*a);
    q->Q.m[1][3] = -2.0f * center.y / (b*b);
    q->Q.m[2][3] = 2.0f * center.z / (c*c); // Note: opposite sign due to negative coefficient
    q->Q.m[3][0] = -2.0f * center.x / (a*a);
    q->Q.m[3][1] = -2.0f * center.y / (b*b);
    q->Q.m[3][2] = 2.0f * center.z / (c*c);
    
    // Set constant term after translation
    q->Q.m[3][3] = center.x*center.x/(a*a) + center.y*center.y/(b*b) - center.z*center.z/(c*c);
    
    // Set material
    q->mat = mat;
}

// Create one-sheet hyperboloid: (x/a)² + (y/b)² - (z/c)² - 1 = 0
void create_hyperboloid_one_sheet(quadric *q, vec3 center, float a, float b, float c, material mat) {
    // Start with zero matrix
    zero_matrix(&q->Q);
    
    // Set diagonal elements for one-sheet hyperboloid
    q->Q.m[0][0] = 1.0f/(a*a);
    q->Q.m[1][1] = 1.0f/(b*b);
    q->Q.m[2][2] = -1.0f/(c*c);
    
    // Translate center
    q->Q.m[0][3] = -2.0f * center.x / (a*a);
    q->Q.m[1][3] = -2.0f * center.y / (b*b);
    q->Q.m[2][3] = 2.0f * center.z / (c*c); // Note: opposite sign due to negative coefficient
    q->Q.m[3][0] = -2.0f * center.x / (a*a);
    q->Q.m[3][1] = -2.0f * center.y / (b*b);
    q->Q.m[3][2] = 2.0f * center.z / (c*c);
    
    // Set constant term after translation
    q->Q.m[3][3] = center.x*center.x/(a*a) + center.y*center.y/(b*b) - center.z*center.z/(c*c) - 1.0f;
    
    // Set material
    q->mat = mat;
}

// Create two-sheet hyperboloid: -(x/a)² - (y/b)² + (z/c)² - 1 = 0
void create_hyperboloid_two_sheet(quadric *q, vec3 center, float a, float b, float c, material mat) {
    // Start with zero matrix
    zero_matrix(&q->Q);
    
    // Set diagonal elements for two-sheet hyperboloid
    q->Q.m[0][0] = -1.0f/(a*a);
    q->Q.m[1][1] = -1.0f/(b*b);
    q->Q.m[2][2] = 1.0f/(c*c);
    
    // Translate center
    q->Q.m[0][3] = 2.0f * center.x / (a*a);
    q->Q.m[1][3] = 2.0f * center.y / (b*b);
    q->Q.m[2][3] = -2.0f * center.z / (c*c);
    q->Q.m[3][0] = 2.0f * center.x / (a*a);
    q->Q.m[3][1] = 2.0f * center.y / (b*b);
    q->Q.m[3][2] = -2.0f * center.z / (c*c);
    
    // Set constant term after translation
    q->Q.m[3][3] = -center.x*center.x/(a*a) - center.y*center.y/(b*b) + center.z*center.z/(c*c) - 1.0f;
    
    // Set material
    q->mat = mat;
}

// Create elliptic paraboloid: (x/a)² + (y/b)² - z = 0
void create_elliptic_paraboloid(quadric *q, vec3 center, float a, float b, material mat) {
    // Start with zero matrix
    zero_matrix(&q->Q);
    
    // According to readme: diag(1/a², 1/b², 0, -1) with (0,0,1,-0) in row/col 3
    // Set diagonal elements for elliptic paraboloid
    q->Q.m[0][0] = 1.0f/(a*a);
    q->Q.m[1][1] = 1.0f/(b*b);
    q->Q.m[3][3] = -1.0f;
    
    // Set z term (0,0,1,-0) in row/col 3
    q->Q.m[2][3] = -0.5f;
    q->Q.m[3][2] = -0.5f;
    
    // Translate center
    float transX = -2.0f * center.x / (a*a);
    float transY = -2.0f * center.y / (b*b);
    
    q->Q.m[0][3] += transX;
    q->Q.m[1][3] += transY;
    q->Q.m[3][0] += transX;
    q->Q.m[3][1] += transY;
    
    // Add to constant term to account for center.z
    q->Q.m[3][3] += center.x*center.x/(a*a) + center.y*center.y/(b*b) - center.z;
    
    // Set material
    q->mat = mat;
}

// Create hyperbolic paraboloid: (x/a)² - (y/b)² - z = 0
void create_hyperbolic_paraboloid(quadric *q, vec3 center, float a, float b, material mat) {
    // Start with zero matrix
    zero_matrix(&q->Q);
    
    // According to readme: diag(1/a², -1/b², 0, 0) with (0,0,1,-0) in row/col 3
    // Set diagonal elements for hyperbolic paraboloid
    q->Q.m[0][0] = 1.0f/(a*a);
    q->Q.m[1][1] = -1.0f/(b*b);
    
    // Set z term (0,0,1,-0) in row/col 3
    q->Q.m[2][3] = -0.5f;
    q->Q.m[3][2] = -0.5f;
    
    // Translate center
    float transX = -2.0f * center.x / (a*a);
    float transY = 2.0f * center.y / (b*b);  // Note: opposite sign due to negative coefficient
    
    q->Q.m[0][3] += transX;
    q->Q.m[1][3] += transY;
    q->Q.m[3][0] += transX;
    q->Q.m[3][1] += transY;
    
    // Add to constant term to account for center.z
    q->Q.m[3][3] += center.x*center.x/(a*a) - center.y*center.y/(b*b) - center.z;
    
    // Set material
    q->mat = mat;
}

// Ray-quadric intersection
int hit_quadric(quadric q, ray r, float *t) {
    // Convert ray to parametric form: p(t) = origin + t*direction
    // For quadric: p^T * Q * p = 0
    // Substituting: (origin + t*direction)^T * Q * (origin + t*direction) = 0
    // This expands to: at² + bt + c = 0
    
    vec3 o = r.ori;
    vec3 d = r.dir;
    
    // Make sure direction is normalized
    float d_mag = mag(d);
    if (d_mag < EPSILON) return 0; // Invalid ray
    
    if (fabs(d_mag - 1.0f) > EPSILON) {
        d.x /= d_mag;
        d.y /= d_mag;
        d.z /= d_mag;
    }
    
    float v1[4] = {d.x, d.y, d.z, 0.0f};  // Direction as homogeneous vector
    float v2[4] = {o.x, o.y, o.z, 1.0f};  // Origin as homogeneous vector
    float Qd[4], Qo[4];                    // Q*d and Q*o
    
    // Calculate Q*d and Q*o using the multiply_vec4 function
    multiply_vec4(q.Q, v1, Qd);
    multiply_vec4(q.Q, v2, Qo);
    
    // Calculate coefficients of quadratic equation
    // a = d^T * Q * d
    float a = v1[0]*Qd[0] + v1[1]*Qd[1] + v1[2]*Qd[2] + v1[3]*Qd[3];
    
    // b = 2 * d^T * Q * o
    float b = 2.0f * (v1[0]*Qo[0] + v1[1]*Qo[1] + v1[2]*Qo[2] + v1[3]*Qo[3]);
    
    // c = o^T * Q * o
    float c = v2[0]*Qo[0] + v2[1]*Qo[1] + v2[2]*Qo[2] + v2[3]*Qo[3];
    
    // Solve quadratic equation
    float discriminant = b*b - 4*a*c;
    
    if (discriminant < 0) {
        return 0; // No intersection
    }
    
    if (fabs(a) < EPSILON) {
        // If a is close to zero, the equation is linear
        if (fabs(b) < EPSILON) {
            return 0; // No valid intersection
        }
        *t = -c / b;
        return (*t > EPSILON) ? 1 : 0;
    }
    
    float discr_sqrt = sqrt(discriminant);
    float t1 = (-b - discr_sqrt) / (2*a);
    float t2 = (-b + discr_sqrt) / (2*a);
    
    // Find the closest intersection point in front of the ray
    if (t1 > EPSILON) {
        *t = t1;
        return 1;
    }
    
    if (t2 > EPSILON) {
        *t = t2;
        return 1;
    }
    
    return 0; // No valid intersection
}

// Intersection test with multiple quadrics
int hit_quadrics(quadric q[], int num_quad, ray r, float *t, vec3 *n, material **mat) {
    int hits = 0;
    vec3 hitP;
    float minT = 1000000.0;
    
    for (int i = 0; i < num_quad; i++) {
        float currentT;
        if (hit_quadric(q[i], r, &currentT)) {
            hits++;
            if (currentT < minT) {
                minT = currentT;
                
                // Calculate hit point
                shoot_ray(r, currentT, &hitP);
                
                // Calculate normal at hit point
                quadric_normal(q[i], hitP, n);
                
                // Get material
                *mat = &(q[i].mat);
            }             
        }
    }
    
    *t = minT;
    return hits;
}
