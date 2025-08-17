#include <objects/scale.hpp>
#include "../utils/utils/texture.hpp"
#include <assets/assets.hpp>
#include <SDL_image.h>

Scale::Scale(SDL_Renderer *renderer) :
    bar(SDL_Incbin(SPRITE_SCALE_SCALE_PNG), renderer),
    arrow(SDL_Incbin(SPRITE_SCALE_ARROW_PNG), renderer)
{}

float Scale::rangeFract(
    const float& start,
    const float& end,
    const float& fract
) {
    return static_cast<float>((start) + fract * (end - start));
}

void Scale::render(
    const SDL_Point& areaSize,
    const float& playerSpeed,
    const float& playerSpeedMin,
    const float& playerSpeedMax
) {
    if (!bar.isInit() && !arrow.isInit()) {
        return;
    }

    const SDL_FPoint barScale {
        0.8f, 0.06f
    };
    const SDL_Point barSize {
        static_cast<int>(static_cast<float>(areaSize.x) * barScale.x),
        static_cast<int>(static_cast<float>(areaSize.y) * barScale.y),
    };
    const SDL_Point barPos {
        static_cast<int>((areaSize.x - barSize.x) / 2.0f),
        static_cast<int>(areaSize.y * 0.95f - barSize.y)
    };
    const SDL_Rect barRect {
        barPos.x, barPos.y,
        barSize.x, barSize.y,
    };
    bar.render(&barRect, nullptr);

    const SDL_FPoint arrowScale {
        0.05f, 0.05f
    };
    const SDL_Point arrowSize {
        static_cast<int>(static_cast<float>(areaSize.x) * arrowScale.x),
        static_cast<int>(static_cast<float>(areaSize.y) * arrowScale.y),
    };
    SDL_Point arrowPos {
        0,
        static_cast<int>(areaSize.y * 0.98f - arrowSize.y)
    };
    
    const float scaleStartX = barRect.x;              // Left edge of the scale
    const float scaleEndX = barRect.x + barRect.w;    // Right edge of the scale
    const float scaleWidth = barRect.w;

    // Define key positions on the scale
    const float minPos = scaleWidth / 3.0f;         // 1/3 of the scale
    const float maxPos = 2.0f * scaleWidth / 3.0f;  // 2/3 of the scale
    const float midPos = scaleWidth / 2.0f;         // Center of the scale

    // Calculate arrow position based on playerSpeed
    float arrowCenterX;
    if (playerSpeed <= playerSpeedMin) {
        float fract = playerSpeed / playerSpeedMin;
        arrowCenterX = rangeFract(0, minPos, fract);
    }
    else if (playerSpeed >= playerSpeedMax) {
        float fract = (playerSpeed - playerSpeedMax) / playerSpeedMax;
        arrowCenterX = rangeFract(maxPos, scaleWidth, fract);
    }
    else {
        float fract = (playerSpeed - playerSpeedMin) / (playerSpeedMax - playerSpeedMin);
        arrowCenterX = rangeFract(minPos, maxPos, fract);
    }
    // Center the arrow horizontally on the calculated position
    arrowPos.x = (arrowCenterX + scaleStartX) - (arrowSize.x / 2.0f);

    const SDL_Rect arrowRect {
        arrowPos.x, arrowPos.y,
        arrowSize.x, arrowSize.y,
    };
    // Render the arrow
    arrow.render(&arrowRect, nullptr);
}
