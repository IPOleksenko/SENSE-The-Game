#include <objects/text.hpp>
#include "../utils/utils/texture.hpp"
#include <SDL_ttf.h>
#include <vector>
#include <sstream>


Text::Text(
    SDL_Renderer* renderer,
    const int& fontSize,
    const SDL_Point& pos,
    const int& animationDuration
) :
    m_sdlFont(TTF_OpenFontRW(SDL_Incbin(FONT_FONT_TTF), SDL_TRUE, fontSize)),
    m_sdlRenderer(renderer),
    m_isInit(m_sdlFont != nullptr),
    m_text(),
    m_pos(pos),
    m_alpha(SDL_ALPHA_OPAQUE),
    m_isAnimated(),
    m_fadeIn(),
    m_isCentered(),
    m_animationStart(),
    m_animationDuration(animationDuration)
{
    if (!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "TTF_OpenFontRW", SDL_GetError()
        );
    }
}

Text::~Text() {
    if(m_isInit) {
        TTF_CloseFont(m_sdlFont);
        m_sdlFont = nullptr;
        m_isInit = false;
    }
}

void Text::setText(const std::string& text) {
    m_text = text;
}

void Text::animationStart(const bool& fadeIn) {
    m_isAnimated = true;
    m_fadeIn = fadeIn;
    m_animationStart = SDL_GetTicks();
}

void Text::animationStop() {
    m_isAnimated = false;
    m_alpha = SDL_ALPHA_OPAQUE;
    m_animationStart = 0;
}

void Text::positionReset() {
    m_isCentered = false;
}

void Text::positionCenter() {
    m_isCentered = true;
}

void Text::render(const SDL_Point& areaSize) {
    if(m_text.empty()) {
        return;
    }

    if (m_isAnimated) {
        const Uint32 currentTime = SDL_GetTicks();
        const float progress = SDL_min(
            static_cast<float>(currentTime - m_animationStart) /
            static_cast<float>(m_animationDuration), 1.0f
        );

        m_alpha = static_cast<Uint8>(
            (m_fadeIn ? progress : 1.0f - progress) *
            SDL_ALPHA_OPAQUE
        );
    }
    else {
        m_alpha = SDL_ALPHA_OPAQUE;
    }

    if (m_alpha == 0) {
        return;
    }

    std::istringstream stream(m_text);
    std::string line;

    std::vector<SurfaceTexture> textures = {};
    int offsetY = 0;

    while (std::getline(stream, line)) {
        textures.emplace_back(
            TTF_RenderText_Blended(
                m_sdlFont,
                line.c_str(),
                { 0xFF, 0xFF, 0xFF, m_alpha }
            ),
            m_sdlRenderer
        );

        SDL_Point textureSize = {};
        if(textures.back().querySize(textureSize)) {
            offsetY += textureSize.y;
        }
    }

    int posY = m_pos.y;
    if(m_isCentered) {
        posY = (areaSize.y - offsetY) / 2;
    }

    for(const auto& tex: textures) {
        SDL_Point textureSize = {};

        if(tex.querySize(textureSize)) {
            int posX = m_pos.x;
            if(m_isCentered) {
                posX = (areaSize.x - textureSize.x) / 2;
            }

            const SDL_Rect destRect = {
                posX, posY,
                textureSize.x, textureSize.y
            };

            tex.render(&destRect, nullptr);
        }

        posY += textureSize.y;
    }
}
