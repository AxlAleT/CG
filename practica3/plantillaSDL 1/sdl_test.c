
#include <SDL2/SDL.h>
#include <stdio.h>
#include "utils.h"	// Include the header for utils functions

// Image pixels
unsigned int pixel[WIDTH*HEIGHT];

int main(int argc, char *argv[]) {
	// Init SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("Error starting SDL: %s\n", SDL_GetError());
		return 1;	// ERROR status
	}

	// Create window
	SDL_Window *window = SDL_CreateWindow("SDL2 test",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (!window) {
		printf("Error creating window: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;	// ERROR status
	}

	// Create renderer
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		printf("Error creating renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;	// ERROR status
	}

	// Create image texture
	SDL_Texture *texture = SDL_CreateTexture(renderer,
		SDL_PIXELFORMAT_RGBA8888,
		SDL_TEXTUREACCESS_STREAMING,
		WIDTH, HEIGHT);
	if (!texture) {
		printf("Error creating texture: %s\n", SDL_GetError());
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return 1;	// ERROR status
	}

	init_image(0x000000ff);
	// Update texture with data from the array
	SDL_UpdateTexture(texture, NULL, pixel, WIDTH*sizeof(unsigned int));

	// load Mario sprite
	if (!load_sprite()){
		printf("ERROR loading mario sprite\n");
		return 1;
	}

	// Main loop
	int quit = 0;
	SDL_Event e;
	while (!quit) {
		// Manage events
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
    				quit = 1;
			else if (e.type == SDL_KEYDOWN) {
				// Pressed key
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					printf("ESC key pressed. Exit...\n");
					quit = 1;
				}else if (e.key.keysym.sym == SDLK_UP) {
					printf("UP\n");
				}else if (e.key.keysym.sym == SDLK_DOWN) {
					printf("DOWN\n");
				}else if (e.key.keysym.sym == SDLK_LEFT) {
					printf("LEFT\n");
				}else if (e.key.keysym.sym == SDLK_RIGHT) {
					printf("RIGHT\n");
				}else if (e.key.keysym.sym == SDLK_a) {
					printf("A\n");
				}else {
					printf("Pressed key: %s\n", SDL_GetKeyName(e.key.keysym.sym));
				}
			}else if (e.type == SDL_KEYUP) {
				printf("Released key: %s\n", SDL_GetKeyName(e.key.keysym.sym));
			}
		}

		/*
		// Check keyboard state
		const Uint8 *state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_W]) {
			printf("W key pressed.\n");
		}
		*/

		// Clean the screen
		SDL_RenderClear(renderer);

		/**************
			Change here the image
		***************/
		change_image_test();

		// draw sprite on image
		//init_image(0x888888ff);
		draw_sprite();


		// Update texture with data from the array
		SDL_UpdateTexture(texture, NULL, pixel, WIDTH*sizeof(unsigned int));

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

	return 0;	// SUCCESS status
}


