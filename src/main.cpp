#define DEFINE_WINDOW
#include <SDL.h>
#include <iostream>

#include "window/window.h"
#include "Events/keydownEvent.h"

int main(int argc, char* argv[]) {
    keydownEvent keyEvent; // Create an object to handle keyboard events

    while (window.running) {
        if (window.event.type == SDL_QUIT) {
                window.running = false; // Close the application when the window is closed
            }
        while (SDL_PollEvent(&window.event)) {
            keyEvent.keydown(); // Handle key presses
        }

        // Clear the screen
        SDL_SetRenderDrawColor(window.renderer, 0, 0, 0, 255); // Black color
        SDL_RenderClear(window.renderer);

        // Display the frame on the screen
        SDL_RenderPresent(window.renderer);
    }

    return 0;
}
