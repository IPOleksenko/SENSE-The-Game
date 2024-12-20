#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "window/window.h"
#include "utils/scaledTextureSize.h"

class Player {
private:
    SDL_Texture* texture = nullptr;

    float playerY = 0.0f;
    float playerSpeed = 0.0f;

    int lastButton = NULL;
    bool isMoving = false;

    float playerSpeedScale = 5.0f;
    float playerSpeedDecrease = 0.5f;

    float playerMinSpeed = 50.0f;
    float playerMaxSpeed = 150.0f;
    float playerNormalSpeed = (playerMaxSpeed + playerMinSpeed) / 2;

public:
    Player() {
        // Initialization of SDL_image
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            SDL_Log("Failed to initialize SDL_image: %s", IMG_GetError());
            return;
        }

        // Loading texture
        texture = IMG_LoadTexture(window.renderer, "assets/sprite/player/player.png");
        if (!texture) {
            SDL_Log("Failed to load texture: %s", SDL_GetError());
        }
    }

    ~Player() {
        SDL_DestroyTexture(texture);
        IMG_Quit(); // Terminating SDL_image
    }

    void moving() {
        const Uint8* state = SDL_GetKeyboardState(nullptr);
        if (state[SDL_SCANCODE_A] && lastButton != SDL_SCANCODE_A) {
            lastButton = SDL_SCANCODE_A;
            playerSpeed += playerSpeedScale;
        }
        else if (state[SDL_SCANCODE_D] && lastButton != SDL_SCANCODE_D) {
            lastButton = SDL_SCANCODE_D;
            playerSpeed += 1;
        }

        if (playerSpeed >= playerNormalSpeed)
            isMoving = true;

        if (isMoving) {
            playerY += 1;
            if (playerMinSpeed > playerSpeed || playerSpeed > playerMaxSpeed) {
                reset();
                std::cout << "You die!" << std::endl;
            }
        }

        if (playerSpeed >= playerSpeedDecrease)
            playerSpeed -= playerSpeedDecrease;
    }

    void reset() {
        playerY = 0.0f;
        playerSpeed = 0.0f;
        lastButton = NULL;
        isMoving = false;
    }

    void render() {
        if (!texture) { return; }

        // Using a function to calculate texture size relative to the window
        SDL_Rect dstRect = getScaledRect(texture, window.window, 0.2, 0);

        // Rendering texture
        SDL_RenderCopy(window.renderer, texture, nullptr, &dstRect);
    }
    int getPlayerY() { return playerY; }
    int getPlayerMinSpeed() { return playerMinSpeed; }
    int getPlayerMaxSpeed() { return playerMaxSpeed; }
    int getPlayerSpeed() { return playerSpeed; }
};
