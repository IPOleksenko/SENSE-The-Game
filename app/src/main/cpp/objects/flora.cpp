#include <objects/flora.hpp>
#include <objects/flora_config.hpp>
#include <random>
#include <algorithm>
#include <filesystem>


Flora::Flora(SDL_Renderer* renderer) {
    loadTextures(renderer);
}

RawTexture Flora::loadTexture(SDL_Renderer* renderer, const std::string& name) {
    std::string customPath = modding::joinPath(modding::getModdingDirectory(), "flora");
    customPath = modding::joinPath(customPath, name + ".png");

    if (std::filesystem::exists(customPath)) {
        return RawTexture(customPath.c_str(), renderer);
    }

    if (name == "grass") return RawTexture(SDL_Incbin(SPRITE_FLORA_GRASS_PNG), renderer);
    if (name == "flower1") return RawTexture(SDL_Incbin(SPRITE_FLORA_FLOWER1_PNG), renderer);
    if (name == "flower2") return RawTexture(SDL_Incbin(SPRITE_FLORA_FLOWER2_PNG), renderer);
    if (name == "flower3") return RawTexture(SDL_Incbin(SPRITE_FLORA_FLOWER3_PNG), renderer);
    if (name == "flower4") return RawTexture(SDL_Incbin(SPRITE_FLORA_FLOWER4_PNG), renderer);
    if (name == "smallrock1") return RawTexture(SDL_Incbin(SPRITE_FLORA_SMALLROCK1_PNG), renderer);
    if (name == "smallrock2") return RawTexture(SDL_Incbin(SPRITE_FLORA_SMALLROCK2_PNG), renderer);
    if (name == "smallrock3") return RawTexture(SDL_Incbin(SPRITE_FLORA_SMALLROCK3_PNG), renderer);

    return RawTexture();
}

void Flora::loadTextures(SDL_Renderer* renderer) {
    auto assets = modding::loadFloraAssets();

    std::string configPath = modding::joinPath(modding::getModdingDirectory(), "flora.cfg");
    std::map<std::string, bool> config;
    
    if (modding::fileExists(configPath)) {
        auto lines = modding::readTextFile(configPath);
        for (const auto& line : lines) {
            if (line.find('=') != std::string::npos) {
                std::string name = line.substr(0, line.find('='));
                std::string value = line.substr(line.find('=') + 1);
                config[name] = (value == "true");
            }
        }
    }
    
    for (const auto& asset : assets) {
        bool enabled = config.count(asset.name) ? config[asset.name] : true;

        if (enabled) {
            auto texture = loadTexture(renderer, asset.name);
            if (texture.get()) {
                m_textures.push_back(std::move(texture));
            }
        }
    }
}

void Flora::reset() {
    m_floraView.clear();
    m_nextGenY = 0;
    m_lastPosY = 0;
}

void Flora::render(const SDL_Point& areaSize, const int& posY) {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    // Update prevArea and scale/clamp positions when the screen size changes
    static SDL_Point prevArea{ 0, 0 };
    if (prevArea.x != areaSize.x || prevArea.y != areaSize.y) {
        if (prevArea.x > 0 && prevArea.y > 0) {
            const float fx = static_cast<float>(areaSize.x) / prevArea.x;
            const float fy = static_cast<float>(areaSize.y) / prevArea.y;

            for (auto& view : m_floraView) {
                // Scale position
                view.pos.x = static_cast<int>(view.pos.x * fx);
                view.pos.y = static_cast<int>(view.pos.y * fy);
            }
            m_nextGenY = static_cast<int>(m_nextGenY * fy);
            m_lastPosY = static_cast<int>(m_lastPosY * fy);
        }
        prevArea = areaSize;
    }

    // Boundaries in percent
    int minX = std::max(0, static_cast<int>(areaSize.x * 0.22f));
    int maxX = std::max(minX, static_cast<int>(areaSize.x * 0.78f));

    static size_t lastTextureCount = 0;
    static std::uniform_int_distribution<size_t> texDist(0, 0);
    if (lastTextureCount != m_textures.size()) {
        if (!m_textures.empty()) {
            texDist = std::uniform_int_distribution<size_t>(0, m_textures.size() - 1);
        }
        else {
            texDist = std::uniform_int_distribution<size_t>(0, 0);
        }
        lastTextureCount = m_textures.size();
    }

    // Shift all existing views when scrolling
    for (auto& view : m_floraView) {
        view.pos.y -= (posY - m_lastPosY);
    }

    const int genStep = std::max(1, static_cast<int>(s_genDist * (static_cast<float>(areaSize.y) / 720.0f)));

    // Generate new plants along Y
    while (posY + areaSize.y > m_nextGenY) {
        for (int i = 0; i < s_density; ++i) {
            const Texture& tex = m_textures[texDist(gen)];

            // Get texture size and scale it to correctly account for width when choosing X
            SDL_Point texSize{};
            if (!tex.querySize(texSize)) continue;

            const float scale = std::max(0.01f, static_cast<float>(areaSize.x) / 1280.0f);
            const int newW = std::max(1, static_cast<int>(texSize.x * scale));

            // Calculate allowed X range considering sprite width
            int genMinX = minX;
            int genMaxX = std::max(genMinX, maxX - newW); // if newW > (maxX - minX) => genMaxX == genMinX

            std::uniform_int_distribution<int> posXDist(genMinX, genMaxX);
            int chosenX = posXDist(gen);

            m_floraView.push_back({ tex, { chosenX, m_nextGenY } });
        }

        m_nextGenY += genStep;
    }

    // Remove those that went above the upper boundary
    m_floraView.remove_if([&areaSize](const auto& view) {
        SDL_Point size{};
        if (!view.texture.querySize(size)) return true;

        const float scale = std::max(0.01f, static_cast<float>(areaSize.x) / 1280.0f);
        const int newH = std::max(1, static_cast<int>(size.y * scale));

        return (view.pos.y + newH) < 0;
        });

    // Rendering
    for (auto& view : m_floraView) {
        SDL_Point texSize{};
        if (!view.texture.querySize(texSize)) continue;

        const float scale = std::max(0.01f, static_cast<float>(areaSize.x) / 1280.0f);
        const int newW = std::max(1, static_cast<int>(texSize.x * scale));
        const int newH = std::max(1, static_cast<int>(texSize.y * scale));

        // Right boundary for X position considering sprite width
        int clampMaxX = std::max(minX, maxX - newW);
        view.pos.x = std::clamp(view.pos.x, minX, clampMaxX);

        const SDL_Rect destRect{
                view.pos.x,
                view.pos.y,
                newW,
                newH
        };

        view.texture.render(&destRect, nullptr);
    }

    m_lastPosY = posY;
}
