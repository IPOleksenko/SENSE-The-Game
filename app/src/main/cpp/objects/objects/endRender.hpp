#pragma once

#include <SDL.h>


class EndRender {
private:
    SDL_Texture* texture = nullptr;
    int alpha = 0; // Initial transparency (fully transparent image)

public:
    EndRender();
    ~EndRender();

    void render(int Y);
};
