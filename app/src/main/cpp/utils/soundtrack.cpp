#include <utils/soundtrack.hpp>
#include <assets/assets.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

Soundtrack::Soundtrack()
    : audioSpec{}
    , audioBuffer(nullptr)
    , audioLength(0)
    , audioPos(nullptr)
    , isPlaying(false) 
{
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        throw std::runtime_error("Failed to initialize SDL: " + std::string(SDL_GetError()));
    }

    loadWAV();

    // Set up audio spec with callback
    audioSpec.callback = audioCallback;
    audioSpec.userdata = this;

    if (SDL_OpenAudio(&audioSpec, nullptr) < 0) {
        throw std::runtime_error("Failed to open audio: " + std::string(SDL_GetError()));
    }
}

Soundtrack& Soundtrack::getInstance() {
    static Soundtrack instance = Soundtrack();

    return instance;
}


Soundtrack::~Soundtrack() {
    SDL_CloseAudio();
    SDL_FreeWAV(audioBuffer);
    SDL_Quit();
}

void Soundtrack::play() {
    if (!isPlaying) {
        reset();
        SDL_PauseAudio(0); // Start audio playback
        isPlaying = true;
    }
}

void Soundtrack::stop() {
    if (isPlaying) {
        SDL_PauseAudio(1); // Pause audio playback
        isPlaying = false;
    }
}

void Soundtrack::reset() {
    SDL_LockAudio();
    audioPos = audioBuffer;
    currentLength = audioLength;
    SDL_UnlockAudio();
}


void Soundtrack::loadWAV() {
    if (SDL_LoadWAV_RW(SDL_Incbin(SOUND_WIND_WAV), SDL_TRUE, &audioSpec, &audioBuffer, &audioLength) == nullptr) {
        throw std::runtime_error("Failed to load WAV file: " + std::string(SDL_GetError()));
    }
}

void Soundtrack::audioCallback(void* userdata, Uint8* stream, int len) {
    auto* audio = reinterpret_cast<Soundtrack*>(userdata);

    if (!audio->isPlaying || audio->currentLength == 0) {
        SDL_memset(stream, 0, len); // Fill stream with silence if not playing
        return;
    }

    len = (len > static_cast<int>(audio->currentLength)) ? audio->currentLength : len;
    SDL_memcpy(stream, audio->audioPos, len);
    audio->audioPos += len;
    audio->currentLength -= len;

    // Loop the audio
    if (audio->currentLength == 0) {
        audio->reset();
    }
}
