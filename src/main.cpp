#define DEFINE_WINDOW
#define DEFINE_SOUNDTRACK

#include <SDL.h>
#include <iostream>

const int finalCheckpoint = 25000;

#include "window/window.cpp"
#include "utils/SoundTrack.cpp"
#include "events/keydownEvent.cpp"
#include "events/playerKeydownEvent.cpp"
#include "objects/scale.cpp"
#include "objects/player.cpp"
#include "objects/road.cpp"
#include "objects/background.cpp"
#include "objects/text.cpp"
#include "objects/endRender.cpp"
#include "utils/camera.cpp"
#include "utils/updatePlayerText.cpp"
#include "objects/flora.cpp"

void showLoadingScreen() {
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
    backgroundGenerator background;
    Camera camera;
    ScaleGenerator scale;
    TextRenderer text(0, 0, 24);
    EndRender end;
    Flora flora;

    soundtrack.play();

    // Main game loop
    while (window.running) {
        // Event handling
        while (SDL_PollEvent(&window.event)) {
            if (window.event.type == SDL_QUIT) {
                window.running = false;  // Exit application when the window is closed
            }

            keyEvent.keydown();  // Handle key press events
            playerkeyEvent.keydown(player);
        }

        SDL_RenderSetLogicalSize(window.renderer, window.BASE_WIDTH, window.BASE_HEIGHT);

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

        if (player.getPlayerY() >= finalCheckpoint) {
            end.render(player.getPlayerY());  // Render the end game screen if the player reaches the final checkpoint
        }

        // Render text
        updatePlayerText(player, text);
        text.render();

        std::cout << "Player Y: " << player.getPlayerY() << std::endl; // Print the player's Y-coordinate

        camera.move();  // Move the camera

        // Present the rendered frame
        SDL_RenderPresent(window.renderer);
        SDL_Delay(16);  // Delay for approximately 16 milliseconds
    }

    return 0;
}
