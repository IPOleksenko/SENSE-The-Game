#pragma once

#include <SDL.h>
#include <SDL_image.h>  // For image loading
#include <iostream>     // For console output
#include "window/window.h"
#include "utils/renderTilingTexture.h"

class RoadGenerator {
private:
    SDL_Texture* texture = nullptr;

public:
    RoadGenerator() {
        texture = IMG_LoadTexture(window.renderer, "assets/sprite/road/road.png");
        if (!texture) {
            SDL_Log("Failed to load texture: %s", SDL_GetError());
        }
    }

    ~RoadGenerator() {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }

    // Render the road
    void render(int Y) {
        if (!texture) { return; }

        renderTilingTexture(texture, -Y, 0.2);
    }
};
