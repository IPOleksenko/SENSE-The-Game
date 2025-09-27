#include <objects/player.hpp>
#include <iomanip>

#define FINAL_CHECKPOINT 25000

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

bool Player::getIsMove() const{
    return m_isMoving;
}

bool Player::hasLost() const {
    return m_hasLost;
}

void Player::move(bool endlessMode) {
    if (m_speed >= m_speedNormal) {
        m_isMoving = true;
    }

    if (m_isMoving) {
        if (((m_posY >= FINAL_CHECKPOINT) == m_isFinalAnimationFinished) || endlessMode)
            m_posY += 1;

        // Adjust speed scale and decrease based on progress
        float progress = static_cast<float>(m_posY) / FINAL_CHECKPOINT;
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

    if (m_speed >= m_speedDecrease && (m_posY < FINAL_CHECKPOINT || endlessMode)) {
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
    m_isFinalAnimationFinished = false;
}

void Player::render(const SDL_Point& windowSize, bool endlessMode) {
    const int row = m_frameCurrent / m_framesPerRow;
    const int col = m_frameCurrent % m_framesPerRow;

    SDL_Rect srcRect = {
            col * m_frameSize.x,
            row * m_frameSize.y,
            m_frameSize.x,
            m_frameSize.y
    };

    const float baseWidth  = 1280.0f;
    const float baseHeight = 720.0f;

    float scaleX = static_cast<float>(windowSize.x) / baseWidth;
    float scaleY = static_cast<float>(windowSize.y) / baseHeight;

    int scaledW = static_cast<int>(m_frameSize.x * scaleX);
    int scaledH = static_cast<int>(m_frameSize.y * scaleY);

    SDL_Rect destRect = {
            static_cast<int>((windowSize.x - scaledW) / 2),
            static_cast<int>((windowSize.y - scaledH) / 2 - (10 * scaleY)),
            scaledW,
            scaledH
    };

    if (m_posY < FINAL_CHECKPOINT || endlessMode) {
        m_texture.render(&destRect, &srcRect);
    }
    else {
        static int exitOffset = 0;
        exitOffset += 1;
        destRect.y += exitOffset;

        if (destRect.y < windowSize.y) {
            m_texture.render(&destRect, &srcRect);
        }
        else {
            m_isFinalAnimationFinished = true;
        }
    }

}

bool Player::isFinalAnimationFinished() const {
    return m_isFinalAnimationFinished;
}

void Player::increaseSpeed(const Player::Move& move, bool endlessMode) {
    if (m_posY >= FINAL_CHECKPOINT && !endlessMode) { return; }

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
