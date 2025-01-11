#include <objects/player.hpp>
#include <iostream>
#include <iomanip>

#define FINAL_CHECKPOINT  25000

Player::Player(SDL_Renderer *renderer)
    : m_texture(SDL_Incbin(SPRITE_PLAYER_PLAYER_PNG), renderer)
{
    SDL_Point textureSize = {};

    if(m_texture.querySize(textureSize)) {
        m_frameSize = {
            textureSize.x / m_framesPerRow,
            textureSize.y / (m_framesTotal / m_framesPerRow)
        };
    }
}

int Player::getPosY() const {
    return m_posY;
}

float Player::getSpeed() const {
    return m_speed;
}

float Player::getSpeedMin() const {
    return m_speedMin;
}

float Player::getSpeedMax() const {
    return m_speedMax;
}

Player::Move Player::getLastMove() const {
    return m_lastMove;
}

bool Player::hasLost() const {
    return m_hasLost;
}

void Player::move() {
    if (m_speed >= m_speedNormal) {
        m_isMoving = true;
    }

    if (m_isMoving) {
        m_posY += 1;

        // Adjust speed scale and decrease based on progress
        float progress = static_cast<float>(m_posY) / 25000;
        m_speedScale = 5.0f + 10 * progress; // Increase scale more aggressively
        m_speedDecrease = 0.5f + progress; // Increase decrease rate more aggressively

        if (m_speed < m_speedMin || m_speedMax < m_speed) {
            m_hasLost = true;
        }
    }

    SDL_LogDebug(
        SDL_LOG_CATEGORY_APPLICATION,
        "Player "
            "pos y: %d\t"
            "speed scale: %.4f\t"
            "speed decrease: %.4f",
        m_posY,
        m_speedScale,
        m_speedDecrease
    );

    if (m_speed >= m_speedDecrease && m_posY <= FINAL_CHECKPOINT) {
        m_speed -= m_speedDecrease;
    }
}

void Player::reset() {
    m_posY = 0;
    m_speed = 0;
    m_frameCurrent = 0;
    m_lastMove = Move::Undefined;

    m_speedScale = 5.0f;
    m_speedDecrease = 0.5f;

    m_isMoving = false;
    m_hasLost = false;
}

void Player::render(const SDL_Point& windowSize) {
    const int row = m_frameCurrent / m_framesPerRow;
    const int col = m_frameCurrent % m_framesPerRow;

    SDL_Rect srcRect = {
        col * m_frameSize.x,    // Start of the frame on X
        row * m_frameSize.y,    // Start of the frame on Y
        m_frameSize.x,          // Frame width
        m_frameSize.y           // Frame height
    };

    SDL_Rect destRect = {
        (windowSize.x - m_frameSize.x) / 2,        // Center on X
        (windowSize.y - m_frameSize.y) / 2 - 10,   // Center on Y minus 10 pixels
        m_frameSize.x,
        m_frameSize.y
    };

    // Rendering the current frame
    m_texture.render(&destRect, &srcRect);
}

void Player::increaseSpeed(const Player::Move& move) {
    if(m_lastMove != move && move != Move::Undefined) {
        m_lastMove = move;
        m_speed += m_speedScale;
    }
}

void Player::updateAnimation() {
    if (!m_isMoving) {
        m_frameCurrent = 0; // Reset to the first frame if the player is not moving
        return;
    }

    Uint32 currentTime = SDL_GetTicks();
    if (currentTime > m_lastFrameTime + m_animationSpeed) {
        m_frameCurrent = (m_frameCurrent + 1) % m_framesTotal;
        m_lastFrameTime = currentTime;
    }
}
