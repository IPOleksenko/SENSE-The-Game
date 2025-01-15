#include <objects/end.hpp>
#include <assets/assets.hpp>
#include <chrono>

End::End(SDL_Renderer *renderer) :
    m_texture(SDL_Incbin(SPRITE_COLOR_SCREEN_BLACK_PNG), renderer),
    m_alpha()
{
    SDL_SetTextureBlendMode(m_texture.getSdlTexture(), SDL_BLENDMODE_BLEND);
}

void End::render(const SDL_Point& areaSize) {
    if (!m_texture.isInit()) {
        return;
    }

    // Start time of rendering
    static auto start_time = std::chrono::high_resolution_clock::now();

    // Current time
    auto current_time = std::chrono::high_resolution_clock::now();
    auto elapsed_time = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - start_time).count();

    // Calculate alpha channel based on elapsed time (3 seconds = 3000 milliseconds)
    if (elapsed_time < 3000) {
        m_alpha = static_cast<int>((elapsed_time / 3000.0) * SDL_ALPHA_OPAQUE);
    }
    else {
        m_alpha = SDL_ALPHA_OPAQUE; // Fully opaque image
    }

    SDL_SetTextureAlphaMod(m_texture.getSdlTexture(), m_alpha); // Set alpha value

    // Define the position and size for rendering
    SDL_Rect dest_rect = { 0, 0, areaSize.x, areaSize.y };

    // Render the texture
    m_texture.render(&dest_rect, nullptr);
}
