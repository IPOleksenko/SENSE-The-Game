#include <objects/player.hpp>
#include <window/window.hpp>
#include <SDL_image.h>
#include <iostream>
#include <iomanip>


Player::Player() {
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

Player::~Player() {
    SDL_DestroyTexture(texture);
    IMG_Quit(); // Terminating SDL_image
}

void Player::moving() {
    if (playerSpeed >= playerNormalSpeed)
        isMoving = true;

    if (isMoving) {
        playerY += 1;

        // Adjust speed scale and decrease based on progress
        float progress = playerY / WINDOW::finalCheckpoint;
        playerSpeedScale = 5.0f + 10 * progress; // Increase scale more aggressively
        playerSpeedDecrease = 0.5f + progress; // Increase decrease rate more aggressively

        if (playerMinSpeed > playerSpeed || playerSpeed > playerMaxSpeed) {
            reset();
            std::cout << "You lost!" << std::endl;
        }
    }

    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Player Y: " << playerY << "\tplayerSpeedScale: " << playerSpeedScale << "\tplayerSpeedDecrease: " << playerSpeedDecrease << std::endl;

    if (playerSpeed >= playerSpeedDecrease && playerY <= WINDOW::finalCheckpoint)
        playerSpeed -= playerSpeedDecrease;
}

void Player::reset() {
    reload.render();
    playerY = 0.0f;
    playerSpeed = 0.0f;
    lastButton = 0;
    isMoving = false;
    currentFrame = 0;
    playerSpeedScale = 5.0f; // Reset to initial values
    playerSpeedDecrease = 0.5f;
}

void Player::updateAnimation() {
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

void Player::render() {
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

void Player::addPlayerSpeed(int button) { 
    lastButton = button;
    playerSpeed += playerSpeedScale;
}
