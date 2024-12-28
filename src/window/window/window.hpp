#pragma once

#include <SDL.h>
#include <iostream>

class WINDOW {
public:
    const static int finalCheckpoint = 25000;

    static int BASE_WIDTH;
    static int BASE_HEIGHT;

    SDL_Window* window = SDL_CreateWindow(
        "SENSE The Game",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        BASE_WIDTH, BASE_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );

    bool running = true;
    SDL_Event event;

    WINDOW();
    ~WINDOW();
};

// Declare or define the object depending on where the header is included
#ifdef DEFINE_WINDOW
    int WINDOW::BASE_WIDTH = 1280;
    int WINDOW::BASE_HEIGHT = 720;
WINDOW window;
#else
    extern WINDOW window;
#endif
