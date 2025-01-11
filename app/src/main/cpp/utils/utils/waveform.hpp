#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <memory>

class Waveform {
public:
    explicit Waveform(SDL_RWops *data);
    ~Waveform() = default;

    [[nodiscard]] bool isInit() const;
    [[nodiscard]] Mix_Music * getSdlMusic() const;

private:

    std::shared_ptr<Mix_Music> m_music;
    bool m_isInit;
};