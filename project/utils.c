#include "utils.h"

// my image
int pixels[WIDTH][HEIGHT][3];

// sprite
int sprite[16][16][3];

char *sprite_src_name = "marioRCP.txt";
char *output_file_name = "out.ppm";

// colors
int skin_color[3] = {247, 164, 34};
int red_color[3] = {255, 23, 3};
int brown_color[3] = {133, 75, 12};

void set_pixel(int i, int j, int color[3]) {
    if (i < 0 || j < 0 || i >= WIDTH || j >= HEIGHT)
        return;
    pixels[i][j][0] = color[0];
    pixels[i][j][1] = color[1];
    pixels[i][j][2] = color[2];
}

void set_pixel_sprite(int i, int j, int color[3]) {
    sprite[i][j][0] = color[0];
    sprite[i][j][1] = color[1];
    sprite[i][j][2] = color[2];
}

int load_sprite() {
    FILE *src = fopen(sprite_src_name, "r");
    if (!src)
        return 0; // error

    char c;
    for (int j = 15; j >= 0; j--) {
        for (int i = 0; i < 16; i++) {
            fscanf(src, "%c", &c);
            switch (c) {
            case 'P':
                set_pixel_sprite(i, j, skin_color);
                break;
            case 'R':
                set_pixel_sprite(i, j, red_color);
                break;
            case 'C':
                set_pixel_sprite(i, j, brown_color);
                break;
            default:
                sprite[i][j][0] =
                    sprite[i][j][1] =
                        sprite[i][j][2] = 0;
                break;
            }
        }
        fscanf(src, "%c", &c);
    }

    fclose(src);
    return 1; // success
}

void show_sprite() {
    for (int j = 0; j < 16; j++) {
        for (int i = 0; i < 16; i++) {
            printf("%d %d %d ", sprite[i][j][0], sprite[i][j][1], sprite[i][j][2]);
        }
        printf("\n");
    }
}

void show_pixels() {
    for (int j = HEIGHT - 1; j >= 0; j--) {
        for (int i = 0; i < WIDTH; i++) {
            printf("( ");
            for (int k = 0; k < 3; k++)
                printf("%d ", pixels[i][j][k]);
            printf(")");
        }
        printf("\n");
    }
}

int save_pixels() {
    FILE *dest = fopen(output_file_name, "w");
    if (!dest)
        return 0; // error

    fprintf(dest, "P3\n");
    fprintf(dest, "%d %d\n", WIDTH, HEIGHT);
    fprintf(dest, "%d\n", 255);

    // RGB values
    for (int j = HEIGHT - 1; j >= 0; j--) {
        for (int i = 0; i < WIDTH; i++)
            for (int k = 0; k < 3; k++)
                fprintf(dest, "%d ", pixels[i][j][k]);
        fprintf(dest, "\n");
    }

    fclose(dest);
    return 1; // success
}

void draw_sprite(int tx, int ty, int sx, int sy, float angle) {
    angle *= PI / 180.0;
    for (int j = 0; j < 16; j++) {
        for (int i = 0; i < 16; i++) {
            int x = sx * (i - 8);
            int y = sy * (j - 8);

            float x2 = x * cos(angle) - y * sin(angle);
            float y2 = x * sin(angle) + y * cos(angle);

            x2 += tx;
            y2 += ty;

            set_pixel((int)x2, (int)y2, sprite[i][j]);
            set_pixel((int)x2 - 1, (int)y2, sprite[i][j]);
            set_pixel((int)x2 + 1, (int)y2, sprite[i][j]);
            set_pixel((int)x2, (int)y2 - 1, sprite[i][j]);
            set_pixel((int)x2, (int)y2 + 1, sprite[i][j]);
        }
    }
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}