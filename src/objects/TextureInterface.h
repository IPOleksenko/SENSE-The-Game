#pragma once

#include <SDL.h>
#include <SDL_image.h>  // For image loading
#include <iostream>     // For console output
#include "window/window.h"
#include <string>
#include "objects/TextureInterface.h"

class TextureInterface {
protected:
    SDL_Texture* texture = nullptr;

public:
    TextureInterface(const std::string& texturePath) {
        texture = IMG_LoadTexture(window.renderer, texturePath.c_str());
        if (!texture) {
            SDL_Log("Failed to load texture: %s", SDL_GetError());
        }
    }

    ~TextureInterface() {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }

    // Render the road
    void render(int Y, float scale) {
        if (!texture) { return; }

        // Using a function to calculate texture size relative to the window
        SDL_Rect dstRect = getScaledRect(texture, window.window, scale, -Y);

        // Render the texture
        SDL_RenderCopy(window.renderer, texture, nullptr, &dstRect);
    }
};
