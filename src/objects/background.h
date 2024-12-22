#pragma once

#include <SDL.h>
#include <SDL_image.h>  // For image loading
#include <iostream>     // For console output
#include "window/window.h"
#include "utils/renderTilingTexture.h"

class backgroundGenerator {
private:
    SDL_Texture* frontTexture = nullptr; // Texture for the foreground background
    SDL_Texture* backTexture = nullptr;  // Texture for the far background

public:
    // Constructor: loads textures for the background layers
    backgroundGenerator() {
        // Load the foreground background texture
        frontTexture = IMG_LoadTexture(window.renderer, "assets/sprite/background/front_background.png");

        // Load the far background texture
        backTexture = IMG_LoadTexture(window.renderer, "assets/sprite/background/back_background.png");

        // Check if the foreground texture failed to load
        if (!frontTexture) {
            SDL_Log("Failed to load foreground texture: %s", SDL_GetError());
        }

        // Check if the back background texture failed to load
        if (!backTexture) {
            SDL_Log("Failed to load background texture: %s", SDL_GetError());
        }
    }

    // Destructor: cleans up the textures to free memory
    ~backgroundGenerator() {
        if (frontTexture) {
            SDL_DestroyTexture(frontTexture); // Free the foreground texture
        }
        if (backTexture) {
            SDL_DestroyTexture(backTexture);  // Free the background texture
        }
    }

    // Renders the background layers based on the player's Y-coordinate
    void render(int Y) {
        if (!frontTexture || !backTexture) { return; } // Ensure textures are loaded

        // Render the back background layer, adjusted by the player's position
        renderTilingTexture(backTexture, -Y, 1, true);

        // Render the front background layer, adjusted by the player's position
        renderTilingTexture(frontTexture, -Y, 1);
    }
};
