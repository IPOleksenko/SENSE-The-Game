#pragma once

#include <SDL_image.h>

/**
 * Renders a tiling texture on the screen.
 *
 * @param texture The SDL_Texture to render.
 * @param startY The vertical starting position for the tiling.
 * @param scale Scaling factor for the texture.
 * @param fullscreen If true, the texture will cover the entire screen without maintaining aspect ratio.
 */
void renderTilingTexture(SDL_Texture* texture, int startY, double scale, bool fullscreen = false);
