#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


class Image {
public:
    SDL_Surface* surface;
public:
    Image(std::string path) {
        if(path.substr(path.size() - 3) == "bmp") {
            std::cout << "Loading BMP image: " << path << std::endl;
            this->surface =
              SDL_LoadBMP(path.c_str());
        } else {
          std::cout << "Loading image: " << path << std::endl;
          this->surface = IMG_Load(path.c_str());
        }
    }
    ~Image() { SDL_FreeSurface(this->surface); }
};


class Game {

    SDL_Window *window = nullptr;
    SDL_Surface *screen_surface = nullptr;

public:
    Game() {}

    ~Game() {
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();
    }
    int init() {
        std::cout << "Hello. Initialising SDL2." << std::endl;

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cout << "SDL could not initialise! SDL_Error: " << SDL_GetError()
                      << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            std::cout << "SDL_image could not initialise! SDL_image Error: " << IMG_GetError() << std::endl;
        }

        this->window = SDL_CreateWindow("edo", SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                        SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (!window) {
            std::cout << "Window could not be created! SDL_Error: "
                      << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            return 1;
        }
        return 0;
    }


    void start() {

        Image placeholder_img = Image("placeholder.png");

        screen_surface = SDL_GetWindowSurface(window);
        SDL_FillRect(screen_surface, nullptr,
                     SDL_MapRGB(screen_surface->format, 0xFF, 0xFF, 0xFF));

        SDL_Event e;
        bool quit = false;
        int posx = 0;
        int posy = 0;
        int howfast = 10;
        while (!quit) {
            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    quit = true;
                } else if (e.type == SDL_KEYDOWN) {
                    switch (e.key.keysym.sym) {
                    case SDLK_UP:
                        posy -= howfast;
                      break;
                    case SDLK_DOWN:
                        posy += howfast;
                      break;
                    case SDLK_LEFT:
                        posx -= howfast;
                        break;
                    case SDLK_RIGHT:
                      posx+= howfast;
                      break;
                    case SDLK_q:
                        quit = true;
                        break;
                    default:
                        break;
                    }
                }
                SDL_Rect rect;
                rect.x = posx;
                rect.y = posy;
                rect.w = SCREEN_WIDTH / 2;
                rect.h = SCREEN_HEIGHT / 2;

                SDL_BlitSurface(placeholder_img.surface, NULL, screen_surface, &rect);
                SDL_UpdateWindowSurface(window);
            }
        }
    }
};

int main() {
    Game game = Game();
    if (game.init()) {
        return 1;
    }
    game.start();
    return 0;

}
