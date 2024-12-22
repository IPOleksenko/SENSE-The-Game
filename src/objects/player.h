#pragma once

#ifndef finalCheckpoint_H
#define finalCheckpoint_H
extern const int finalCheckpoint;
#endif 

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "window/window.h"

class Player {
private:
    SDL_Texture* texture = nullptr;

    // Player parameters
    float playerY = 0.0f;
    float playerSpeed = 0.0f;

    int lastButton = NULL;
    bool isMoving = false;

    float playerSpeedScale = 5.0f;
    float playerSpeedDecrease = 0.5f;

    float playerMinSpeed = playerSpeedDecrease + 50.0f;
    float playerMaxSpeed = 150.0f;
    float playerNormalSpeed = (playerMaxSpeed + playerMinSpeed) / 2;

    // Animation parameters
    int frameWidth = 0;
    int frameHeight = 0;
    int currentFrame = 0;
    int totalFrames = 1; // Total number of frames in the animation
    int framesPerRow = 1; // Number of frames per row
    int animationSpeed = 100; // Animation speed in milliseconds
    Uint32 lastFrameTime = 0; // Time of the last frame switch

public:
    Player() {
        // Initializing SDL_image
        if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
            SDL_Log("Failed to initialize SDL_image: %s", IMG_GetError());
            return;
        }

        // Loading texture
        texture = IMG_LoadTexture(window.renderer, "assets/sprite/player/player.png");
        if (!texture) {
            SDL_Log("Failed to load texture: %s", SDL_GetError());
            return;
        }

        // Determining frame dimensions
        int textureWidth, textureHeight;
        SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);
        frameWidth = textureWidth / framesPerRow;
        frameHeight = textureHeight / (totalFrames / framesPerRow);
    }

    ~Player() {
        SDL_DestroyTexture(texture);
        IMG_Quit(); // Terminating SDL_image
    }

    void moving() {
        if (playerSpeed >= playerNormalSpeed)
            isMoving = true;

        if (isMoving) {
            playerY += 1;
            if (playerMinSpeed > playerSpeed || playerSpeed > playerMaxSpeed) {
                reset();
                std::cout << "You lost!" << std::endl;
            }
        }

        if (playerSpeed >= playerSpeedDecrease && playerY <= finalCheckpoint)
            playerSpeed -= playerSpeedDecrease;
    }

    void reset() {
        playerY = 0.0f;
        playerSpeed = 0.0f;
        lastButton = NULL;
        isMoving = false;
        currentFrame = 0;
    }

    void updateAnimation() {
        if (!isMoving) {
            currentFrame = 0; // Reset to the first frame if the player is not moving
            return;
        }

        Uint32 currentTime = SDL_GetTicks();
        if (currentTime > lastFrameTime + animationSpeed) {
            currentFrame = (currentFrame + 1) % totalFrames;
            lastFrameTime = currentTime;
        }
    }

    void render() {
        if (!texture) { return; }

        // Calculate the coordinates of the frame in the sprite sheet
        int row = currentFrame / framesPerRow;
        int col = currentFrame % framesPerRow;

        SDL_Rect srcRect = {
            col * frameWidth, // Start of the frame on X
            row * frameHeight, // Start of the frame on Y
            frameWidth, // Frame width
            frameHeight // Frame height
        };

        // Destination rectangle on the screen
        SDL_Rect dstRect = {
            (window.BASE_WIDTH - frameWidth) / 2, // Center on X
            (window.BASE_HEIGHT - frameHeight) / 2 - 10, // Center on Y minus 10 pixels
            frameWidth,
            frameHeight
        };

        // Rendering the current frame
        SDL_RenderCopy(window.renderer, texture, &srcRect, &dstRect);
    }

    void addPlayerSpeed() { playerSpeed += playerSpeedScale; }

    int getPlayerY() { return playerY; }
    int getPlayerMinSpeed() { return playerMinSpeed; }
    int getPlayerMaxSpeed() { return playerMaxSpeed; }
    int getPlayerSpeed() { return playerSpeed; }
};
