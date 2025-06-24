#include <SDL2/SDL.h>
#include <stdio.h>
#include <unistd.h>	// to use usleep()

#include "utils.h"	// Include the header for utils functions
#include "my2d.h"
#include "poly.h"
#include "my3d.h"


camera cam = {-5.0, -4.0, 0.0, 0.0, 0.0, 0};

transform obj = {
    0.0, 0.0, 0.0,
    0.0, 0.0, 0.0,
    1.0
};

transform obj_cube = {
    -1.5, 0.250, 0.0,
    0.0, 0.0, 0.0,
    0.25
};

// Image pixels
unsigned int pixel[WIDTH * HEIGHT];

// Mario sprite
int mario[16][16][3];
// Cube textures
int cube_top[16][16][3];
int cube_side[16][16][3];
int cube_bottom[16][16][3];

// Some colors
int black[3] = {0, 0, 0};
int red[3] = {255, 0, 0};
int green[3] = {0, 255, 0};
int blue[3] = {0, 0, 255};
int cyan[3] = {0, 255, 255};
int magenta[3] = {255, 0, 255};
int yellow[3] = {255, 255, 0};
int white[3] = {255, 255, 255};
int gray[3] = {100, 100, 100};


enum Fig current = GRID;

int should_update = 0;
int is_changing = 0;

float curr[M * N][3];
float next[M * N][3];

float t = 2.0;
float deltaT = 0.05;


