#pragma once

#include <SDL.h>


class Reload {
private:
    SDL_Texture* texture = nullptr;
    SDL_AudioDeviceID audioDevice = 0;
    Uint8* soundBuffer = nullptr;
    Uint32 soundLength = 0;
    SDL_AudioSpec soundSpec;  // Structure for sound data

public:
    Reload();
    ~Reload();

    void render();
};
