#pragma once

#include <SDL.h>

class Soundtrack {
public:
    static Soundtrack& getInstance();
    ~Soundtrack();
    void play();
    void stop();
    void reset();

private:
    Soundtrack();
    SDL_AudioSpec audioSpec;
    Uint8* audioBuffer;
    Uint32 audioLength;
    Uint8* audioPos;
    Uint32 currentLength;
    bool isPlaying;

    void loadWAV();

    static void audioCallback(void* userdata, Uint8* stream, int len);
};
