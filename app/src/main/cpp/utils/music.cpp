#include <utils/music.hpp>

Music::Music(SDL_RWops *data) :
    m_SdlMusic(Mix_LoadMUS_RW(data, SDL_TRUE), Mix_FreeMusic),
    m_isInit(m_SdlMusic != nullptr)
{
    if(!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "SDL_LoadWAV_RW", SDL_GetError()
        );
    }
}

bool Music::isInit() const {
    return m_isInit;
}

Mix_Music* Music::getSdlMusic() const {
    return m_SdlMusic.get();
}
