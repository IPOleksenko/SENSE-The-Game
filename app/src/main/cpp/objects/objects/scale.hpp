#pragma once

#include <SDL.h>
#include <string>

class ScaleGenerator {
private:
    SDL_Texture* arrowTexture = nullptr;
    SDL_Texture* scaleTexture = nullptr;

    /**
     * Helper function to safely load a texture.
     *
     * @param path The file path to the texture.
     * @return A pointer to the loaded SDL_Texture or nullptr if loading fails.
     */
    SDL_Texture* loadTexture(const std::string& path);

    /**
     * Helper function to linearly interpolate between two values.
     *
     * @param start The starting value.
     * @param end The ending value.
     * @param t The interpolation factor (0.0 to 1.0).
     * @return The interpolated value.
     */
    int interpolate(int start, int end, double t);

public:
    ScaleGenerator();
    ~ScaleGenerator();

    /**
     * Render the scale and arrow based on the player's speed.
     *
     * @param playerSpeed The current speed of the player.
     * @param playerMinSpeed The minimum speed for the player.
     * @param playerMaxSpeed The maximum speed for the player.
     */
    void render(int playerSpeed, int playerMinSpeed, int playerMaxSpeed);
};
