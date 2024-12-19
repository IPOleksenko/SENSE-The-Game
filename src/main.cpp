#define DEFINE_WINDOW
#include <SDL.h>
#include <iostream>

#include "window/window.h"
#include "Events/keydownEvent.h"
#include "objects/player.h"
#include "objects/road.h"
#include "utils/camera.h"

int main(int argc, char* argv[]) {
    keydownEvent keyEvent;  // Object for handling key presses
    Player player;          // Creating the player object
    RoadGenerator road;
    Camera camera;
    // Main game loop
    while (window.running) {
        // Event handling
        while (SDL_PollEvent(&window.event)) {
            if (window.event.type == SDL_QUIT) {
                window.running = false;  // Exit application when the window is closed
            }

            keyEvent.keydown();  // Handle key press events
        }

        // Clear the screen
        SDL_SetRenderDrawColor(window.renderer, 0, 0, 0, 255);  // Set color to black
        SDL_RenderClear(window.renderer);
        player.moving();

        // Render the road
        road.render(player.getPlayerY());
        // Render the player
        player.render();

        camera.move();
        
        // Present the rendered frame
        SDL_RenderPresent(window.renderer);
        SDL_Delay(16);
    }

    return 0;
}
