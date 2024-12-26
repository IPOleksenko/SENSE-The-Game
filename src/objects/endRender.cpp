#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <chrono>
#include "window/window.cpp"

class EndRender {
private:
    SDL_Texture* texture = nullptr;
    int alpha = 0; // Initial transparency (fully transparent image)

public:
    EndRender() {
        texture = IMG_LoadTexture(window.renderer, "assets/sprite/color_screen/black.png");
        if (!texture) {
            SDL_Log("Failed to load texture: %s", SDL_GetError());
        }
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND); // Enable alpha blending
    }

    ~EndRender() {
        if (texture) {
            SDL_DestroyTexture(texture);
        }
    }

    void render(int Y) {
        if (!texture) { return; }

        // Start time of rendering
        static auto start_time = std::chrono::high_resolution_clock::now();

        // Current time
        auto current_time = std::chrono::high_resolution_clock::now();
        auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();

        // Calculate alpha channel based on elapsed time (3 seconds = 3000 milliseconds)
        if (elapsed_time < 3000) {
            alpha = static_cast<int>((elapsed_time / 3000.0) * 255);
        }
        else {
            alpha = 255; // Fully opaque image
        }

        SDL_SetTextureAlphaMod(texture, alpha); // Set alpha value

        // Define the position and size for rendering
        SDL_Rect dest_rect = { 0, 0, window.BASE_WIDTH, window.BASE_HEIGHT };

        // Render the texture
        SDL_RenderCopy(window.renderer, texture, nullptr, &dest_rect);
    }
};
