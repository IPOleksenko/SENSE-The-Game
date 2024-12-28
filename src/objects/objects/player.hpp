#pragma once

#include <objects/reload.hpp>
#include <SDL.h>


class Player {
private:
    Reload reload;

    SDL_Texture* texture = nullptr;

    // Player parameters
    float playerY = 0.0f;
    float playerSpeed = 0.0f;

    int lastButton = 0;
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
    int totalFrames = 120; // Total number of frames in the animation
    int framesPerRow = 20; // Number of frames per row
    int animationSpeed = 100; // Animation speed in milliseconds
    Uint32 lastFrameTime = 0; // Time of the last frame switch

public:
    Player();
    ~Player();

    void moving();
    void reset();
    void updateAnimation();
    void render();
    void addPlayerSpeed(int button);

    int getPlayerY()            { return playerY; }
    int getPlayerMinSpeed()     { return playerMinSpeed; }
    int getPlayerMaxSpeed()     { return playerMaxSpeed; }
    int getPlayerSpeed()        { return playerSpeed; }
    int getPlayerLastButton()   { return lastButton; }
};
