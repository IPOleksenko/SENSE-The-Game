#pragma once

#include <SDL.h>
#include <string>

class Icon;

class Window {
public:
    explicit Window(
        const SDL_Point& pos,
        const SDL_Point& size,
        const std::string& label
    );
    virtual ~Window();

    [[nodiscard]] bool isInit() const;
    [[nodiscard]] bool isFullscreen() const;
    [[nodiscard]] SDL_Window *getSdlWindow() const;
    [[nodiscard]] const SDL_Point& getSize() const;

    void setFullscreenOff();
    void setFullscreenOn();
    void setIcon(const Icon& icon);

    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) = delete;

private:
    void setFullscreen(const bool& fullscreen);

    SDL_Window *m_sdlWindow;
    bool m_isInit;
    bool m_isFullscreen;
    SDL_Point m_size;
    std::string m_label;

    static const int s_flags =
        SDL_WINDOW_SHOWN;
};
