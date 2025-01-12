#pragma once

#include <SDL.h>
#include <SDL_mixer.h>
#include <memory>

class Sfx {
public:
    explicit Sfx(SDL_RWops *data);
    virtual ~Sfx() = default;

    [[nodiscard]] bool isInit() const;
    [[nodiscard]] Mix_Chunk* getSdlChunk() const;

private:
    std::shared_ptr<Mix_Chunk> m_SdlChunk;
    bool m_isInit;
};
