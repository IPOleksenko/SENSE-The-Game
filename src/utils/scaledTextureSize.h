#pragma once

#include <SDL.h>
#include <algorithm>


// Function to scale a texture while maintaining its aspect ratio and applying vertical offset
inline SDL_Rect getScaledRect(SDL_Texture* texture, SDL_Window* window, float scale = 0, float y = 0) {
    if (!texture || !window) {
        return { 0, 0, 0, 0 };
    }

    // Get the texture dimensions
    int textureWidth, textureHeight;
    SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

    // Get the window dimensions
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    // Calculate scaling factors for the window
    float scaleX = static_cast<float>(windowWidth) / textureWidth;
    float scaleY = static_cast<float>(windowHeight) / textureHeight;

    // Choose the smallest scaling factor to maintain aspect ratio
    float uniformScale = std::min(scaleX, scaleY) * scale;

    // Calculate the new texture dimensions
    int scaledWidth = static_cast<int>(textureWidth * uniformScale);
    int scaledHeight = static_cast<int>(textureHeight * uniformScale);

    // Center the texture in the window with vertical offset
    SDL_Rect dstRect;
    dstRect.x = (windowWidth - scaledWidth) / 2;  // Horizontal centering
    dstRect.y = y;  // Vertical offset
    dstRect.w = scaledWidth;
    dstRect.h = scaledHeight;

    return dstRect;
}
