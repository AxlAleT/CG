#include "utils.h"

// my image
int pixel[WIDTH][HEIGHT][3];

// sprite

int mario[16][16][3];

char *src_name = "marioRCP.txt";
char *file_name = "out.ppm";

// colors
int piel[3] = {247, 164, 34};
int rojo[3] = {255, 23, 3};
int cafe[3] = {133, 75, 12};

void set_pixel(int i, int j, int c[3])
{
    if (i < 0 || j < 0 || i >= WIDTH || j >= HEIGHT)
        return;
    pixel[i][j][0] = c[0];
    pixel[i][j][1] = c[1];
    pixel[i][j][2] = c[2];
}

void set_pixel_sprite(int i, int j, int c[3])
{
    mario[i][j][0] = c[0];
    mario[i][j][1] = c[1];
    mario[i][j][2] = c[2];
}

int load_sprite()
{
    FILE *src = fopen(src_name, "r");
    if (!src)
        return 0; // error

    char c;
    for (int j = 15; j >= 0; j--)
    {
        for (int i = 0; i < 16; i++)
        {
            fscanf(src, "%c", &c);
            switch (c)
            {
            case 'P':
                set_pixel_sprite(i, j, piel);
                break;
            case 'R':
                set_pixel_sprite(i, j, rojo);
                break;
            case 'C':
                set_pixel_sprite(i, j, cafe);
                break;
            default:
                mario[i][j][0] =
                    mario[i][j][1] =
                        mario[i][j][2] = 0;
                break;
            }
        }
        fscanf(src, "%c", &c);
    }

    fclose(src);
    return 1; // success
}

void show_sprite()
{
    for (int j = 0; j < 16; j++)
    {
        for (int i = 0; i < 16; i++)
        {
            printf("%d %d %d ", mario[i][j][0], mario[i][j][1], mario[i][j][2]);
        }
        printf("\n");
    }
}

void show_pixels()
{
    for (int j = HEIGHT - 1; j >= 0; j--)
    {
        for (int i = 0; i < WIDTH; i++)
        {
            printf("( ");
            for (int k = 0; k < 3; k++)
                printf("%d ", pixel[i][j][k]);
            printf(")");
        }
        printf("\n");
    }
}

int save_pixels()
{
    FILE *dest = fopen(file_name, "w");
    if (!dest)
        return 0; // error

    fprintf(dest, "P3\n");
    fprintf(dest, "%d %d\n", WIDTH, HEIGHT);
    fprintf(dest, "%d\n", 255);

    // valores RGB
    for (int j = HEIGHT - 1; j >= 0; j--)
    {
        for (int i = 0; i < WIDTH; i++)
            for (int k = 0; k < 3; k++)
                fprintf(dest, "%d ", pixel[i][j][k]);
        fprintf(dest, "\n");
    }

    fclose(dest);
    return 1; // success
}

void draw_sprite(int tx, int ty, int sx, int sy, float ang)
{
    ang *= PI / 180.0;
    for (int j = 0; j < 16; j++)
    {
        for (int i = 0; i < 16; i++)
        {
            int x = sx * (i - 8);
            int y = sy * (j - 8);

            float x2 = x * cos(ang) - y * sin(ang);
            float y2 = x * sin(ang) + y * cos(ang);

            x2 += tx;
            y2 += ty;

            set_pixel((int)x2, (int)y2, mario[i][j]);
            set_pixel((int)x2 - 1, (int)y2, mario[i][j]);
            set_pixel((int)x2 + 1, (int)y2, mario[i][j]);
            set_pixel((int)x2, (int)y2 - 1, mario[i][j]);
            set_pixel((int)x2, (int)y2 + 1, mario[i][j]);
        }
    }
}


void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}