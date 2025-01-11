#include <application/audio.hpp>
#include <utils/waveform.hpp>
#include <limits>


const int AudioManager::TIMES_LOOP = std::numeric_limits<int>::max();

AudioManager::AudioManager() :
    m_isInit(Mix_OpenAudio(
        s_frequency,
        s_format,
        s_channels,
        s_chunkSize
    ) == 0)
{
    if(!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "Mix_OpenAudio", SDL_GetError()
        );
    }
}

AudioManager::~AudioManager() {
    if(m_isInit) {
        Mix_CloseAudio();
        m_isInit = false;
    }

}

bool AudioManager::isInit() const {
    return m_isInit;
}


void AudioManager::play(const Waveform& audio, const int& times) const {
    if(!m_isInit) {
        return;
    }

    if(Mix_PlayMusic(audio.getSdlMusic(), times) != 0) {
        SDL_LogWarn(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "Mix_PlayMusic", SDL_GetError()
        );
    }
}

void AudioManager::stop() const {
    if(!m_isInit) {
        return;
    }

    Mix_HaltMusic();
}

void AudioManager::resume() const {
    if(!m_isInit) {
        return;
    }

    if(Mix_PausedMusic() == SDL_TRUE) {
        Mix_ResumeMusic();
    }
}

void AudioManager::pause() const {
    if(!m_isInit) {
        return;
    }

    if(Mix_PlayingMusic() == SDL_TRUE) {
        Mix_PauseMusic();
    }
}
