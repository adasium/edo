#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <iostream>
#include <SDL2/SDL.h>


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


int main() {
    std::cout << "Hello. Initialising SDL2." << std::endl;

    SDL_Window* window = nullptr;
    SDL_Surface* screen_surface = nullptr;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    window = SDL_CreateWindow("edo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if(!window) {
        std::cout << "Window could not be created! SDL_Error: " << SDL_GetError()
                  << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    screen_surface = SDL_GetWindowSurface(window);
    SDL_FillRect(screen_surface, nullptr, SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));
    SDL_UpdateWindowSurface(window);
    SDL_Event e;
    bool quit = false;
    while(!quit) {
        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                quit = true;
            }
        }
    }
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
