#pragma once

#include <SDL.h>
#include <SDL_image.h>  // For image loading
#include <iostream>     // For console output
#include "window/window.h"
#include "utils/renderTilingTexture.h"

class backgroundGenerator {
private:
    SDL_Texture* frontTexture = nullptr;
    SDL_Texture* backTexture = nullptr;

public:
    backgroundGenerator() {
        frontTexture = IMG_LoadTexture(window.renderer, "assets/sprite/background/front_background.png");
        backTexture = IMG_LoadTexture(window.renderer, "assets/sprite/background/back_background.png");
        if (!frontTexture) {
            SDL_Log("Failed to load texture: %s", SDL_GetError());
        }
    }

    ~backgroundGenerator() {
        if (frontTexture || backTexture) {
            SDL_DestroyTexture(frontTexture);
        }
    }

    void render(int Y) {
        if (!frontTexture || !backTexture) { return; }

        renderTilingTexture(backTexture, -Y, 1, true);
        renderTilingTexture(frontTexture, -Y, 1);
        
    }
};
