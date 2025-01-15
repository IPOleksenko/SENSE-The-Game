#include <application/game.hpp>
#include <application/window.hpp>
#include <application/renderer.hpp>
#include <application/audio.hpp>
#include <objects/text.hpp>
#include <objects/player.hpp>
#include <objects/background.hpp>
#include <objects/scale.hpp>
#include <objects/road.hpp>
#include <objects/flora.hpp>
#include <objects/end.hpp>
#include <utils/music.hpp>
#include <utils/sfx.hpp>
#include <utils/icon.hpp>
#include <assets/checkpoints.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>


const std::string Game::s_orientation = "Landscape";
const std::string Game::s_name = "Sense The Game";
const SDL_Point Game::s_windowSize = { 1280, 720 };
const SDL_Point Game::s_windowPos = {
    static_cast<int>(SDL_WINDOWPOS_CENTERED),
    static_cast<int>(SDL_WINDOWPOS_CENTERED)
};


Game::Game() :
    m_isInit(false)
{
    m_isInit = (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == 0);
    if(!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "SDL_Init", SDL_GetError()
        );
        return;
    }

    m_isInit = (IMG_Init(IMG_INIT_PNG) != 0);
    if(!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "IMG_Init", SDL_GetError()
        );
        return;
    }

    m_isInit = (Mix_Init(MIX_INIT_MP3) != 0);
    if (!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "Mix_Init", SDL_GetError()
        );
        return;
    }

    m_isInit = (TTF_Init() == 0);
    if(!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "TTF_Init", SDL_GetError()
        );
        return;
    }

    if(SDL_SetHint(SDL_HINT_ORIENTATIONS, s_orientation.c_str()) == SDL_FALSE) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "SDL_SetHint", "can't set orientation"
        );
    }
}

bool Game::isInit() const {
    return m_isInit;
}

void Game::run() const {
    if(!isInit()) {
        return;
    }

    Window window(s_windowPos, s_windowSize, s_name);
    if(!window.isInit()) {
        return;
    }

    Renderer renderer(window);
    if(!renderer.isInit()) {
        return;
    }

    AudioManager audioManager;
    if(!audioManager.isInit()) {
        return;
    }

    window.setIcon(
        Icon(SDL_Incbin(ICON_BMP))
    );

    play(window, renderer, audioManager);
}

void Game::loadStartScreen(Window& window, Renderer& renderer) {
    renderer.setDrawColor({0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE});
    renderer.clear();

    Text text(renderer.getSdlRenderer(), 48, {0, 0});
    text.setText("Loading...");
    text.positionCenter();
    text.render(window.getSize());

    renderer.present();
    SDL_Delay(200);
}

void Game::updateText(Text& text, const int& yPos) {
    switch (static_cast<CheckPoint>(yPos)) {
        case CheckPoint::IDLE: {
            text.setText(getCheckpointText(CheckPoint::IDLE));
            text.positionReset();
            text.animationStop();
            break;
        }
        case CheckPoint::BEGIN: {
            text.animationStart(false);
            break;
        }
        case CheckPoint::A_START:
        case CheckPoint::B_START:
        case CheckPoint::C_START:
        case CheckPoint::D_START:
        case CheckPoint::E_START:
        case CheckPoint::F_START:
        case CheckPoint::G_START:
        case CheckPoint::H_START:
        case CheckPoint::I_START:
        case CheckPoint::J_START:
        case CheckPoint::K_START:
        case CheckPoint::L_START:
        case CheckPoint::M_START:
        case CheckPoint::N_START:
        case CheckPoint::O_START:
        case CheckPoint::P_START:
        case CheckPoint::Q_START:
        case CheckPoint::R_START:
        case CheckPoint::S_START:
        case CheckPoint::T_START: 
        case CheckPoint::FINAL_START: {
            if (static_cast<CheckPoint>(yPos) == CheckPoint::FINAL_START) {
                text.resize(48);
            }
            text.setText(getCheckpointText(static_cast<CheckPoint>(yPos)));
            text.positionCenter();
            text.animationStart(true);
            break;
        }
        case CheckPoint::A_STOP:
        case CheckPoint::B_STOP:
        case CheckPoint::C_STOP:
        case CheckPoint::D_STOP:
        case CheckPoint::E_STOP:
        case CheckPoint::F_STOP:
        case CheckPoint::G_STOP:
        case CheckPoint::H_STOP:
        case CheckPoint::I_STOP:
        case CheckPoint::J_STOP:
        case CheckPoint::K_STOP:
        case CheckPoint::L_STOP:
        case CheckPoint::M_STOP:
        case CheckPoint::N_STOP:
        case CheckPoint::O_STOP:
        case CheckPoint::P_STOP:
        case CheckPoint::Q_STOP:
        case CheckPoint::R_STOP:
        case CheckPoint::S_STOP:
        case CheckPoint::T_STOP:
        case CheckPoint::FINAL_STOP: {
            text.animationStart(false);
            break;
        }
    }
}

