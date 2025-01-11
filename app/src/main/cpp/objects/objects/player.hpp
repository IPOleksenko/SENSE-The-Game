#pragma once

#include <utils/texture.hpp>
#include <SDL.h>


class Player {
public:
    enum class Move {
        Left,
        Right,
        Undefined,
    };

    explicit Player(SDL_Renderer* renderer);
    ~Player() = default;

    [[nodiscard]] int getPosY() const;
    [[nodiscard]] float getSpeed() const;
    [[nodiscard]] float getSpeedMin() const;
    [[nodiscard]] float getSpeedMax() const;
    [[nodiscard]] Move getLastMove() const;
    [[nodiscard]] bool hasLost() const;

    void move();
    void reset();
    void render(const SDL_Point& windowSize);
    void increaseSpeed(const Player::Move& move);
    void updateAnimation();

private:
    // Player parameters
    RawTexture m_texture;
    int m_posY = 0;
    float m_speed = 0;

    Move m_lastMove = Move::Undefined;
    bool m_isMoving = false;
    bool m_hasLost = false;

    float m_speedScale = 5.0f;
    float m_speedDecrease = 0.5f;
    const float m_speedMin = m_speedDecrease + 50.0f;
    const float m_speedMax = 150.0f;
    const float m_speedNormal = (m_speedMin + m_speedMax) / 2;

    // Animation parameters
    SDL_Point m_frameSize = {};
    int m_frameCurrent = 0;
    Uint32 m_lastFrameTime = 0; // Time of the last frame switch
    const int m_framesTotal = 120;    // Total number of frames in the animation
    const int m_framesPerRow = 20;    // Number of frames per row
    const int m_animationSpeed = 100; // Animation speed in milliseconds
};
