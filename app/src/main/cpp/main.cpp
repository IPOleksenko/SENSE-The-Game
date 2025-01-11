#include <cstdlib>
#include <application/game.hpp>


int main(int, char*[]) {
    Game game = Game();

    if(game.isInit()) {
        game.run();
    }

    return EXIT_SUCCESS;
}

/*
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <assets/assets.hpp>
#include <cstdlib>


namespace SdlConfig {
    constexpr int rendererIndex = -1;
    constexpr int rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
    constexpr char orientation[] = "Landscape";
}

#define SDL_RENDERER_FLAGS ()

int main(int, char*[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_SetHint(SDL_HINT_ORIENTATIONS, SdlConfig::orientation);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);


    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;


    window = SDL_CreateWindow(
        "An SDL2 window",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        1280,
        720,
        SDL_WINDOW_OPENGL
    );

    renderer = SDL_CreateRenderer(
        window,
        SdlConfig::rendererIndex,
        SdlConfig::rendererFlags
    );

    SDL_SetRenderDrawColor(renderer, 0, 255, 127, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_AudioSpec wavSpec = {};
    Uint32 wavLength = 0;
    Uint8 *wavBuffer = nullptr;

    SDL_LoadWAV_RW(
        SDL_Incbin(SOUND_WIND_WAV),
        SDL_TRUE,
        &wavSpec,
        &wavBuffer,
        &wavLength
    );

    SDL_AudioDeviceID deviceId = 0;

    deviceId = SDL_OpenAudioDevice(
        nullptr, 0, &wavSpec, nullptr, 0
    );

    SDL_QueueAudio(deviceId, wavBuffer, wavLength);
    SDL_PauseAudioDevice(deviceId, SDL_FALSE);



//    SDL_Surface *loadedImage = IMG_Load_RW(SDL_Incbin(SPRITE_PLAYER_PLAYER_PNG), SDL_TRUE);
//    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, loadedImage);
//    SDL_FreeSurface(loadedImage);
//
//    SDL_RenderCopy(renderer, texture, NULL, &r);
//    SDL_RenderPresent(renderer);


    SDL_Delay(20000);


    SDL_FreeWAV(wavBuffer);
    SDL_CloseAudioDevice(deviceId);


    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);


    return EXIT_SUCCESS;
}
*/

/*

#include <window/window.hpp>
#include <utils/camera.hpp>
#include <utils/soundtrack.hpp>
#include <objects/scale.hpp>
#include <objects/player.hpp>
#include <objects/road.hpp>
#include <objects/background.hpp>
#include <objects/text.hpp>
#include <objects/endRender.hpp>
#include <objects/flora.hpp>
#include <events/keydownEvent.hpp>
#include <events/playerKeydownEvent.hpp>

#include <SDL.h>
#include <cstdlib>
#include <iostream>


void showLoadingScreen() {
    Window& window = Window::getInstance();

    // Set background color to black
    SDL_SetRenderDrawColor(window.renderer, 0, 0, 0, 255);
    SDL_RenderClear(window.renderer);

    // Create loading text
    TextRenderer loadingText(0, 0, 48);
    loadingText.setText("Loading...");
    loadingText.centerTextOnScreen();

    // Render the loading text
    loadingText.render();
    SDL_RenderPresent(window.renderer);
}

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    SDL_SetHint(SDL_HINT_ORIENTATIONS, "LandscapeLeft");

    showLoadingScreen();

    keydownEvent keyEvent;  // Object for handling key presses
    PlayerKeydownEvent playerkeyEvent;
    Player player;          // Creating the player object
    RoadGenerator road;
    BackgroundGenerator background;
    Camera camera;
    ScaleGenerator scale;
    TextRenderer text(0, 0, 24);
    EndRender end;
    Flora flora;


    // Main game loop
    Window& window = Window::getInstance();
//    Soundtrack& soundtrack = Soundtrack::getInstance();
//
//    soundtrack.play();

    while (window.running) {
        // Event handling
        while (SDL_PollEvent(&window.event)) {
            if (window.event.type == SDL_QUIT) {
                window.running = false;  // Exit application when the window is closed
            }

            keyEvent.keydown();  // Handle key press events
            playerkeyEvent.keydown(player);
        }

        SDL_RenderSetLogicalSize(window.renderer, Window::baseWidth, Window::baseHeight);

        // Clear the screen
        SDL_SetRenderDrawColor(window.renderer, 0, 0, 0, 255);  // Set color to black
        SDL_RenderClear(window.renderer);
        player.moving();

        background.render(player.getPlayerY()); // Render the background based on the player's position
        flora.render(player.getPlayerY());
        road.render(player.getPlayerY());      // Render the road based on the player's position
        player.updateAnimation();             // Update player animation
        player.render();                      // Render the player

        scale.render(player.getPlayerSpeed(), player.getPlayerMinSpeed(), player.getPlayerMaxSpeed()); // Render the speed scale

        if (player.getPlayerY() >= Window::finalCheckpoint) {
            end.render(player.getPlayerY());  // Render the end game screen if the player reaches the final checkpoint
        }

        // Render text
        text.update(player.getPlayerY());
        text.render();

        camera.move();  // Move the camera

        // Present the rendered frame
        SDL_RenderPresent(window.renderer);
        SDL_Delay(16);  // Delay for approximately 16 milliseconds
    }

    return EXIT_SUCCESS;
}

*/