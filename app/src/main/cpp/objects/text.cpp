#include <objects/text.hpp>
#include <utils/texture.hpp>
#include <utils/localization.hpp>
#include <utils/modding.hpp>
#include <SDL_ttf.h>
#include <vector>
#include <sstream>


Text::Text(
    SDL_Renderer* renderer,
    const int& fontSize,
    const SDL_Point& pos,
    bool forceDefaultFont,
    const int& animationDuration
) :
    m_sdlFont(nullptr),
    m_sdlRenderer(renderer),
    m_isInit(false),
    m_text(),
    m_pos(pos),
    m_alpha(SDL_ALPHA_OPAQUE),
    m_isAnimated(),
    m_fadeIn(),
    m_positionMode(),
    m_animationStart(),
    m_animationDuration(animationDuration),
    m_color{ 255, 255, 255, 255 }
{
    if(forceDefaultFont) {
        m_sdlFont = TTF_OpenFontRW(SDL_Incbin(FONT_FONT_TTF), SDL_TRUE, fontSize);
        m_isInit = m_sdlFont != nullptr;
        if(!m_isInit) {
            SDL_LogCritical(SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s", "TTF_OpenFontRW", SDL_GetError());
        }
        else {
            TTF_SetFontHinting(m_sdlFont, TTF_HINTING_LIGHT);
        }
        return;
    }

    customFontPath = modding::loadCustomFontPath();
    if (!customFontPath.empty()) {
        m_sdlFont = TTF_OpenFont(customFontPath.c_str(), fontSize);
        if (m_sdlFont) {
            TTF_SetFontHinting(m_sdlFont, TTF_HINTING_LIGHT);
            m_isInit = true;
            return;
        }
    }

    m_sdlFont = TTF_OpenFontRW(SDL_Incbin(FONT_FONT_TTF), SDL_TRUE, fontSize);
    m_isInit = m_sdlFont != nullptr;

    if (!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "TTF_OpenFontRW", SDL_GetError()
        );
    } else {
        TTF_SetFontHinting(m_sdlFont, TTF_HINTING_LIGHT);
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

void Text::setColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    m_color = { r, g, b, a };
}

void Text::setLocalizedText(const std::string& key) {
    std::string localizedText = LocalizationManager::instance().getText(key);
    if (!localizedText.empty()) {
        m_text = localizedText;
    }
}

void Text::loadCustomFont(const std::string& path) {
    if (m_isInit) {
        TTF_CloseFont(m_sdlFont);
        m_sdlFont = nullptr;
        m_isInit = false;
    }

    std::string fontPath = path;
    if (fontPath.empty()) {
        fontPath = modding::loadCustomFontPath();
    }

    int fontSize = m_sdlFont ? TTF_FontHeight(m_sdlFont) : 25;

    if (!fontPath.empty() && modding::fileExists(fontPath)) {
        m_sdlFont = TTF_OpenFont(fontPath.c_str(), fontSize);
    } else {
        m_sdlFont = TTF_OpenFontRW(SDL_Incbin(FONT_FONT_TTF), SDL_TRUE, fontSize);
    }

    m_isInit = m_sdlFont != nullptr;
    if (!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "TTF_OpenFont", SDL_GetError()
        );
    }
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

int Text::getAnimationDuration() const {
    return m_animationDuration;
}

void Text::positionReset() {
    m_positionMode = PositionMode::Default;
}

void Text::positionCenter() {
    m_positionMode = PositionMode::Center;
}

void Text::positionTopRight() {
    m_positionMode = PositionMode::TopRight;
}

void Text::positionTopCenter() {
    m_positionMode = PositionMode::TopCenter;
}

void Text::resize(const int& fontSize)
{
    if (m_sdlFont) {
        TTF_CloseFont(m_sdlFont);
        m_sdlFont = nullptr;
        m_isInit = false;
    }

    if (!customFontPath.empty() && modding::fileExists(customFontPath)) {
        m_sdlFont = TTF_OpenFont(customFontPath.c_str(), fontSize);
    }
    else {
        m_sdlFont = TTF_OpenFontRW(SDL_Incbin(FONT_FONT_TTF), SDL_TRUE, fontSize);
    }

    m_isInit = m_sdlFont != nullptr;

    if (!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "TTF_OpenFont", SDL_GetError()
        );
    }
    else {
        TTF_SetFontHinting(m_sdlFont, TTF_HINTING_LIGHT);
    }
}


void Text::render(const SDL_Point& areaSize) {
    if (m_text.empty()) {
        return;
    }

    if (m_isAnimated) {
        const Uint32 currentTime = SDL_GetTicks();
        const float progress = SDL_min(
                static_cast<float>(currentTime - m_animationStart) /
                static_cast<float>(m_animationDuration), 1.0f
        );

        m_alpha = static_cast<Uint8>(
                (m_fadeIn ? progress : 1.0f - progress) * SDL_ALPHA_OPAQUE
        );
    } else {
        m_alpha = SDL_ALPHA_OPAQUE;
    }

    if (m_alpha == 0) {
        return;
    }

    std::istringstream stream(m_text);
    std::string line;
    std::vector<SurfaceTexture> textures;
    int totalHeight = 0;

    const float scaleX = static_cast<float>(areaSize.x) / 1280.0f;
    const float scaleY = static_cast<float>(areaSize.y) / 720.0f;

    while (std::getline(stream, line)) {
        if (line.empty()) continue;

        SDL_Color drawColor = m_color;
        drawColor.a = m_alpha;

        textures.emplace_back(
            TTF_RenderUTF8_Blended(
                m_sdlFont,
                line.c_str(),
                drawColor
            ),
            m_sdlRenderer
        );

        SDL_Point textureSize{};
        if (textures.back().querySize(textureSize)) {
            totalHeight += static_cast<int>(textureSize.y * scaleY);
        }
    }

    int posY;
    switch (m_positionMode) {
    case PositionMode::Center:
        posY = (areaSize.y - totalHeight) / 2;
        break;
    case PositionMode::TopCenter:
        posY = 0;
        break;
    default:
        posY = static_cast<int>(m_pos.y * scaleY);
        break;
    }

    for (const auto& tex : textures) {
        SDL_Point textureSize{};
        if (tex.querySize(textureSize)) {
            int posX;
            switch (m_positionMode) {
            case PositionMode::Center:
            case PositionMode::TopCenter:
                posX = (areaSize.x - static_cast<int>(textureSize.x * scaleX)) / 2;
                break;
            case PositionMode::TopRight:
                posX = areaSize.x - static_cast<int>(textureSize.x * scaleX);
                break;
            default:
                posX = static_cast<int>(m_pos.x * scaleX);
                break;
            }

            SDL_Rect destRect = {
                posX, posY,
                static_cast<int>(textureSize.x * scaleX),
                static_cast<int>(textureSize.y * scaleY)
            };

            tex.render(&destRect, nullptr);
            posY += destRect.h;
        }
    }
}
