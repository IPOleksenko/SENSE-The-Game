#include <objects/scale.hpp>
#include "../utils/utils/texture.hpp"
#include <assets/assets.hpp>
#include <SDL_image.h>

Scale::Scale(SDL_Renderer *renderer) :
    bar(SDL_Incbin(SPRITE_SCALE_SCALE_PNG), renderer),
    arrow(SDL_Incbin(SPRITE_SCALE_ARROW_PNG), renderer)
{}

int Scale::interpolate(
    const int& start,
    const int& end,
    const double& t
) {
    return static_cast<int>(start + t * (end - start));
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
        (areaSize.x - barSize.x) / 2,
        static_cast<int>(areaSize.y * 0.95 - barSize.y)
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
        static_cast<int>(areaSize.y * 0.98 - arrowSize.y)
    };

    const int scaleStartX = barRect.x;              // Left edge of the scale
    const int scaleEndX = barRect.x + barRect.w;    // Right edge of the scale
    const int scaleWidth = scaleEndX - scaleStartX;

    // Define key positions on the scale
    const int minPos = scaleStartX + scaleWidth / 3;      // 1/3 of the scale
    const int maxPos = scaleStartX + 2 * scaleWidth / 3;  // 2/3 of the scale
    const int midPos = scaleStartX + scaleWidth / 2;      // Center of the scale

    // Calculate arrow position based on playerSpeed
    int arrowCenterX;
    if (playerSpeed <= playerSpeedMin) {
        // Map [0, playerSpeedMin] to [scaleStartX, minPos]
        float t = playerSpeed / playerSpeedMin;
        arrowCenterX = interpolate(scaleStartX, minPos, t);
    }
    else if (playerSpeed >= playerSpeedMax) {
        // Map [playerSpeedMax, ...] to [maxPos, scaleEndX]
        float t = (playerSpeed - playerSpeedMax) / playerSpeedMax;
        arrowCenterX = interpolate(maxPos, scaleEndX, t);
    }
    else {
        // Map [playerSpeedMin, playerSpeedMax] to [minPos, maxPos]
        float t = (playerSpeed - playerSpeedMin) / (playerSpeedMax - playerSpeedMin);
        arrowCenterX = interpolate(minPos, maxPos, t);
    }
    // Center the arrow horizontally on the calculated position
    arrowPos.x = arrowCenterX  - arrowSize.x / 2;

    const SDL_Rect arrowRect {
        arrowPos.x, arrowPos.y,
        arrowSize.x, arrowSize.y,
    };
    // Render the arrow
    arrow.render(&arrowRect, nullptr);
}
