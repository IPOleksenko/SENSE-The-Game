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
        const float& playerSpeedMax,
        const int& distance,
        const bool endlessMode
    );
    void reset();

private:
    RawTexture bar;
    RawTexture arrow;
    Uint8 m_alpha = 255;
    bool m_fadingOut = false;
    static float rangeFract(
        const float& start,
        const float& end,
        const float& t
    );
};
