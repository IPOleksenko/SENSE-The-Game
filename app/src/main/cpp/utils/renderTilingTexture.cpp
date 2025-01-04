#include <utils/renderTilingTexture.hpp>
#include <SDL.h>
#include <iostream>
#include <window/window.hpp>

void renderTilingTexture(SDL_Texture* texture, int startY, double scale, bool fullscreen) {
    Window& window = Window::getInstance();

    if (!texture || !window.renderer) return;

    // Get the original texture dimensions
    int textureWidth, textureHeight;
    SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

    // Calculate scaling factors
    float scaleX = static_cast<float>(Window::baseWidth) / textureWidth;
    float scaleY = static_cast<float>(window.baseHeight) / textureHeight;
    float uniformScale = std::min(scaleX, scaleY) * scale;

    // Calculate the scaled dimensions of the texture
    int scaledWidth = static_cast<int>(textureWidth * uniformScale);
    int scaledHeight = static_cast<int>(textureHeight * uniformScale);

    SDL_Rect dstRect;
    for (int y = startY; y < window.baseHeight; y += scaledHeight) {
        if (fullscreen) {
            // Stretch the texture to fill the screen
            dstRect = { 0, y, Window::baseWidth, window.baseHeight };
        }
        else {
            // Center the texture horizontally and offset it vertically
            dstRect.x = (Window::baseWidth - scaledWidth) / 2;  // Horizontal centering
            dstRect.y = y;                                      // Vertical offset
            dstRect.w = scaledWidth;
            dstRect.h = scaledHeight;
        }
        // Render the texture
        SDL_RenderCopy(window.renderer, texture, nullptr, &dstRect);
    }
}