int main(int argc, char *argv[]) {
    (void) argc; // Suppress unused parameter warning
    (void) argv; // Suppress unused parameter warning

    // Set Wayland as the preferred video driver for Linux
    SDL_SetHint(SDL_HINT_VIDEODRIVER, "wayland,x11");

    // Init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Error starting SDL: %s\n", SDL_GetError());
        return 1; // ERROR status
    }

    // Create window
    SDL_Window *window = SDL_CreateWindow("SDL2 test",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Error creating window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1; // ERROR status
    }

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1; // ERROR status
    }

    // Create image texture - Using ARGB8888 format for Fedora Linux compatibility
    SDL_Texture *texture = SDL_CreateTexture(renderer,
                                             SDL_PIXELFORMAT_ARGB8888,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             WIDTH, HEIGHT);
    if (!texture) {
        printf("Error creating texture: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1; // ERROR status
    }

    // black at start - Using ARGB format (alpha is first byte)
    init_image(0xff000000);
    // Update texture with data from the array
    SDL_UpdateTexture(texture, NULL, pixel, WIDTH * sizeof(unsigned int));

    // load Mario sprite
    if (!load_mario_sprite()) {
        printf("ERROR loading mario sprite\n");
        return 1;
    }

    // load other sprites
    // TO DO

    // Create grid mesh
    mesh m = {
        M * N, 2 * M * N - M - N, (M - 1) * (N - 1),
        {0, 150, 0} // wireframe color
    };
    create_grid_verts(N, M, m.base_verts);
    create_grid_edges(N, M, m.edges);
    create_grid_faces(N, M, m.faces);

    create_grid_verts(N, M, curr); // current shape

    // Create cube mesh
    mesh cube = {
        8, 12, 6,
        {150, 50, 0}, // wireframe color
        {
            {1.0, 1.0, 1.0}, {-1.0, 1.0, 1.0},
            {1.0, -1.0, 1.0}, {-1.0, -1.0, 1.0},
            {1.0, 1.0, -1.0}, {-1.0, 1.0, -1.0},
            {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0}
        }, // 8 base_verts
        {
            {1.0, 1.0, 1.0}, {-1.0, 1.0, 1.0},
            {1.0, -1.0, 1.0}, {-1.0, -1.0, 1.0},
            {1.0, 1.0, -1.0}, {-1.0, 1.0, -1.0},
            {1.0, -1.0, -1.0}, {-1.0, -1.0, -1.0}
        }, // 8 verts
        {
            {0, 1}, {1, 3}, {3, 2}, {2, 0},
            {4, 5}, {5, 7}, {7, 6}, {6, 4},
            {1, 5}, {4, 0}, {3, 7}, {6, 2}
        }, // 12 edges
        {
            {3, 2, 0, 1}, {6, 7, 5, 4},
            {1, 0, 4, 5}, {2, 3, 7, 6},
            {2, 6, 4, 0}, {7, 3, 1, 5}
        } // 6 faces
    };

    // Show flags
    int show_verts = 1;
    int show_edges = 1;
    int show_faces = 1;
    int use_mario_sprite = 1;

    // Add shape information text
    char shape_name[32] = "Shape: Grid";

    // Main loop
    int quit = 0;
    SDL_Event e;
    while (!quit) {
        usleep(50000);

        // Handle shape transition animation
        if (is_changing) {
            t += deltaT;
            if (t >= 1.0) {
                t = 1.0;
                is_changing = 0;

                // Copy next shape to current shape
                for (int i = 0; i < M * N; i++) {
                    curr[i][0] = next[i][0];
                    curr[i][1] = next[i][1];
                    curr[i][2] = next[i][2];
                }
            }

            // Apply interpolation to update the mesh
            interpolate(curr, next, t, m.base_verts, M * N);
            should_update = 1;
        }

        // Manage events
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = 1;
            else if (e.type == SDL_KEYDOWN) {
                // Pressed key
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    //printf("ESC key pressed. Exit...\n");
                    quit = 1;
                } else if (e.key.keysym.sym == SDLK_UP) {
                    //printf("UP\n");
                    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT])
                        cam.D -= 0.1;
                    else cam.angX -= 5.0;
                } else if (e.key.keysym.sym == SDLK_DOWN) {
                    //printf("DOWN\n");
                    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT])
                        cam.D += 0.1;
                    else cam.angX += 5.0;
                } else if (e.key.keysym.sym == SDLK_LEFT) {
                    //printf("LEFT\n");
                    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT])
                        obj.tx -= 0.1;
                    else cam.angY -= 5.0;
                } else if (e.key.keysym.sym == SDLK_RIGHT) {
                    //printf("RIGHT\n");
                    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LSHIFT])
                        obj.tx += 0.1;
                    else cam.angY += 5.0;
                } else if (e.key.keysym.sym == SDLK_a) {
                    //printf("A - Grid\n");
                    if (!is_changing && current != GRID) {
                        current = GRID;
                        create_grid_verts(N, M, next);
                        t = 0.0;
                        is_changing = 1;
                        should_update = 1;
                        sprintf(shape_name, "Shape: Grid");
                    }
                } else if (e.key.keysym.sym == SDLK_c) {
                    //printf("C - Cylinder\n");
                    if (!is_changing && current != CYLINDER) {
                        current = CYLINDER;
                        create_cylinder_verts(N, M, next);
                        t = 0.0;
                        is_changing = 1;
                        should_update = 1;
                        sprintf(shape_name, "Shape: Cylinder");
                    }
                } else if (e.key.keysym.sym == SDLK_o) {
                    //printf("O - Cone\n");
                    if (!is_changing && current != CONE) {
                        current = CONE;
                        create_cone_verts(N, M, next);
                        t = 0.0;
                        is_changing = 1;
                        should_update = 1;
                        sprintf(shape_name, "Shape: Cone");
                    }
                } else if (e.key.keysym.sym == SDLK_s) {
                    //printf("S - Sphere\n");
                    if (!is_changing && current != SPHERE) {
                        current = SPHERE;
                        create_sphere_verts(N, M, next);
                        t = 0.0;
                        is_changing = 1;
                        should_update = 1;
                        sprintf(shape_name, "Shape: Sphere");
                    }
                } else if (e.key.keysym.sym == SDLK_t) {
                    //printf("T - Torus\n");
                    if (!is_changing && current != TORUS) {
                        current = TORUS;
                        create_torus_verts(N, M, TORUS_RATIO, next);
                        t = 0.0;
                        is_changing = 1;
                        should_update = 1;
                        sprintf(shape_name, "Shape: Torus");
                    }
                } else if (e.key.keysym.sym == SDLK_r) {
                    //printf("R - Rotate object\n");
                    obj.angY += 15.0;
                    if (obj.angY >= 360.0) obj.angY -= 360.0;
                } else if (e.key.keysym.sym == SDLK_p) {
                    printf("is flat_proj: %d\n", cam.is_flat_proj);
                    cam.is_flat_proj = !cam.is_flat_proj;
                } else if (e.key.keysym.sym == SDLK_1) {
                    //printf("1\n");
                    show_verts = !show_verts;
                } else if (e.key.keysym.sym == SDLK_2) {
                    //printf("2\n");
                    show_edges = !show_edges;
                } else if (e.key.keysym.sym == SDLK_3) {
                    //printf("3\n");
                    show_faces = !show_faces;
                } else if (e.key.keysym.sym == SDLK_4) {
                    //printf("4\n");
                    use_mario_sprite = !use_mario_sprite;
                }
            }
        }

        // Clean the screen
        SDL_RenderClear(renderer);

        // Initialize with dark gray background
        init_image(0xff1f1f1f);


        // Draw mario sprite
        draw_mario_sprite(10, 10, 2, 2, 0.0);

        // MESH
        transform_verts(obj, &m);
        if (show_faces) {
            if (use_mario_sprite)
                draw_faces(&m, gray, cam, mario);
            else
                draw_faces(&m, gray, cam, NULL);
        }
        if (show_edges)
            draw_edges(&m, cam);
        if (show_verts)
            draw_verts(&m, white, cam);

        // CUBE
        transform_verts(obj_cube, &cube);
        if (show_faces) {
            if (use_mario_sprite)
                draw_faces(&cube, gray, cam, mario);
            else
                draw_faces(&cube, gray, cam, NULL);
        }
        if (show_edges)
            draw_edges(&cube, cam);
        if (show_verts)
            draw_verts(&cube, white, cam);

        // Update texture with data from the pixel array
        SDL_UpdateTexture(texture, NULL, pixel, WIDTH * sizeof(unsigned int));

        // Render texture
        SDL_RenderCopy(renderer, texture, NULL, NULL);

        // Show window
        SDL_RenderPresent(renderer);
    }

    // Release resources
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0; // SUCCESS status
}
