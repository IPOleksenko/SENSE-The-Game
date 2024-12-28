#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class TextRenderer {
private:
    TTF_Font* font;                // Current font
    SDL_Rect basePosition;         // Current position of the text
    SDL_Rect originalPosition;     // Original position of the text
    Uint8 alpha;                   // Current text transparency (0-255)
    Uint32 animationStartTime;     // Start time of the animation
    Uint32 animationDuration;      // Duration of the animation in milliseconds
    bool fadeIn;                   // Direction flag for the animation
    std::string currentText;       // Current text to display
    bool isAnimated;               // Animation flag
    bool positionChanged;          // Flag indicating if the text position has changed

public:
    TextRenderer(int baseX, int baseY, int fontSize, Uint32 duration = 2000);
    ~TextRenderer();

    void setText(const std::string& text);
    void startAnimation(bool fadeInDirection);
    void setAnimated(bool animated);
    void centerTextOnScreen();
    void restoreOriginalPosition();
    void render();
    void update(const int playerY);
};
