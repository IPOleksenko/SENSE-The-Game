#include <objects/flora.hpp>
#include <window/window.hpp>
#include <assets/assets.hpp>
#include <SDL_image.h>
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <ctime>


Flora::Flora() {
    loadTextures();
    std::srand(std::time(nullptr)); // Seed for random number generation
}

Flora::~Flora() {
    for (SDL_Texture* texture : textures) {
        SDL_DestroyTexture(texture);
    }
}

void Flora::render(int playerY) {
    if (textures.empty()) {
        std::cerr << "No textures loaded to render." << std::endl;
        return;
    }

    // Generate new textures at fixed distances
    while (playerY + window.BASE_HEIGHT >= nextGenerationY) {
        // Generate several textures at the current Y level
        for (int i = 0; i < 5; ++i) { // Increase the number of textures
            SDL_Texture* texture = textures[std::rand() % textures.size()]; // Random texture

            int textureWidth, textureHeight;
            SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

            SDL_Rect destRect;
            destRect.x = getRandomXPosition(window.BASE_WIDTH, textureWidth); // Random x-position within allowed sections
            destRect.y = nextGenerationY; // Fixed y-position
            destRect.w = textureWidth;
            destRect.h = textureHeight;

            // Save texture and its position
            renderedTextures.push_back({ texture, destRect });
        }

        nextGenerationY += generationDistance; // Update coordinate for the next generation
    }

    // Update texture coordinates relative to playerY
    for (auto& element : renderedTextures) {
        element.destRect.y -= playerY - lastPlayerY;
        SDL_RenderCopy(window.renderer, element.texture, nullptr, &element.destRect);
    }

    lastPlayerY = playerY; // Update last `playerY`
}


void Flora::loadTextures() {
    textures = {
        IMG_LoadTexture_RW(window.renderer, SDL_Incbin(SPRITE_FLORA_FLOWER1_PNG), SDL_TRUE),
        IMG_LoadTexture_RW(window.renderer, SDL_Incbin(SPRITE_FLORA_FLOWER2_PNG), SDL_TRUE),
        IMG_LoadTexture_RW(window.renderer, SDL_Incbin(SPRITE_FLORA_FLOWER3_PNG), SDL_TRUE),
        IMG_LoadTexture_RW(window.renderer, SDL_Incbin(SPRITE_FLORA_FLOWER4_PNG), SDL_TRUE),
        IMG_LoadTexture_RW(window.renderer, SDL_Incbin(SPRITE_FLORA_GRASS_PNG), SDL_TRUE),
        IMG_LoadTexture_RW(window.renderer, SDL_Incbin(SPRITE_FLORA_SMALLROCK1_PNG), SDL_TRUE),
        IMG_LoadTexture_RW(window.renderer, SDL_Incbin(SPRITE_FLORA_SMALLROCK2_PNG), SDL_TRUE),
        IMG_LoadTexture_RW(window.renderer, SDL_Incbin(SPRITE_FLORA_SMALLROCK3_PNG), SDL_TRUE),
    };

    for (const auto& flora_texture : textures) {
        if (!flora_texture) {
            std::cerr << "Failed to load flora textures. " << "SDL_Error: " << SDL_GetError() << std::endl;
            break;
        }
    }

    if (textures.empty()) {
        std::cerr << "No valid textures found in " << assetsPath << std::endl;
    }
}

int Flora::getRandomXPosition(int screenWidth, int textureWidth) {
    // Divide the screen into 8 parts
    int sectionWidth = screenWidth / 9;

    // Allowed sections: 3, 4, 6, 7
    int allowedSections[] = { 2, 3, 5, 6 }; // Indices for a 0-based array

    // Choose a random allowed section
    int randomSection = allowedSections[std::rand() % 4];

    // Calculate x-coordinate within the selected section
    int xMin = randomSection * sectionWidth;
    int xMax = xMin + sectionWidth - textureWidth;

    return xMin + (std::rand() % (xMax - xMin + 1));
}
