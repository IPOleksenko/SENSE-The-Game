#include <iostream>
#include <SDL.h>

#include "window/window.h"

class Camera {
private:
    SDL_Rect viewport;

public:
    Camera() {
        SDL_RenderSetViewport(window.renderer, &viewport);
        
        viewport.x = 0;
        viewport.y = 0;
        
    }
    void move() {
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window.window, &windowWidth, &windowHeight);
        viewport.h = windowWidth;
        viewport.w = windowHeight;

        SDL_RenderSetViewport(window.renderer, &viewport);
    }
};