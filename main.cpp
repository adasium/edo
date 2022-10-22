#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


class Image {
public:
    SDL_Surface* surface;
    SDL_Texture* texture;
public:
    Image(std::string path) {
        if(path.substr(path.size() - 3) == "bmp") {
            std::cout << "[IMG] Loading BMP image: " << path << std::endl;
            this->surface =
              SDL_LoadBMP(path.c_str());
        } else {
          std::cout << "[IMG] Loading image surface: " << path << std::endl;
          this->surface = IMG_Load(path.c_str());
          if (!this->surface) {
            std::cout << "[IMG] Unable to load image " << path
                      << "! SDL Error: " << SDL_GetError() << std::endl;
          }
        }
    }
    Image(std::string path, SDL_Renderer* renderer) : Image(path) {
      std::cout << "[IMG] Loading image texture: " << path << std::endl;
      this->texture = SDL_CreateTextureFromSurface(renderer, this->surface);
      if (!this->texture){
        std::cout << "[IMG] Unable to create texture from " << path
                  << "! SDL Error: " << SDL_GetError() << std::endl;
      }
    }
    ~Image() {
        if (this->surface){
            SDL_FreeSurface(this->surface);
        }
    }

};


class Game {

    SDL_Window *window = nullptr;
    SDL_Surface *screen_surface = nullptr;
    SDL_Renderer *renderer = nullptr;


public:
    Game() {}

    ~Game() {
        SDL_DestroyRenderer(this->renderer);
        SDL_DestroyWindow(this->window);
        IMG_Quit();
        SDL_Quit();
    }
    int init() {
        std::cout << "Hello. Initialising SDL2." << std::endl;

        if (SDL_Init(SDL_INIT_VIDEO) < 0) {
            std::cout << "SDL could not initialise! SDL_Error: " << SDL_GetError()
                      << std::endl;
            SDL_DestroyWindow(window);
            return 1;
        }

        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            std::cout << "SDL_image could not initialise! SDL_image Error: " << IMG_GetError() << std::endl;
            return 1;
        }

        this->window = SDL_CreateWindow("edo", SDL_WINDOWPOS_UNDEFINED,
                                        SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH,
                                        SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
        if(!this->renderer) {
          std::cout << "Renderer could not be created! SDL Error: "
                    << SDL_GetError() << std::endl;
          return 1;
        }
        if (!window) {
            std::cout << "Window could not be created! SDL_Error: "
                      << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            return 1;
        }
        return 0;
    }


    void start() {

        Image placeholder_img = Image("placeholder.png", this->renderer);

        screen_surface = SDL_GetWindowSurface(window);
        SDL_FillRect(screen_surface, nullptr,
                     SDL_MapRGB(screen_surface->format, 0xF0, 0xF0, 0xF0));

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
                    case SDLK_w:
                    case SDLK_UP:
                        posy -= howfast;
                      break;
                    case SDLK_s:
                    case SDLK_DOWN:
                        posy += howfast;
                      break;
                    case SDLK_a:
                    case SDLK_LEFT:
                      posx -= howfast;
                      break;
                    case SDLK_d:
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

                SDL_RenderClear(this->renderer);
                SDL_RenderCopy(this->renderer, placeholder_img.texture, nullptr, &rect);
                SDL_RenderPresent(this->renderer);
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
