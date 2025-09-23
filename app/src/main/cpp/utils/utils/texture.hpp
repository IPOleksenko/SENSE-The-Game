#pragma once

#include <assets/assets.hpp>
#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <memory>

class Texture {
public:
    explicit Texture(SDL_Texture* texture, SDL_Renderer* renderer);
    virtual ~Texture() = default;

    [[nodiscard]] bool isInit() const;
    [[nodiscard]] SDL_Texture* getSdlTexture() const;
    virtual bool querySize(SDL_Point& size) const;
    virtual void render(const SDL_Rect* destRect, const SDL_Rect* srcRect) const;

    virtual void tile(
        const SDL_Point& areaSize,
        const float& scale,
        const int& startY,
        const bool& isFullscreen
    ) const;

protected:
    std::shared_ptr<SDL_Texture> m_sdlTexture;
    bool m_isInit;
    SDL_Renderer* m_sdlRenderer;
};


class RawTexture : public Texture {
public:
    explicit RawTexture(SDL_RWops* data, SDL_Renderer* renderer);
    explicit RawTexture(const char* path, SDL_Renderer* renderer);
    RawTexture() : Texture(nullptr, nullptr) {}

    SDL_Texture* get() const { return m_sdlTexture.get(); }
    void setAlpha(Uint8 alpha);
};


class SurfaceTexture : public Texture {
public:
    explicit SurfaceTexture(SDL_Surface* surface, SDL_Renderer* renderer);
};
