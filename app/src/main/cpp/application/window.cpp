#include <application/window.hpp>
#include <utils/icon.hpp>


Window::Window(
    const SDL_Point& pos,
    const SDL_Point& size,
    const std::string& label
) :
    m_sdlWindow(SDL_CreateWindow(
        label.c_str(),
        pos.x, pos.y,
        size.x, size.y,
        s_flags
    )),
    m_isInit(m_sdlWindow != nullptr),
    m_isFullscreen(),
    m_size(size),
    m_label(label)
{
    if(!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "SDL_CreateWindow", SDL_GetError()
        );
    }
}

Window::~Window() {
    if(m_isInit) {
        SDL_DestroyWindow(m_sdlWindow);
        m_sdlWindow = nullptr;
        m_isInit = false;
    }
}

bool Window::isInit() const {
    return m_isInit;
}

bool Window::isFullscreen() const {
    return m_isFullscreen;
}

SDL_Window *Window::getSdlWindow() const {
    return m_sdlWindow;
}

const SDL_Point& Window::getSize() const {
    return m_size;
}

void Window::setFullscreenOff() {
    setFullscreen(false);
}

void Window::setFullscreenOn() {
    setFullscreen(true);
}

void Window::setFullscreen(const bool& fullscreen) {
    if(!m_isInit) {
        return;
    }

    int flags = 0;

    if(fullscreen) {
        flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    if(SDL_SetWindowFullscreen(m_sdlWindow, flags) != 0) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "SDL_SetWindowFullscreen", SDL_GetError()
        );
    }
    m_isFullscreen = fullscreen;
}

void Window::setIcon(const Icon& icon) {
    if(!m_isInit) {
        return;
    }

    if(icon.isInit()) {
        SDL_SetWindowIcon(m_sdlWindow, icon.getSdlSurface());
    }
}
