#pragma once

#include <utils/texture.hpp>
#include <SDL.h>


class Scale {
public:
    explicit Scale(SDL_Renderer* renderer);
    ~Scale() = default;

    void render(
        const SDL_Point& areaSize,
        const float& playerSpeed,
        const float& playerSpeedMin,
        const float& playerSpeedMax
    );

private:
    RawTexture bar;
    RawTexture arrow;

    static int interpolate(
        const int& start,
        const int& end,
        const double& t
    );
};
