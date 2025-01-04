#define DEFINE_SOUNDTRACK

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
    Soundtrack& soundtrack = Soundtrack::getInstance();

    soundtrack.play();

    while (window.running) {
        // Event handling
        while (SDL_PollEvent(&window.event)) {
            if (window.event.type == SDL_QUIT) {
                window.running = false;  // Exit application when the window is closed
            }

            keyEvent.keydown();  // Handle key press events
            playerkeyEvent.keydown(player);
        }

        SDL_RenderSetLogicalSize(window.renderer, Window::baseWidth, window.baseHeight);

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

    return 0;
}
