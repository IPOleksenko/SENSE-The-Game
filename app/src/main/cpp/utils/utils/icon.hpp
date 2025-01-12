#pragma once

#include <assets/assets.hpp>
#include <SDL2/SDL.h>
#include <memory>

class Icon {
public:
    explicit Icon(SDL_RWops* data);
    virtual ~Icon() = default;

    [[nodiscard]] bool isInit() const;
    [[nodiscard]] SDL_Surface* getSdlSurface() const;

protected:
    std::shared_ptr<SDL_Surface> m_sdlSurface;
    bool m_isInit;
};
