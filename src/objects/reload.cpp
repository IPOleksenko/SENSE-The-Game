#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <chrono>

#include "../window/window.cpp"
#include "../utils/soundTrack.cpp"

class Reload {
private:
    SDL_Texture* texture = nullptr;
    SDL_AudioDeviceID audioDevice = 0;
    Uint8* soundBuffer = nullptr;
    Uint32 soundLength = 0;
    SDL_AudioSpec soundSpec;  // Structure for sound data

public:
    Reload() {
        // Load texture
        texture = IMG_LoadTexture(window.renderer, "assets/sprite/color_screen/black.png");
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
        if (SDL_LoadWAV("assets/sound/reload.wav", &soundSpec, &soundBuffer, &soundLength) == nullptr) {
            SDL_Log("Failed to load WAV: %s", SDL_GetError());
        }
    }

    ~Reload() {
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

    void render() {
        soundtrack.stop();

        // Define the position and size for rendering
        SDL_Rect dest_rect = { 0, 0, window.BASE_WIDTH, window.BASE_HEIGHT };

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
};
