#pragma once

#include <application/window.hpp>
#include <SDL.h>


class Renderer {
public:
    explicit Renderer(const Window& window);
    virtual ~Renderer();

    [[nodiscard]] bool isInit() const;
    [[nodiscard]] SDL_Renderer *getSdlRenderer() const;
    void setDrawColor(const SDL_Color& color) const;
    void setViewport (const SDL_Color& color) const;
    void clear() const;
    void present() const;

    Renderer(const Renderer&) = delete;
    Renderer(Renderer&&) = delete;
    Renderer& operator=(const Renderer&) = delete;
    Renderer& operator=(Renderer&&) = delete;

private:

    SDL_Renderer *m_sdlRenderer;
    bool m_isInit;

    static const int s_driverIdx = -1;
    static const int s_flags =
        SDL_RENDERER_ACCELERATED |
        SDL_RENDERER_PRESENTVSYNC;
};
