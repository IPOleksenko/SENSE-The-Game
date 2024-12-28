#pragma once

#include <SDL.h>
struct SDL_Rect;

class Camera {
private:
    SDL_Rect viewport; // Rectangle defining the viewport

public:
    Camera();
    void move();
};
