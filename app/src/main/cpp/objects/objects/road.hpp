#pragma once

#include <utils/texture.hpp>
#include <SDL.h>

class Road {
public:
    explicit Road(SDL_Renderer* renderer);
    ~Road() = default;

    void render(const SDL_Point& areaSize, const int& posY);

private:
    RawTexture m_texture;
};
