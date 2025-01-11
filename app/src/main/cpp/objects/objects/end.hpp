#pragma once

#include <SDL.h>
#include <utils/texture.hpp>

class End {
public:
    explicit End(SDL_Renderer* renderer);
    ~End() = default;

    void render(const SDL_Point& areaSize);

private:
    RawTexture m_texture;
    int m_alpha ;
};
