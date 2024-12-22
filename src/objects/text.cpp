#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>

#include "window/window.cpp" // Include the file with renderer and window definitions

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
    TextRenderer(int baseX, int baseY, int fontSize, Uint32 duration = 2000)
        : basePosition{ baseX, baseY, 0, 0 },
        originalPosition{ baseX, baseY, 0, 0 },
        alpha(0), animationStartTime(0), animationDuration(duration),
        fadeIn(true), currentText(""), isAnimated(true), positionChanged(false) {
        if (TTF_Init() == -1) {
            printf("TTF_Init: %s\n", TTF_GetError());
            exit(1);
        }

        font = TTF_OpenFont("assets/font/font.ttf", fontSize);
        if (!font) {
            printf("TTF_OpenFont: %s\n", TTF_GetError());
            exit(1);
        }
    }

    ~TextRenderer() {
        if (font) TTF_CloseFont(font);
        TTF_Quit();
    }

    void setText(const std::string& text) {
        currentText = text;
        alpha = 0; // Reset text transparency when the text changes
    }

    void startAnimation(bool fadeInDirection) {
        fadeIn = fadeInDirection;
        animationStartTime = SDL_GetTicks();
    }

    void setAnimated(bool animated) {
        isAnimated = animated;
        if (!animated) {
            alpha = 255;
        }
    }

    void centerTextOnScreen() {
        if (currentText.empty()) return;

        // Save the current position as the original
        if (!positionChanged) {
            originalPosition = basePosition;
            positionChanged = true;
        }

        // Split the text into lines
        std::istringstream stream(currentText);
        std::string line;

        int totalHeight = 0;
        int maxWidth = 0;

        // Calculate total text height and maximum line width
        while (std::getline(stream, line)) {
            SDL_Surface* surface = TTF_RenderText_Blended(font, line.c_str(), { 255, 255, 255, 255 });
            if (!surface) {
                printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
                continue;
            }
            totalHeight += surface->h;
            maxWidth = std::max(maxWidth, surface->w);
            SDL_FreeSurface(surface);
        }

        // Center the text relative to the screen
        basePosition.x = (window.BASE_WIDTH - maxWidth) / 2;
        basePosition.y = (window.BASE_HEIGHT - totalHeight) / 2;
    }

    void restoreOriginalPosition() {
        if (positionChanged) {
            basePosition = originalPosition;
            positionChanged = false;
        }
    }

    void render() {
        if (currentText.empty()) return;

        if (isAnimated) {
            Uint32 currentTime = SDL_GetTicks();
            float progress = static_cast<float>(currentTime - animationStartTime) / animationDuration;
            progress = std::min(1.0f, progress);

            alpha = static_cast<Uint8>((fadeIn ? progress : 1.0f - progress) * 255);
        }
        else {
            alpha = 255;
        }

        if (alpha == 0) return;

        std::istringstream stream(currentText);
        std::string line;
        int offsetY = 0;

        while (std::getline(stream, line)) {
            SDL_Surface* surface = TTF_RenderText_Blended(font, line.c_str(), { 255, 255, 255, 255 });
            if (!surface) {
                printf("TTF_RenderText_Blended: %s\n", TTF_GetError());
                continue;
            }

            SDL_Texture* texture = SDL_CreateTextureFromSurface(window.renderer, surface);
            if (!texture) {
                printf("SDL_CreateTextureFromSurface: %s\n", SDL_GetError());
                SDL_FreeSurface(surface);
                continue;
            }

            SDL_SetTextureAlphaMod(texture, alpha);

            SDL_Rect destRect = {
                basePosition.x,
                basePosition.y + offsetY,
                surface->w,
                surface->h
            };

            SDL_RenderCopy(window.renderer, texture, nullptr, &destRect);

            offsetY += surface->h;

            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }
    }
};
