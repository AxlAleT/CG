#include "../include/utils.h"

// Global variables for image saving
static unsigned char *image_data = NULL;

// Initialize the image data buffer
void initialize_image_buffer() {
    if (image_data == NULL) {
        image_data = (unsigned char *)malloc(WIDTH * HEIGHT * 3 * sizeof(unsigned char));
        if (image_data == NULL) {
            fprintf(stderr, "Failed to allocate memory for image buffer\n");
            exit(1);
        }
    }
}

// Set pixel color in the image buffer
void set_pixel(int x, int y, int color[3]) {
    if (image_data == NULL) {
        initialize_image_buffer();
    }
    
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT) {
        int idx = (y * WIDTH + x) * 3;
        image_data[idx] = color[0];
        image_data[idx + 1] = color[1];
        image_data[idx + 2] = color[2];
    }
}

// Save the image buffer to a PPM file
int save_pixels() {
    if (image_data == NULL) {
        fprintf(stderr, "No image data to save\n");
        return 0;
    }
    
    FILE *fp = fopen("raytracer_output.ppm", "wb");
    if (!fp) {
        fprintf(stderr, "Error opening file raytracer_output.ppm for writing\n");
        return 0;
    }
    
    // Write PPM header
    fprintf(fp, "P6\n%d %d\n255\n", WIDTH, HEIGHT);
    
    // Write pixel data
    fwrite(image_data, 1, WIDTH * HEIGHT * 3, fp);
    
    fclose(fp);
    printf("Image saved to raytracer_output.ppm\n");
    
    // Free memory
    free(image_data);
    image_data = NULL;
    
    return 1;
}

// Vector math implementations
void shoot_ray(ray r, float t, vec3 *p) {
    p->x = r.ori.x + t * r.dir.x;
    p->y = r.ori.y + t * r.dir.y;
    p->z = r.ori.z + t * r.dir.z;
}

float dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

void cross(vec3 a, vec3 b, vec3 *c) {
    c->x = a.y * b.z - a.z * b.y;
    c->y = a.z * b.x - a.x * b.z;
    c->z = a.x * b.y - a.y * b.x;
}

float mag(vec3 v) {
    return sqrt(dot(v, v));
}

int unit_vec(vec3 v, vec3 *u) {
    float m = mag(v);
    if (m < EPSILON) return 0;
    u->x = v.x / m;
    u->y = v.y / m;
    u->z = v.z / m;
    return 1;
}

int make_unit_vec(vec3 *v) {
    float m = mag(*v);
    if (m < EPSILON) return 0;
    v->x /= m;
    v->y /= m;
    v->z /= m;
    return 1;
}

void add(vec3 a, vec3 b, vec3 *c) {
    c->x = a.x + b.x;
    c->y = a.y + b.y;
    c->z = a.z + b.z;
}

void sub(vec3 a, vec3 b, vec3 *c) {
    c->x = a.x - b.x;
    c->y = a.y - b.y;
    c->z = a.z - b.z;
}

void porK(float k, vec3 *v) {
    v->x *= k;
    v->y *= k;
    v->z *= k;
}

void rotateX(vec3 *v, float ang) {
    ang *= PI/180.0;

    float x = v->x;
    float y = v->y*cos(ang) - v->z*sin(ang);
    float z = v->y*sin(ang) + v->z*cos(ang);

    v->x = x; v->y = y; v->z = z;
}

void rotateY(vec3 *v, float ang) {
    ang *= PI/180.0;
    float y = v->y;
    float z = v->z*cos(ang) - v->x*sin(ang);
    float x = v->z*sin(ang) + v->x*cos(ang);
    
    v->x = x; v->y = y; v->z = z;
}

void rotateZ(vec3 *v, float ang) {
    ang *= PI/180.0;
    float z = v->z;
    float x = v->x*cos(ang) - v->y*sin(ang);
    float y = v->x*sin(ang) + v->y*cos(ang);
    
    v->x = x; v->y = y; v->z = z;
}

void bg_color(ray r, int c[3]) {
    // t in [0,1] :- dir.y in [-0.5, 0.5]
    float t = r.dir.y + 0.5;
    c[0] = (int)(255.99*((1.0-t) + t*0.5));
    c[1] = (int)(255.99*((1.0-t) + t*0.7));
    c[2] = (int)(255.99*((1.0-t) + t*1.0));
}

int hit_sphere(sphere s, ray r, float *t) {
    // u <-- origin - center
    vec3 u;
    sub(r.ori, s.center, &u);

    // a <-- dir . dir
    float a = dot(r.dir, r.dir);
    
    // b <-- 2dir . u
    float b = 2.0 * dot(r.dir, u);
    
    // c <-- u . u - r^2
    float c = dot(u, u) - s.radius * s.radius;
    
    // discr <-- b^2 - 4ac 
    float discr = b*b - 4.0*a*c;

    if (discr < 0) return 0;
    if (discr == 0) {
        *t = -b/(2.0*a);
        if (*t < EPSILON) return 0;
        return 1;
    }
    float aux = sqrt(discr);
    *t = (-b-aux)/(2.0*a);
    if (*t < EPSILON) {
        *t = (-b+aux)/(2.0*a);
        if (*t < EPSILON) return 0;
    }
    return 1;
}

int hit_spheres(sphere s[], int num_sph, ray r, float *t, vec3 *n, material **mat) {
    int hits = 0;
    vec3 hitP;
    float minT = 1000000.0;
    for (int i = 0; i < num_sph; i++) {
        float currentT;
        if (hit_sphere(s[i], r, &currentT)) {
            hits++;
            if (currentT < minT) {
                minT = currentT;
                shoot_ray(r, currentT, &hitP);
                sub(hitP, s[i].center, n);
                *mat = &(s[i].mat);
            }             
        }
    }
    *t = minT;
    return hits;
}

float shade(vec3 n, vec3 luz, float col_obj) {
    float kamb = 0.1, Iamb = 1.0;
    float kdiff = 0.9, Iluz = 1.0;

    float out = kamb * Iamb;
    float cos_ang = dot(n, luz);
    if (cos_ang > EPSILON) {
        out += kdiff * Iluz * cos_ang;
    }
    return out*col_obj;
}

void shade_phong(ray r, vec3 n, vec3 luz, int col_obj[3], int col[3]) {
    float kamb = 0.1, Iamb = 1.0;
    float kdiff = 0.9, Iluz = 1.0;
    float kspec = 0.3;
    int exp = 16;
    
    //ambient
    float out = kamb * Iamb;
    
    //diffuse
    float cos_ang = dot(n, luz);
    if (cos_ang > EPSILON) {
        out += kdiff * Iluz * cos_ang; //falta atenuacion
    }

    //specular
    float spec = 0.0;
    //calcular R
    vec3 R = n;
    porK(cos_ang*2.0, &R);    
    sub(R, luz, &R);
    
    //calcular cos_phi
    vec3 neg_dir = r.dir;
    porK(-1.0, &neg_dir);
    if(make_unit_vec(&neg_dir)) {
        float cos_phi = dot(neg_dir, R);
        if(cos_phi > 0.0) {
            //calcular specular component
            spec = kspec * pow(cos_phi, exp);
        }
    }
    
    col[0] = (int)(out*col_obj[0] + 255.99*spec);
    if(col[0] > 255) col[0] = 255;

    col[1] = (int)(out*col_obj[1] + 255.99*spec);
    if(col[1] > 255) col[1] = 255;

    col[2] = (int)(out*col_obj[2] + 255.99*spec);
    if(col[2] > 255) col[2] = 255;
}
