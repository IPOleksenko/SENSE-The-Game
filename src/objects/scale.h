#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "window/window.h"
#include "utils/scaledTextureSize.h"

class ScaleGenerator {
private:
    SDL_Texture* arrowTexture = nullptr;
    SDL_Texture* scaleTexture = nullptr;

public:
    ScaleGenerator() {
        arrowTexture = IMG_LoadTexture(window.renderer, "assets/sprite/scale/arrow.png");
        scaleTexture = IMG_LoadTexture(window.renderer, "assets/sprite/scale/scale.png");
        if (!arrowTexture || !scaleTexture) {
            SDL_Log("Failed to load texture: %s", SDL_GetError());
        }
    }

    ~ScaleGenerator() {
        if (arrowTexture) {
            SDL_DestroyTexture(arrowTexture);
        }
        if (scaleTexture) {
            SDL_DestroyTexture(scaleTexture);
        }
    }

    // Helper function to interpolate between two points
    int interpolate(int min, int max, double percentage) {
        return min + static_cast<int>((max - min) * percentage);
    }

    // Render method
    void render(int playerSpeed, int playerMinSpeed, int playerMaxSpeed) {
        if (!arrowTexture || !scaleTexture) {
            return;
        }

        // Get window size
        int windowWidth, windowHeight;
        SDL_GetWindowSize(window.window, &windowWidth, &windowHeight);

        // Scale rendering (5% from the bottom edge of the window)
        SDL_Rect scaleRect = getScaledRect(scaleTexture, window.window, 0.8, 0);
        scaleRect.h = static_cast<int>(windowHeight * 0.06);
        scaleRect.y = static_cast<int>(windowHeight * 0.95 - scaleRect.h); // 5% from the bottom edge

        SDL_RenderCopy(window.renderer, scaleTexture, nullptr, &scaleRect);

        // Arrow rendering (4% from the bottom edge of the window)
        SDL_Rect arrowRect = getScaledRect(arrowTexture, window.window, 0.05, 10);
        arrowRect.y = static_cast<int>(windowHeight * 0.98 - arrowRect.h); // 4% from the bottom edge

        // Calculate positions
        int scaleStartX = scaleRect.x;         // Left side of the scale
        int scaleEndX = scaleRect.x + scaleRect.w; // Right side of the scale
        int scaleWidth = scaleEndX - scaleStartX;

        // Calculate special points on the scale
        int minPosition = scaleStartX + scaleWidth / 3;  // 1/3 of the scale
        int maxPosition = scaleStartX + 2 * scaleWidth / 3; // 2/3 of the scale

        // Determine the arrow position based on playerSpeed
        int arrowCenterX;
        if (playerSpeed <= playerMinSpeed) {
            // Map [0, playerMinSpeed] to [scaleStartX, minPosition]
            double percentage = static_cast<double>(playerSpeed) / playerMinSpeed;
            arrowCenterX = interpolate(scaleStartX, minPosition, percentage);
        }
        else {
            // Map [playerMinSpeed, playerMaxSpeed] to [minPosition, maxPosition]
            double percentage = static_cast<double>(playerSpeed - playerMinSpeed) /
                (playerMaxSpeed - playerMinSpeed);
            arrowCenterX = interpolate(minPosition, maxPosition, percentage);
        }

        // Center the arrow horizontally
        arrowRect.x = arrowCenterX - arrowRect.w / 2;

        SDL_RenderCopy(window.renderer, arrowTexture, nullptr, &arrowRect);
    }
};
