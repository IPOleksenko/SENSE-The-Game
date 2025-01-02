#pragma once

#include <SDL.h>
#include <vector>
#include <string>


class Flora {
public:
    Flora();
    ~Flora();
    void render(int playerY);

private:
    struct RenderedTexture {
        SDL_Texture* texture;
        SDL_Rect destRect;
    };

    std::string assetsPath = "assets/sprite/flora";
    std::vector<SDL_Texture*> textures;
    std::vector<RenderedTexture> renderedTextures;

    int lastPlayerY = 0; // Last value of playerY
    int nextGenerationY = 0; // Coordinate for the next generation
    int generationDistance = 3.5; // Distance between textures

    void loadTextures();
    int getRandomXPosition(int screenWidth, int textureWidth);
};
