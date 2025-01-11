#pragma once

#include <utils/texture.hpp>
#include <list>

class Flora {
public:
    explicit Flora(SDL_Renderer* renderer);
    ~Flora() = default;

    void render(const SDL_Point& areaSize, const int& posY);
    void reset();

private:
    std::vector<RawTexture> m_textures;

    struct TexView {
        Texture texture;
        SDL_Point pos;
    };
    std::list<TexView> m_floraView;

    int m_lastPosY = 0;         // Last value of playerY
    int m_nextGenY = 0;         // Coordinate for the next generation

    static const int s_genDist = 3;    // Distance between textures
    static const int s_density = 5;    // Density
};


/*
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
*/


