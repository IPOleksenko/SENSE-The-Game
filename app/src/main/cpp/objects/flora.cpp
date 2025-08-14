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
    if (m_textures.empty()) {
        m_lastPosY = posY;
        return;
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<int> posXDist(
        static_cast<int>(static_cast<float>(areaSize.x) * (2.0f / 9.0f)),
        static_cast<int>(static_cast<float>(areaSize.x) * (7.0f / 9.0f))
    );

    static size_t lastTextureCount = 0;
    static std::uniform_int_distribution<size_t> texDist(0, 0);
    if (lastTextureCount != m_textures.size()) {
        texDist = std::uniform_int_distribution<size_t>(0, m_textures.size() - 1);
        lastTextureCount = m_textures.size();
    }

    for(auto & view: m_floraView) {
        view.pos.y -= (posY - m_lastPosY);
    }

    while (posY + areaSize.y > m_nextGenY) {
        if (m_nextGenY > kMaxFloraY) {
            break;
        }
        for (int i = 0; i < s_density; ++i) {
            const Texture &tex = m_textures[texDist(gen)];

            m_floraView.push_back({
                tex,
                {posXDist(gen), m_nextGenY}
            });
        }

        m_nextGenY += s_genDist;
    }

    m_floraView.remove_if(
        [&areaSize](const auto& view) {
            SDL_Point size {};
            if(!view.texture.querySize(size)) {
                return true;
            }

            if(view.pos.y + size.y < 0) {
                return true;
            }

            return false;
        }
    );

    for(const auto & view: m_floraView) {
        SDL_Point size {};
        if(!view.texture.querySize(size)) {
            continue;
        }

        const SDL_Rect destRect {
            view.pos.x, view.pos.y,
            size.x, size.y
        };

        view.texture.render(&destRect, nullptr);
    }

    m_lastPosY = posY;
}
