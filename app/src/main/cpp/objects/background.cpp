#include <objects/background.hpp>

Background::Background(SDL_Renderer *renderer) :
    back(SDL_Incbin(SPRITE_BACKGROUND_BACK_PNG), renderer),
    front(SDL_Incbin(SPRITE_BACKGROUND_FRONT_PNG), renderer)
{}

void Background::render(const SDL_Point& areaSize, const int& posY) {
    back.tile(areaSize, 1.0f, -posY, true);
    front.tile(areaSize, 1.0f, -posY, false);
}
