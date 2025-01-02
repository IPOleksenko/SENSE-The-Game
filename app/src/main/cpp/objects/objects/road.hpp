#pragma once

#include <SDL.h>


class RoadGenerator {
private:
    SDL_Texture* texture = nullptr;

public:
    RoadGenerator();
    ~RoadGenerator();

    /**
     * Render the road.
     *
     * @param Y Vertical offset for the road texture.
     */
    void render(int Y);
};
