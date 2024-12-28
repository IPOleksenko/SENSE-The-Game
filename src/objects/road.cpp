#pragma once

#include <SDL.h>
#include <SDL_image.h>  // For image loading
#include <iostream>     // For console output
#include "../window/window.cpp"
#include "../utils/renderTilingTexture.cpp"

class RoadGenerator {
private:
    SDL_Texture* texture = nullptr;

public:
    RoadGenerator() {
        // Load the road texture
        texture = IMG_LoadTexture(window.renderer, "assets/sprite/road/road.png");
        if (!texture) {
            SDL_Log("Failed to load texture: %s", SDL_GetError());
        }
    }

    ~RoadGenerator() {
        // Clean up the texture
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }

    /**
     * Render the road.
     *
     * @param Y Vertical offset for the road texture.
     */
    void render(int Y) {
        if (!texture) { return; }

        // Render the road texture with tiling
        renderTilingTexture(texture, -Y, 0.2);
    }
};
