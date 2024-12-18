#pragma once

#include <SDL.h>
#include <iostream>

class WINDOW {
public:
    SDL_Window* window = SDL_CreateWindow("SENSE The Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        800, 600,
        SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    bool running = true;
    SDL_Event event;

    WINDOW() {
        SDL_Surface* icon = SDL_LoadBMP("assets/icon.bmp"); // Load BMP file
        if (icon) {
            SDL_SetWindowIcon(window, icon); // Set window icon
            SDL_FreeSurface(icon);          // Free memory
        }
        else {
            std::cerr << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
        }
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            exit(1);
        }

        if (!window) {
            std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            exit(1);
        }

        if (!renderer) {
            std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
            SDL_DestroyWindow(window);
            SDL_Quit();
            exit(1);
        }
    }

    ~WINDOW() {
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
};

// Declare or define the object depending on where the header is included
#ifdef DEFINE_WINDOW
WINDOW window;
#else
extern WINDOW window;
#endif
