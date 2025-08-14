#pragma once

#include <utils/texture.hpp>
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class Text {
public:
    explicit Text(
        SDL_Renderer* renderer,
        const int& fontSize,
        const SDL_Point& defaultPos,
        const int& animationDuration = 2000
    );
    ~Text();

    void setText(const std::string& text);
    void setLocalizedText(const std::string& key);
    void loadCustomFont(const std::string& path = "");
    void animationStart(const bool& fadeIn);
    void animationStop();
    void positionReset();
    void positionCenter();
    void resize(const int& fontSize);
    void render(const SDL_Point& areaSize);

private:
    TTF_Font* m_sdlFont;
    SDL_Renderer* m_sdlRenderer;
    bool m_isInit;

    std::string m_text;
    const SDL_Point m_pos;
    Uint8 m_alpha;
    bool m_isAnimated;
    bool m_fadeIn;
    bool m_isCentered;
    Uint32 m_animationStart;
    const Uint32 m_animationDuration;
};
