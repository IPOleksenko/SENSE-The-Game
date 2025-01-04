#include <objects/reload.hpp>
#include <window/window.hpp>
#include <utils/soundtrack.hpp>
#include <assets/assets.hpp>
#include <SDL_image.h>


Reload::Reload() {
    Window& window = Window::getInstance();

    // Load texture
    texture = IMG_LoadTexture_RW(
        window.renderer,
        SDL_Incbin(SPRITE_COLOR_SCREEN_BLACK_PNG),
        SDL_TRUE
    );
    if (!texture) {
        SDL_Log("Failed to load texture: %s", SDL_GetError());
    }
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND); // Enable alpha blending

    // Initialize SDL audio system
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        SDL_Log("SDL could not initialize! SDL Error: %s", SDL_GetError());
    }

    // Open audio device
    SDL_AudioSpec desiredSpec;
    SDL_zero(desiredSpec);
    desiredSpec.freq = 44100;
    desiredSpec.format = AUDIO_S16LSB;  // 16-bit little-endian
    desiredSpec.channels = 2;           // Stereo
    desiredSpec.samples = 4096;
    desiredSpec.callback = nullptr;

    audioDevice = SDL_OpenAudioDevice(nullptr, 0, &desiredSpec, nullptr, 0);
    if (audioDevice == 0) {
        SDL_Log("Failed to open audio device: %s", SDL_GetError());
    }

    // Load sound into memory (WAV file)
    if (SDL_LoadWAV_RW(SDL_Incbin(SOUND_RELOAD_WAV), SDL_TRUE, &soundSpec, &soundBuffer, &soundLength) == nullptr) {
        SDL_Log("Failed to load WAV: %s", SDL_GetError());
    }
}

Reload::~Reload() {
    if (texture) {
        SDL_DestroyTexture(texture);
    }

    if (soundBuffer) {
        SDL_FreeWAV(soundBuffer);
    }

    if (audioDevice) {
        SDL_CloseAudioDevice(audioDevice);
    }

    SDL_Quit();
}

void Reload::render() {
    Window& window = Window::getInstance();

    soundtrack.stop();

    // Define the position and size for rendering
    SDL_Rect dest_rect = { 0, 0, Window::baseWidth, window.baseHeight };

    // Render the texture
    SDL_RenderCopy(window.renderer, texture, nullptr, &dest_rect);
    SDL_RenderPresent(window.renderer); // Present the rendered frame to the screen

    if (soundBuffer) {
        // Queue the audio data and play
        SDL_QueueAudio(audioDevice, soundBuffer, soundLength);
        SDL_PauseAudioDevice(audioDevice, 0);  // Start audio playback

        // Calculate the duration of the sound in milliseconds
        // Duration = (samples / frequency) * 1000
        float durationSec = static_cast<float>(soundLength) / (soundSpec.freq * soundSpec.channels * (SDL_AUDIO_BITSIZE(soundSpec.format) / 8));
        int delayMs = static_cast<int>(durationSec * 1000);  // Convert to milliseconds

        // Wait until the sound finishes playing
        SDL_Delay(delayMs);  // Delay matches the sound length
    }
    else {
        SDL_Delay(100); // Default delay if sound fails to load
    }
    soundtrack.play();
}
