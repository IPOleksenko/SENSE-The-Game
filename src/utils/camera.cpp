#pragma once

#include <iostream>
#include <SDL.h>

#include "../window/window.cpp" 

class Camera {
private:
    SDL_Rect viewport; // Rectangle defining the viewport

public:
    Camera() {
        // Set initial viewport dimensions
        viewport.x = 0;
        viewport.y = 0;
        viewport.w = 1000; // Set width to 1000
        viewport.h = 600;  // Approximate height

        // Apply the viewport
        SDL_RenderSetViewport(window.renderer, &viewport);
    }

    void move() {
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window.window, &windowWidth, &windowHeight);

        // Set the viewport dimensions based on the window size
        viewport.w = std::max(1000, windowWidth); // Minimum width is 1000
        viewport.h = windowHeight;

        // Apply the updated viewport
        SDL_RenderSetViewport(window.renderer, &viewport);
    }
};
