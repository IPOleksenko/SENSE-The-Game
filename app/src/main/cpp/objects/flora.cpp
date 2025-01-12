#include <objects/flora.hpp>
#include <random>
#include <algorithm>


Flora::Flora(SDL_Renderer* renderer)
    : m_textures{
        RawTexture(SDL_Incbin(SPRITE_FLORA_GRASS_PNG), renderer),
        RawTexture(SDL_Incbin(SPRITE_FLORA_FLOWER1_PNG), renderer),
        RawTexture(SDL_Incbin(SPRITE_FLORA_FLOWER2_PNG), renderer),
        RawTexture(SDL_Incbin(SPRITE_FLORA_FLOWER3_PNG), renderer),
        RawTexture(SDL_Incbin(SPRITE_FLORA_FLOWER4_PNG), renderer),
        RawTexture(SDL_Incbin(SPRITE_FLORA_SMALLROCK1_PNG), renderer),
        RawTexture(SDL_Incbin(SPRITE_FLORA_SMALLROCK2_PNG), renderer),
        RawTexture(SDL_Incbin(SPRITE_FLORA_SMALLROCK3_PNG), renderer),
    }
{}

void Flora::reset() {
    m_floraView.clear();
    m_nextGenY = 0;
    m_lastPosY = 0;
}

void Flora::render(const SDL_Point& areaSize, const int& posY) {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<size_t> texDist(
        0,
        m_textures.size() - 1
    );
    static std::uniform_int_distribution<int> posXDist(
        static_cast<int>(static_cast<float>(areaSize.x) * (2.0f / 9.0f)),
        static_cast<int>(static_cast<float>(areaSize.x) * (7.0f / 9.0f))
    );

    for(auto & view: m_floraView) {
        view.pos.y -= (posY - m_lastPosY);
    }

    while (posY + areaSize.y > m_nextGenY) {
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

            if(view.pos.y < 0) {
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
