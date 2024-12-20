#pragma once

#include <SDL.h>
#include <SDL_image.h>  // For image loading
#include <iostream>     // For console output
#include "window/window.h"

void renderTilingTexture(SDL_Texture* texture, int startY, double scale, bool fullscreen = false) {
    if (!texture || !window.renderer) return; 

    int windowWidth, windowHeight;
    SDL_GetRendererOutputSize(window.renderer, &windowWidth, &windowHeight);

    SDL_Rect dstRect;
    for (int y = startY; y < windowHeight; y += getScaledRect(texture, window.window, scale, y).h) {
        if (fullscreen) {
            dstRect = { 0, y, windowWidth, windowHeight };
        }
        else {
            dstRect = getScaledRect(texture, window.window, scale, y);
        }
        SDL_RenderCopy(window.renderer, texture, nullptr, &dstRect);
    }
}