void Game::play(Window& window, Renderer& renderer, AudioManager& audioManager) {
    loadStartScreen(window, renderer);

    Background background(renderer.getSdlRenderer());
    Flora flora(renderer.getSdlRenderer());
    Road road(renderer.getSdlRenderer());
    Player player(renderer.getSdlRenderer());
    Scale scale(renderer.getSdlRenderer());
    End end(renderer.getSdlRenderer());
    Text text(renderer.getSdlRenderer(), 24, {0, 0});
    Music soundtrack(SDL_Incbin(SOUND_WIND_WAV));
    Sfx reload(SDL_Incbin(SOUND_RELOAD_WAV));

    audioManager.play(soundtrack, AudioManager::TIMES_LOOP);
    audioManager.resume();

    bool isRunning = true;
    SDL_Event event = {};

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_KEYUP: {
                    const SDL_KeyboardEvent &keyboardEvent = event.key;

                    switch (keyboardEvent.keysym.sym) {
                        case SDLK_ESCAPE: {
                            isRunning = false;
                            break;
                        }
                        case SDLK_f: {
                            if(window.isFullscreen()) {
                                window.setFullscreenOff();
                            } else {
                                window.setFullscreenOn();
                            }
                            break;
                        }
                        case SDLK_LEFT:
                        case SDLK_a: {
                            player.increaseSpeed(Player::Move::Left);
                            break;
                        }
                        case SDLK_RIGHT:
                        case SDLK_d: {
                            player.increaseSpeed(Player::Move::Right);
                            break;
                        }
                    }
                    break;
                }
                case SDL_FINGERDOWN : {
                    const SDL_TouchFingerEvent &touchEvent = event.tfinger;

                    if (touchEvent.x < 0.5f) {
                        player.increaseSpeed(Player::Move::Left);
                    } else {
                        player.increaseSpeed(Player::Move::Right);
                    }
                    break;
                }
                case SDL_QUIT: {
                    isRunning = false;
                    break;
                }
            }
        }

        renderer.setDrawColor({0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE});
        renderer.clear();
        player.move();

        if(player.hasLost()) {
            player.reset();
            flora.reset();
            renderer.present();

            if(
                const int channel = audioManager.play(
                    reload, 1, AudioManager::CHANNEL_UNDEFINED
                );
                channel != AudioManager::CHANNEL_UNDEFINED
            ) {
                while (Mix_Playing(channel) == SDL_TRUE);                
            }

            renderer.setDrawColor({0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE});
            renderer.clear();
        }

        background.render(window.getSize(), player.getPosY());
        flora.render(window.getSize(), player.getPosY());
        road.render(window.getSize(), player.getPosY());
        player.updateAnimation();
        player.render(window.getSize());

        scale.render(
            window.getSize(),
            player.getSpeed(),
            player.getSpeedMin(),
            player.getSpeedMax()
        );

        if(player.getPosY() >= static_cast<int>(CheckPoint::FINAL_START)) {
            end.render(window.getSize());
            if (player.getPosY() >= static_cast<int>(CheckPoint::FINAL_STOP))
                isRunning = false;
        }

        updateText(text, player.getPosY());
        text.render(window.getSize());

        renderer.present();
        SDL_Delay(16);
    }

    audioManager.stop();
}

Game::~Game() {
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();

    m_isInit = false;
}
