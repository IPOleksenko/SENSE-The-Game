#include <objects/endRender.hpp>
#include <window/window.hpp>
#include <assets/assets.hpp>
#include <SDL_image.h>
#include <chrono>


EndRender::EndRender() {
    Window& window = Window::getInstance();

    texture = IMG_LoadTexture_RW(
        window.renderer,
        SDL_Incbin(SPRITE_COLOR_SCREEN_BLACK_PNG),
        SDL_TRUE
    );
    if (!texture) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
    }
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND); // Enable alpha blending
}

EndRender::~EndRender() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}

void EndRender::render(int Y) {
    if (!texture) { return; }

    Window& window = Window::getInstance();

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
    SDL_Rect dest_rect = { 0, 0, Window::baseWidth, window.baseHeight };

    // Render the texture
    SDL_RenderCopy(window.renderer, texture, nullptr, &dest_rect);
}
