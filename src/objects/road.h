#pragma once

#include <SDL.h>
#include <SDL_image.h>  // For image loading
#include <iostream>     // For console output
#include "window/window.h"

class RoadGenerator {
private:
    SDL_Texture* texture = nullptr;
    float x = 0.0f;
    float y = 0.0f;
    float velocity = 0.0f;
    const float acceleration = 2.0f;
    const float deceleration = 0.5f;
    const float maxSpeed = 100.0f;
    const float minSpeed = 50.0f;
    const float requiredSpeed = (maxSpeed + minSpeed) / 2;
    bool isMoving = false;
    bool lastPressedA = false;

public:
    RoadGenerator() {
        texture = IMG_LoadTexture(window.renderer, "assets/sprite/road/road.png");
        if (!texture) {
            SDL_Log("Failed to load texture: %s", SDL_GetError());
        }
    }

    ~RoadGenerator() {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }

    // Render the road
    void render() {
        if (!texture) { return; }

        // Using a function to calculate texture size relative to the window
        SDL_Rect dstRect = getScaledRect(texture, window.window, 0.2);

        // Render the texture
        SDL_RenderCopy(window.renderer, texture, nullptr, &dstRect);
    }
};
