#pragma once

#ifndef finalCheckpoint_H
#define finalCheckpoint_H
extern const int finalCheckpoint; // Declares an external constant representing the final checkpoint
#endif 

#include "window/window.cpp"
#include "objects/player.cpp"

class PlayerKeydownEvent {
public:
    // Handles keydown events related to the player
    void keydown(Player& player) {
        // Check if a key was released and the player's Y-coordinate is within the final checkpoint
        if (window.event.type == SDL_KEYUP && player.getPlayerY() <= finalCheckpoint) {
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
};
