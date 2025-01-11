#pragma once

#include <SDL.h>
#include <SDL_mixer.h>

class Waveform;


class AudioManager {
public:
    explicit AudioManager();
    virtual ~AudioManager();

    [[nodiscard]] bool isInit() const;

    void play(const Waveform& audio, const int& times = 1) const;
    void stop() const;
    void resume() const;
    void pause() const;

    AudioManager(const AudioManager&) = delete;
    AudioManager(AudioManager&&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;
    AudioManager& operator=(AudioManager&&) = delete;

    static const int TIMES_LOOP;

private:
    bool m_isInit;

    static const int s_frequency = 44100;
    static const Uint16 s_format = MIX_DEFAULT_FORMAT;
    static const int s_channels = 2;
    static const int s_chunkSize = 4096;
};
