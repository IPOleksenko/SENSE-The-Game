#include <objects/road.hpp>

Road::Road(SDL_Renderer *renderer) :
    m_texture(SDL_Incbin(SPRITE_ROAD_ROAD_PNG), renderer)
{}

void Road::render(const SDL_Point& areaSize, const int& posY) {
    m_texture.tile(
        areaSize,
        0.12f,
        -posY,
        false
    );
}
