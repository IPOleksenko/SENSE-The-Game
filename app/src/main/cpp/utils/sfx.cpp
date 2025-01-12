#include <utils/sfx.hpp>

Sfx::Sfx(SDL_RWops *data) :
    m_SdlChunk(Mix_LoadWAV_RW(data, SDL_TRUE), Mix_FreeChunk),
    m_isInit(m_SdlChunk != nullptr)
{
    if(!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "Mix_LoadWAV_RW", SDL_GetError()
        );
    }
}

bool Sfx::isInit() const {
    return m_isInit;
}

Mix_Chunk* Sfx::getSdlChunk() const {
    return m_SdlChunk.get();
}
