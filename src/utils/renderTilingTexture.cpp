#pragma once

#include <SDL.h>
#include <SDL_image.h>  // For image loading
#include <iostream>     // For console output
#include "window/window.cpp"

/**
 * Renders a tiling texture on the screen.
 *
 * @param texture The SDL_Texture to render.
 * @param startY The vertical starting position for the tiling.
 * @param scale Scaling factor for the texture.
 * @param fullscreen If true, the texture will cover the entire screen without maintaining aspect ratio.
 */
void renderTilingTexture(SDL_Texture* texture, int startY, double scale, bool fullscreen = false) {
    if (!texture || !window.renderer) return;

    // Get the original texture dimensions
    int textureWidth, textureHeight;
    SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

    // Calculate scaling factors
    float scaleX = static_cast<float>(window.BASE_WIDTH) / textureWidth;
    float scaleY = static_cast<float>(window.BASE_HEIGHT) / textureHeight;
    float uniformScale = std::min(scaleX, scaleY) * scale;

    // Calculate the scaled dimensions of the texture
    int scaledWidth = static_cast<int>(textureWidth * uniformScale);
    int scaledHeight = static_cast<int>(textureHeight * uniformScale);

    SDL_Rect dstRect;
    for (int y = startY; y < window.BASE_HEIGHT; y += scaledHeight) {
        if (fullscreen) {
            // Stretch the texture to fill the screen
            dstRect = { 0, y, window.BASE_WIDTH, window.BASE_HEIGHT };
        }
        else {
            // Center the texture horizontally and offset it vertically
            dstRect.x = (window.BASE_WIDTH - scaledWidth) / 2;  // Horizontal centering
            dstRect.y = y;                                      // Vertical offset
            dstRect.w = scaledWidth;
            dstRect.h = scaledHeight;
        }
        // Render the texture
        SDL_RenderCopy(window.renderer, texture, nullptr, &dstRect);
    }
}
