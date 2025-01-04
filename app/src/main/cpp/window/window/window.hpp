#pragma once

#include <SDL.h>
#include <iostream>

class Window {
public:
    const static int baseWidth = 1280;
    const static int baseHeight = 720;

    static Window& getInstance();
    ~Window();

    // TODO: make private
    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running = true;
    SDL_Event event;
    
    // TODO: remove
    const static int finalCheckpoint = 25000;
private:
    Window();
};
