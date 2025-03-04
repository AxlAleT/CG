#include <iostream>
#include <SDL.h>

bool initSDL();

int main() {
    if (!initSDL()) {
        std::cerr << "[ERROR] SDL initialization failed." << std::endl;
        return 1;
    }

    std::cout << "[SUCCESS] SDL initialized correctly." << std::endl;
    SDL_Quit(); // Cleanup SDL
    return 0;
}

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "[ERROR] SDL could not initialize: " << SDL_GetError() << std::endl;
        return false;
    }
    return true;
}

