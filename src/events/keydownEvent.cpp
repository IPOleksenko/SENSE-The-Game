#include <events/keydownEvent.hpp>
#include <window/window.hpp>
#include <SDL.h>


void keydownEvent::keydown() {
    if (window.event.type == SDL_KEYDOWN) {
        if (window.event.type == SDL_QUIT) {
            window.running = false; // Close the application when the window is closed
        }
        if (window.event.key.keysym.sym == SDLK_f) {
            // Toggle fullscreen mode
            if (isFullscreen) {
                SDL_SetWindowFullscreen(window.window, 0); // Windowed mode
                isFullscreen = false;
            }
            else {
                SDL_SetWindowFullscreen(window.window, SDL_WINDOW_FULLSCREEN_DESKTOP); // Fullscreen mode
                isFullscreen = true;
            }
        }
        else if (window.event.key.keysym.sym == SDLK_ESCAPE) {
            window.running = false; // Exit the application
        }
    }
}
