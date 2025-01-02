#include <objects/text.hpp>
#include <window/window.hpp>
#include <assets/assets.hpp>
#include <sstream>


TextRenderer::TextRenderer(int baseX, int baseY, int fontSize, Uint32 duration)
    : basePosition{ baseX, baseY, 0, 0 }
    , originalPosition{ baseX, baseY, 0, 0 }
    , alpha(0)
    , animationStartTime(0)
    , animationDuration(duration)
    , fadeIn(true)
    , currentText("")
    , isAnimated(true)
    , positionChanged(false)
{
    if (TTF_Init() == -1) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(1);
    }

    font = TTF_OpenFontRW(SDL_Incbin(FONT_FONT_TTF), SDL_TRUE, fontSize);
    if (!font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(1);
    }
}

TextRenderer::~TextRenderer() {
    if (font) TTF_CloseFont(font);
    TTF_Quit();
}

void TextRenderer::setText(const std::string& text) {
    currentText = text;
    alpha = 0; // Reset text transparency when the text changes
}

void TextRenderer::startAnimation(bool fadeInDirection) {
    fadeIn = fadeInDirection;
    animationStartTime = SDL_GetTicks();
}

void TextRenderer::setAnimated(bool animated) {
    isAnimated = animated;
    if (!animated) {
        alpha = 255;
    }
}

void TextRenderer::centerTextOnScreen() {
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

void TextRenderer::restoreOriginalPosition() {
    if (positionChanged) {
        basePosition = originalPosition;
        positionChanged = false;
    }
}

void TextRenderer::render() {
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

void TextRenderer::update(const int playerY) {
    switch (playerY) {
    case 0:
        // Initial message
        setText(R"(
Instructions:
    *Press A (or LEFT) and D (or RIGHT) alternately to move.
    *Pass the middle mark to start moving.
    *Keep your balance—don't let the pointer hit the red zone, or you'll lose!

Fullscreen: Press F to toggle fullscreen mode.)");

        restoreOriginalPosition();
        setAnimated(false);       // Disable animation
        break;
    case 1:
        // Start fading out animation
        setAnimated(true);
        startAnimation(false);
        break;

    case 1000:
        // Display a philosophical message
        setText("You opened your eyes, but did you see anything new?");
        centerTextOnScreen();     // Center the text on the screen
        startAnimation(true);     // Start fading in animation
        break;
    case 1500:
        // Stop animation
        startAnimation(false);
        break;

    case 2000:
        setText("Every day is like the one before, yet you search for differences");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 2500:
        startAnimation(false);
        break;

    case 3000:
        setText("People chase dreams, but who said dreams hold value?");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 3500:
        startAnimation(false);
        break;

    case 4000:
        setText("How many questions have you asked, and how many answers have you received?");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 4500:
        startAnimation(false);
        break;

    case 5000:
        setText("The world moves in circles, but where is its beginning and where is its end?");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 5500:
        startAnimation(false);
        break;

    case 6000:
        setText("You strive to find a purpose, but what is it even for?");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 6500:
        startAnimation(false);
        break;

    case 7000:
        setText("Stones lie on the ground for millennia, yet you live for only a moment.");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 7500:
        startAnimation(false);
        break;

    case 8000:
        setText("What matters more: your thoughts or the sound of the wind?");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 8500:
        startAnimation(false);
        break;

    case 9000:
        setText("The history of the world is full of heroes, but no one remembers them.");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 9500:
        startAnimation(false);
        break;

    case 10000:
        setText("If something disappears tomorrow, what changes today?");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 10500:
        startAnimation(false);
        break;

    case 11000:
        setText("The sun rises every day, but not for you.");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 11500:
        startAnimation(false);
        break;

    case 12000:
        setText("Your heart beats, but who cares?");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 12500:
        startAnimation(false);
        break;

    case 13000:
        setText("Everything you build will one day turn to dust.");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 13500:
        startAnimation(false);
        break;

    case 14000:
        setText("You search for truth, but in this world, there is no law of truth.");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 14500:
        startAnimation(false);
        break;

    case 15000:
        setText("Who created this world if even they had no plan?");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 15500:
        startAnimation(false);
        break;

    case 16000:
        setText("Joy and pain alternate, but both eventually fade.");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 16500:
        startAnimation(false);
        break;

    case 17000:
        setText("You want to be needed, but by whom?");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 17500:
        startAnimation(false);
        break;

    case 18000:
        setText("The stars shine, but not to show you the way.");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 18500:
        startAnimation(false);
        break;

    case 19000:
        setText("Eternity is a word that both frightens and frees.");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 19500:
        startAnimation(false);
        break;

    case 20000:
        setText("In this chaos, you seek meaning, but chaos demands no explanation.");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case 20500:
        startAnimation(false);
        break;

    case WINDOW::finalCheckpoint:
        setText("THE UNIVERSE DOESN'T MAKE SENSE.");
        centerTextOnScreen();
        startAnimation(true);
        break;
    case WINDOW::finalCheckpoint + 500:
        // End the game
        window.running = false;
        break;
    }
}
