#include <events/playerKeydownEvent.hpp>
#include <window/window.hpp>
#include <objects/player.hpp>


void PlayerKeydownEvent::keydown(Player& player) {
    Window& window = Window::getInstance();

    // Check if a key was released and the player's Y-coordinate is within the final checkpoint
    if (window.event.type == SDL_KEYUP && player.getPlayerY() <= Window::finalCheckpoint) {
        // If the 'A' or Left Arrow key is released, increase the player's speed
        if ((window.event.key.keysym.sym == SDLK_a || window.event.key.keysym.sym == SDLK_LEFT) && player.getPlayerLastButton() != SDL_SCANCODE_A) {
            player.addPlayerSpeed(SDL_SCANCODE_A);
        }
        // If the 'D' or Right Arrow key is released, increase the player's speed
        else if ((window.event.key.keysym.sym == SDLK_d || window.event.key.keysym.sym == SDLK_RIGHT) && player.getPlayerLastButton() != SDL_SCANCODE_D) {
            player.addPlayerSpeed(SDL_SCANCODE_D);
        }
    }
}
