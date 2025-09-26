#include <utils/texture.hpp>
#include <SDL_image.h>


Texture::Texture(SDL_Texture* texture, SDL_Renderer* renderer) :
    m_sdlTexture(texture, SDL_DestroyTexture),
    m_isInit(m_sdlTexture != nullptr),
    m_sdlRenderer(renderer)
{}

bool Texture::isInit() const {
    return m_isInit;
}

SDL_Texture* Texture::getSdlTexture() const {
    return m_sdlTexture.get();
}

bool Texture::querySize(SDL_Point& size) const {
    if(!m_isInit) {
        return false;
    }

    int err = SDL_QueryTexture(
        m_sdlTexture.get(),
        nullptr,
        nullptr,
        &size.x,
        &size.y
    );

    if(err != 0) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "SDL_QueryTexture", SDL_GetError()
        );
        return false;
    }

    return true;
}

void Texture::render(const SDL_Rect* destRect, const SDL_Rect* srcRect) const {
    if(!m_isInit) {
        return;
    }

    int err = 0;

    err = SDL_RenderCopy(m_sdlRenderer, m_sdlTexture.get(), srcRect, destRect);

    if (err != 0) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "SDL_RenderCopy", SDL_GetError()
        );
    }
}

void Texture::tile(
        const SDL_Point& areaSize,
        const float& scale,
        const int& startY,
        const bool& isFullscreen
) const {
    SDL_Point textureSize{};
    if (!querySize(textureSize)) return;

    float targetWidth = areaSize.x * scale;
    float aspectRatio = static_cast<float>(textureSize.y) / textureSize.x;

    int scaledW = (static_cast<int>(targetWidth));
    int scaledH = (static_cast<int>(scaledW * aspectRatio));

    SDL_Rect destRect{};
    for (int y = startY; y < areaSize.y; y += scaledH) {
        destRect = { (areaSize.x - scaledW) / 2, y, scaledW, scaledH };
        render(&destRect, nullptr);
    }
}


RawTexture::RawTexture(SDL_RWops* data, SDL_Renderer* renderer) :
    Texture(IMG_LoadTexture_RW(renderer, data, SDL_TRUE), renderer)
{
    if (!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "IMG_LoadTexture_RW", SDL_GetError()
        );
    }
}

RawTexture::RawTexture(const char* path, SDL_Renderer* renderer) :
    Texture(IMG_LoadTexture(renderer, path), renderer)
{
    if (!m_isInit) {
        SDL_LogWarn(SDL_LOG_CATEGORY_APPLICATION, 
            "Failed to load texture from %s: %s", 
            path, SDL_GetError());
    }
}

void RawTexture::setAlpha(Uint8 alpha) {
    if (m_sdlTexture) {
        SDL_SetTextureAlphaMod(m_sdlTexture.get(), alpha);
    }
}

SurfaceTexture::SurfaceTexture(SDL_Surface* surface, SDL_Renderer* renderer) :
    Texture(SDL_CreateTextureFromSurface(renderer, surface), renderer)
{
    if (!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "SDL_CreateTextureFromSurface", SDL_GetError()
        );
    }
    SDL_FreeSurface(surface);
}
