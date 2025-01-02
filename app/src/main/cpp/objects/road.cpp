#include <objects/road.hpp>
#include <window/window.hpp>
#include <utils/renderTilingTexture.hpp>
#include <assets/assets.hpp>
#include <SDL_image.h>


RoadGenerator::RoadGenerator() {
    // Load the road texture
    texture = IMG_LoadTexture_RW(
        window.renderer,
        SDL_Incbin(SPRITE_ROAD_ROAD_PNG),
        SDL_TRUE
    );    
    if (!texture) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
    }
}

RoadGenerator::~RoadGenerator() {
    // Clean up the texture
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void RoadGenerator::render(int Y) {
    if (!texture) { return; }

    // Render the road texture with tiling
    renderTilingTexture(texture, -Y, 0.2);
}
