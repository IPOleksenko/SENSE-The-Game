#pragma once

#include <SDL.h>


class BackgroundGenerator {
private:
    SDL_Texture* frontTexture = nullptr; // Texture for the foreground background
    SDL_Texture* backTexture = nullptr;  // Texture for the far background

public:
    // Constructor: loads textures for the background layers
    BackgroundGenerator();

    // Destructor: cleans up the textures to free memory
    ~BackgroundGenerator();

    // Renders the background layers based on the player's Y-coordinate
    void render(int Y);
};
