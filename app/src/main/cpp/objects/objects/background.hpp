#pragma once

#include <utils/texture.hpp>
#include <SDL.h>

class Background {
public:
    explicit Background(SDL_Renderer* renderer);
    ~Background() = default;

    void render(const SDL_Point& areaSize, const int& posY);

private:
    RawTexture back;
    RawTexture front;
};
