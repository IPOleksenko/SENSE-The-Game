#include <utils/waveform.hpp>


Waveform::Waveform(SDL_RWops *data) :
    m_music(
        Mix_LoadMUSType_RW(data, MUS_WAV, SDL_TRUE),
        Mix_FreeMusic
    ),
    m_isInit(m_music != nullptr)
{
    if(!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "SDL_LoadWAV_RW", SDL_GetError()
        );
    }
}

bool Waveform::isInit() const {
    return m_isInit;
}

Mix_Music* Waveform::getSdlMusic() const {
    return m_music.get();
}
