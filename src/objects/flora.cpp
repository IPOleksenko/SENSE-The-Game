#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <cstdlib>
#include <ctime>

#include "window/window.cpp"

class Flora {
public:
    Flora() {
        loadTextures();
        std::srand(std::time(nullptr)); // Seed for random number generation
    }

    ~Flora() {
        for (SDL_Texture* texture : textures) {
            SDL_DestroyTexture(texture);
        }
    }

    void render(int playerY) {
        if (textures.empty()) {
            std::cerr << "No textures loaded to render." << std::endl;
            return;
        }

        // Generate new textures at fixed distances
        while (playerY + window.BASE_HEIGHT >= nextGenerationY) {
            SDL_Texture* texture = textures[std::rand() % textures.size()]; // Random texture

            int textureWidth, textureHeight;
            SDL_QueryTexture(texture, nullptr, nullptr, &textureWidth, &textureHeight);

            SDL_Rect destRect;
            destRect.x = getRandomXPosition(window.BASE_WIDTH, textureWidth); // Random x-position within allowed sections
            destRect.y = nextGenerationY; // Fixed y-position
            destRect.w = textureWidth;
            destRect.h = textureHeight;

            // Save texture and its position
            renderedTextures.push_back({ texture, destRect });

            nextGenerationY += generationDistance; // Update coordinate for the next generation
        }

        // Update texture coordinates relative to playerY
        for (auto& element : renderedTextures) {
            element.destRect.y -= playerY - lastPlayerY;
            SDL_RenderCopy(window.renderer, element.texture, nullptr, &element.destRect);
        }

        lastPlayerY = playerY; // Update last `playerY`
    }

private:
    struct RenderedTexture {
        SDL_Texture* texture;
        SDL_Rect destRect;
    };

    std::string assetsPath = "assets/sprite/flora";
    std::vector<SDL_Texture*> textures;
    std::vector<RenderedTexture> renderedTextures;

    int lastPlayerY = 0; // Last value of playerY
    int nextGenerationY = 0; // Coordinate for the next generation
    int generationDistance = 150; // Distance between textures

    void loadTextures() {
        for (const auto& entry : std::filesystem::directory_iterator(assetsPath)) {
            if (entry.is_regular_file()) {
                SDL_Texture* texture = IMG_LoadTexture(window.renderer, entry.path().string().c_str());
                if (texture) {
                    textures.push_back(texture);
                }
                else {
                    std::cerr << "Failed to load texture: " << entry.path() << " SDL_Error: " << SDL_GetError() << std::endl;
                }
            }
        }

        if (textures.empty()) {
            std::cerr << "No valid textures found in " << assetsPath << std::endl;
        }
    }

    int getRandomXPosition(int screenWidth, int textureWidth) {
        // Divide the screen into 8 parts
        int sectionWidth = screenWidth / 9;

        // Allowed sections: 3, 4, 6, 7
        int allowedSections[] = { 2, 3, 5, 6 }; // Indices for a 0-based array

        // Choose a random allowed section
        int randomSection = allowedSections[std::rand() % 4];

        // Calculate x-coordinate within the selected section
        int xMin = randomSection * sectionWidth;
        int xMax = xMin + sectionWidth - textureWidth;

        return xMin + (std::rand() % (xMax - xMin + 1));
    }
};
