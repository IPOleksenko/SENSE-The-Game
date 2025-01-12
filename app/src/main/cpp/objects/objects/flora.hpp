#pragma once

#include <utils/texture.hpp>
#include <vector>
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
        const Texture& texture;
        SDL_Point pos;
    };
    std::list<TexView> m_floraView;

    int m_lastPosY = 0;         // Last value of playerY
    int m_nextGenY = 0;         // Coordinate for the next generation

    static const int s_genDist = 3;    // Distance between textures
    static const int s_density = 5;    // Density
};
