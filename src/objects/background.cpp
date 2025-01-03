#include <objects/background.hpp>
#include <window/window.hpp>
#include <utils/renderTilingTexture.hpp>
#include <assets/assets.hpp>
#include <SDL_image.h>


BackgroundGenerator::BackgroundGenerator() {
    // Load the foreground background texture
    frontTexture = IMG_LoadTexture_RW(
        window.renderer,
        SDL_Incbin(SPRITE_BACKGROUND_FRONT_PNG),
        SDL_TRUE
    );

    // Load the far background texture
    backTexture = IMG_LoadTexture_RW(
        window.renderer,
        SDL_Incbin(SPRITE_BACKGROUND_BACK_PNG),
        SDL_TRUE
    );

    // Check if the foreground texture failed to load
    if (!frontTexture) {
        SDL_Log("Failed to load foreground texture: %s", SDL_GetError());
    }

    // Check if the back background texture failed to load
    if (!backTexture) {
        SDL_Log("Failed to load background texture: %s", SDL_GetError());
    }
}

BackgroundGenerator::~BackgroundGenerator() {
    if (frontTexture) {
        SDL_DestroyTexture(frontTexture); // Free the foreground texture
    }
    if (backTexture) {
        SDL_DestroyTexture(backTexture);  // Free the background texture
    }
}

void BackgroundGenerator::render(int Y) {
    if (!frontTexture || !backTexture) { return; } // Ensure textures are loaded

    // Render the back background layer, adjusted by the player's position
    renderTilingTexture(backTexture, -Y, 1, true);

    // Render the front background layer, adjusted by the player's position
    renderTilingTexture(frontTexture, -Y, 1);
}
