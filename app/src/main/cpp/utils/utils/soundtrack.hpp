#pragma once

#include <SDL.h>

class Soundtrack {
public:
    Soundtrack();
    ~Soundtrack();
    void play();
    void stop();
    void reset();

private:
    SDL_AudioSpec audioSpec;
    Uint8* audioBuffer;
    Uint32 audioLength;
    Uint8* audioPos;
    Uint32 currentLength;
    bool isPlaying;

    void loadWAV();

    static void audioCallback(void* userdata, Uint8* stream, int len);
};

#ifdef DEFINE_SOUNDTRACK
Soundtrack soundtrack;
#else
extern Soundtrack soundtrack;
#endif
