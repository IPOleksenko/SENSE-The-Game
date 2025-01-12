#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <memory>

class Music {
public:
    explicit Music(SDL_RWops *data);
    virtual ~Music() = default;

    [[nodiscard]] bool isInit() const;
    [[nodiscard]] Mix_Music* getSdlMusic() const;

private:
    std::shared_ptr<Mix_Music> m_SdlMusic;
    bool m_isInit;
};