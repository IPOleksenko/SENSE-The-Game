#include <application/renderer.hpp>

Renderer::Renderer(const Window& window) :
    m_sdlRenderer(SDL_CreateRenderer(
        window.getSdlWindow(),
        s_driverIdx,
        s_flags
    )),
    m_isInit(m_sdlRenderer != nullptr)
{
    if(!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "SDL_CreateRenderer", SDL_GetError()
        );
        return;
    }

    const SDL_Point& size = window.getSize();
    m_isInit = (SDL_RenderSetLogicalSize(m_sdlRenderer, size.x, size.y) == 0);

    if(!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "SDL_RenderSetLogicalSize", SDL_GetError()
        );
    }
}

Renderer::~Renderer() {
    if(m_isInit) {
        SDL_DestroyRenderer(m_sdlRenderer);
        m_sdlRenderer = nullptr;
        m_isInit = false;
    }
}

bool Renderer::isInit() const {
    return m_isInit;
}

SDL_Renderer *Renderer::getSdlRenderer() const {
    return m_sdlRenderer;
}

void Renderer::setDrawColor(const SDL_Color& color) const {
    if(!m_isInit) {
        return;
    }

    int err = SDL_SetRenderDrawColor(
        m_sdlRenderer,
        color.r,
        color.g,
        color.b,
        color.a
    );

    if(err != 0) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "SDL_SetRenderDrawColor", SDL_GetError()
        );
    }
}

void Renderer::clear() const {
    int err = SDL_RenderClear(m_sdlRenderer);

    if(err != 0) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "SDL_RenderClear", SDL_GetError()
        );
    }
}

void Renderer::present() const {
    SDL_RenderPresent(m_sdlRenderer);
}
