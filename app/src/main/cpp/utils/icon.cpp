#include <utils/icon.hpp>

Icon::Icon(SDL_RWops* data) :
    m_sdlSurface(SDL_LoadBMP_RW(data, SDL_TRUE), SDL_FreeSurface),
    m_isInit(m_sdlSurface != nullptr)
{}

bool Icon::isInit() const {
    return m_isInit;
}

SDL_Surface* Icon::getSdlSurface() const {
    return m_sdlSurface.get();
}
