#include <objects/scale.hpp>
#include <SDL_image.h>
#include <window/window.hpp>
#include <assets/assets.hpp>


ScaleGenerator::ScaleGenerator() {
    arrowTexture = IMG_LoadTexture_RW(
        window.renderer,
        SDL_Incbin(SPRITE_SCALE_ARROW_PNG),
        SDL_TRUE
    );
    if (!arrowTexture) {
        SDL_Log("Failed to load scale arrow texture: %s", SDL_GetError());
    }

    scaleTexture = IMG_LoadTexture_RW(
        window.renderer,
        SDL_Incbin(SPRITE_SCALE_SCALE_PNG),
        SDL_TRUE
    );
    if (!scaleTexture) {
        SDL_Log("Failed to load scale texture: %s", SDL_GetError());
    }
}

ScaleGenerator::~ScaleGenerator() {
    // Cleanup textures
    if (arrowTexture) SDL_DestroyTexture(arrowTexture);
    if (scaleTexture) SDL_DestroyTexture(scaleTexture);
}

int ScaleGenerator::interpolate(int start, int end, double t) {
    return static_cast<int>(start + t * (end - start));
}

void ScaleGenerator::render(int playerSpeed, int playerMinSpeed, int playerMaxSpeed) {
    if (!arrowTexture || !scaleTexture) return;

    // Ratios for scaling dimensions
    const double scaleWidthRatio = 0.8;  // 80% of screen width
    const double scaleHeightRatio = 0.06; // 6% of screen height
    const double arrowWidthRatio = 0.05;  // 5% of screen width
    const double arrowHeightRatio = 0.05; // Maintain aspect ratio for arrow

    // Scale rectangle (background)
    SDL_Rect scaleRect;
    scaleRect.w = static_cast<int>(window.BASE_WIDTH * scaleWidthRatio);
    scaleRect.h = static_cast<int>(window.BASE_HEIGHT * scaleHeightRatio);
    scaleRect.x = (window.BASE_WIDTH - scaleRect.w) / 2; // Center horizontally
    scaleRect.y = static_cast<int>(window.BASE_HEIGHT * 0.95 - scaleRect.h); // Position 5% from bottom

    SDL_RenderCopy(window.renderer, scaleTexture, nullptr, &scaleRect);

    // Arrow rectangle
    SDL_Rect arrowRect;
    arrowRect.w = static_cast<int>(window.BASE_WIDTH * arrowWidthRatio);
    arrowRect.h = static_cast<int>(window.BASE_HEIGHT * arrowHeightRatio);
    arrowRect.y = static_cast<int>(window.BASE_HEIGHT * 0.98 - arrowRect.h); // Position 2% from bottom

    // Define scale boundaries
    int scaleStartX = scaleRect.x;               // Left edge of the scale
    int scaleEndX = scaleRect.x + scaleRect.w;   // Right edge of the scale
    int scaleWidth = scaleEndX - scaleStartX;

    // Define key positions on the scale
    int minPosition = scaleStartX + scaleWidth / 3;  // 1/3 of the scale
    int maxPosition = scaleStartX + 2 * scaleWidth / 3; // 2/3 of the scale
    int midPosition = scaleStartX + scaleWidth / 2;    // Center of the scale

    // Calculate arrow position based on playerSpeed
    int arrowCenterX;
    if (playerSpeed <= playerMinSpeed) {
        // Map [0, playerMinSpeed] to [scaleStartX, minPosition]
        double t = static_cast<double>(playerSpeed) / playerMinSpeed;
        arrowCenterX = interpolate(scaleStartX, minPosition, t);
    }
    else if (playerSpeed >= playerMaxSpeed) {
        // Map [playerMaxSpeed, ...] to [maxPosition, scaleEndX]
        double t = static_cast<double>(playerSpeed - playerMaxSpeed) / playerMaxSpeed;
        arrowCenterX = interpolate(maxPosition, scaleEndX, t);
    }
    else {
        // Map [playerMinSpeed, playerMaxSpeed] to [minPosition, maxPosition]
        double t = static_cast<double>(playerSpeed - playerMinSpeed) / (playerMaxSpeed - playerMinSpeed);
        arrowCenterX = interpolate(minPosition, maxPosition, t);
    }

    // Center the arrow horizontally on the calculated position
    arrowRect.x = arrowCenterX - arrowRect.w / 2;

    // Render the arrow
    SDL_RenderCopy(window.renderer, arrowTexture, nullptr, &arrowRect);
}
