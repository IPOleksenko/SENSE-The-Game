#include <application/game.hpp>
#include <application/window.hpp>
#include <application/renderer.hpp>
#include <application/audio.hpp>
#include <objects/text.hpp>
#include <objects/player.hpp>
#include <objects/background.hpp>
#include <objects/scale.hpp>
#include <objects/road.hpp>
#include <objects/decor.hpp>
#include <objects/end.hpp>
#include <utils/music.hpp>
#include <utils/sfx.hpp>
#include <utils/icon.hpp>
#include <assets/checkpoints.hpp>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <utils/utils/modding.hpp>
#include <utils/utils/localization.hpp>

const std::string Game::s_orientation = "Landscape";
const std::string Game::s_name = "SENSE: The Game";
const SDL_Point Game::s_windowSize = { 1280, 720 };
const SDL_Point Game::s_windowPos = {
        static_cast<int>(SDL_WINDOWPOS_CENTERED),
        static_cast<int>(SDL_WINDOWPOS_CENTERED)
};

std::vector<SDL_GameController*> Game::controllers;

Game::Game() :
    m_isInit(false)
{
    m_isInit = (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) == 0);
    if (!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "SDL_Init", SDL_GetError()
        );
        return;
    }

    m_isInit = (IMG_Init(IMG_INIT_PNG) != 0);
    if (!m_isInit) {
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
    if (!m_isInit) {
        SDL_LogCritical(
            SDL_LOG_CATEGORY_SYSTEM, "%s failed: %s",
            "TTF_Init", SDL_GetError()
        );
        return;
    }

    if (SDL_SetHint(SDL_HINT_ORIENTATIONS, s_orientation.c_str()) == SDL_FALSE) {
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
    if (!isInit()) {
        return;
    }

#if defined(__ANDROID__)
    SDL_Rect displayBounds;
    if (SDL_GetDisplayBounds(0, &displayBounds) != 0) {
        SDL_LogError(SDL_LOG_CATEGORY_VIDEO, "Failed to get display bounds: %s", SDL_GetError());
        return;
    }

    SDL_Point adjustedSize = { displayBounds.w, displayBounds.h };

    Window window(s_windowPos, adjustedSize, s_name);
    window.setFullscreenOn();
#else
    Window window(s_windowPos, s_windowSize, s_name);
#endif
    if (!window.isInit()) {
        return;
    }

    Renderer renderer(window);
    if (!renderer.isInit()) {
        return;
    }

    AudioManager audioManager;
    if (!audioManager.isInit()) {
        return;
    }

    window.setIcon(
        Icon(SDL_Incbin(ICON_BMP))
    );

    loadStartScreen(window, renderer);
    play(window, renderer, audioManager);
}

void Game::loadStartScreen(Window& window, Renderer& renderer) {
    renderer.setDrawColor({ 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE });
    renderer.clear();

    Text textLoadScreen(renderer.getSdlRenderer(), modding::anotherFontSize, { 0, 0 });
    Text textAuthor(renderer.getSdlRenderer(), 16, { 0, 0 }, true);

    textLoadScreen.setText(getCheckpointText(CheckPoint::LOADING_TEXT));
    textLoadScreen.positionCenter();
    textLoadScreen.render(window.getSize());

    textAuthor.setText("by IPOleksenko");
    textAuthor.render(window.getSize());

    renderer.present();
}

void Game::updateText(Text& text, const int& yPos) {
    switch (static_cast<CheckPoint>(yPos)) {
    case CheckPoint::IDLE: {
        text.setText(getCheckpointText(CheckPoint::IDLE));
        text.positionReset();
        text.animationStop();
        text.setColor(255, 255, 255, 255);
        text.resize(modding::fontSize);
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
            text.resize(modding::anotherFontSize);
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
    default:
        break;
    }
}

void Game::play(Window& window, Renderer& renderer, AudioManager& audioManager) {
    modding::createDefaultLocalizationFile();
    modding::createDefaultFontFile();
    modding::createDecorDirectory();
    modding::loadCustomFontSize();
    LocalizationManager::instance().init();

    SDL_Event event = {};

    Background background(renderer.getSdlRenderer());
    Decor decor(renderer.getSdlRenderer());
    Road road(renderer.getSdlRenderer());
    Player player(renderer.getSdlRenderer());
    Scale scale(renderer.getSdlRenderer());
    End end(renderer.getSdlRenderer());

    Text text(renderer.getSdlRenderer(), modding::fontSize, { 0, 0 });
    Text endlessModeText(renderer.getSdlRenderer(), modding::fontSize, { 0, 0 });
    Text timerText(renderer.getSdlRenderer(), modding::fontSize, { 0, 0 });
    endlessModeText.positionTopRight();
    endlessModeText.setColor(200, 200, 200, 255);
    timerText.positionTopCenter();
    timerText.setColor(255, 255, 0, 255);

    Music soundtrack(SDL_Incbin(SOUND_WIND_WAV));
    Sfx reload(SDL_Incbin(SOUND_RELOAD_WAV));

    audioManager.play(soundtrack, AudioManager::TIMES_LOOP);
    audioManager.resume();

    bool isRunning = true;
    bool endlessMode = false;

    bool dimActive = true;
    bool dimFading = false;
    Uint32 dimFadeStart = 0;
    const Uint8 dimInitialAlpha = 120;

    Uint32 dimFadeDuration = 800;
    int textAnimDur = text.getAnimationDuration();
    if (textAnimDur > 0) {
        dimFadeDuration = static_cast<Uint32>(textAnimDur);
    }
    bool prevPlayerIsMove = false;

    int fingerCount = 0;
    Uint32 twoFingerStartTime = 0;

    static Uint64 timerStartTime = 0;
    int elapsedTime = 0;
    char timerBuffer[64];

    while (SDL_PollEvent(&event)) {}

    while (isRunning) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
            case SDL_QUIT: {
                isRunning = false;
                break;
            }
            case SDL_FINGERDOWN: {
                const SDL_TouchFingerEvent& touchEvent = event.tfinger;
                fingerCount++;

                if (touchEvent.x < 0.5f) {
                    player.increaseSpeed(Player::Move::Left, endlessMode);
                }
                else {
                    player.increaseSpeed(Player::Move::Right, endlessMode);
                }

                if (fingerCount == 2) {
                    twoFingerStartTime = SDL_GetTicks();
                }
                break;
            }

            case SDL_FINGERUP: {
                if (fingerCount > 0) fingerCount--;
                if (fingerCount < 2) {
                    twoFingerStartTime = 0;
                }
                break;
            }
            case SDL_KEYDOWN: {
                const SDL_KeyboardEvent &keyboardEvent = event.key;
                if (keyboardEvent.repeat == 0) {
                    switch (keyboardEvent.keysym.sym) {
                        case SDLK_ESCAPE: {
                            isRunning = false;
                            break;
                        }
#if !defined(__ANDROID__)
                            case SDLK_f: {
                                if (window.isFullscreen()) {
                                    window.setFullscreenOff();
                                }
                                else {
                                    window.setFullscreenOn();
                                }
                                break;
                            }
#endif
                        case SDLK_LEFT:
                        case SDLK_a: {
                            player.increaseSpeed(Player::Move::Left, endlessMode);
                            break;
                        }
                        case SDLK_RIGHT:
                        case SDLK_d: {
                            player.increaseSpeed(Player::Move::Right, endlessMode);
                            break;
                        }
                        case SDLK_SPACE: {
                            if (!player.getIsMove())
                                endlessMode = !endlessMode;
                            break;
                        }
                    }
                    break;
                }
            }
            case SDL_CONTROLLERDEVICEADDED: {
                int joystick_index = event.cdevice.which;
                if (SDL_IsGameController(joystick_index)) {
                    SDL_GameController* controller = SDL_GameControllerOpen(joystick_index);
                    if (controller) {
                        controllers.push_back(controller);
                        SDL_Log("Controller %d connected: %s", joystick_index, SDL_GameControllerName(controller));
                    }
                }
                break;
            }

            case SDL_CONTROLLERDEVICEREMOVED: {
                SDL_JoystickID joyId = event.cdevice.which;
                for (auto it = controllers.begin(); it != controllers.end(); ++it) {
                    if (SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(*it)) == joyId) {
                        SDL_GameControllerClose(*it);
                        controllers.erase(it);
                        SDL_Log("Controller %d disconnected", joyId);
                        break;
                    }
                }
                break;
            }
            case SDL_CONTROLLERBUTTONDOWN: {
                const SDL_ControllerButtonEvent& cbutton = event.cbutton;

                switch (cbutton.button) {
                case SDL_CONTROLLER_BUTTON_START: {
                    isRunning = false;
                    break;
                }
#if !defined(__ANDROID__)
                case SDL_CONTROLLER_BUTTON_Y: {
                    if (window.isFullscreen()) {
                        window.setFullscreenOff();
                    }
                    else {
                        window.setFullscreenOn();
                    }
                    break;
                }
#endif
                case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
                case SDL_CONTROLLER_BUTTON_X: {
                    player.increaseSpeed(Player::Move::Left, endlessMode);
                    break;
                }

                case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
                case SDL_CONTROLLER_BUTTON_B: {
                    player.increaseSpeed(Player::Move::Right, endlessMode);
                    break;
                }

                case SDL_CONTROLLER_BUTTON_A: {
                    if (!player.getIsMove())
                        endlessMode = !endlessMode;
                    break;
                }
                }
                break;
            }
            }
        }
    

        renderer.setDrawColor({ 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE });
        renderer.clear();

        player.move(endlessMode);

        bool curPlayerIsMove = player.getIsMove();
        if (dimActive && !dimFading && !prevPlayerIsMove && curPlayerIsMove) {
            dimFading = true;
            dimFadeStart = SDL_GetTicks();
        }
        prevPlayerIsMove = curPlayerIsMove;

        background.render(window.getSize(), player.getPosY());
        decor.render(window.getSize(), player.getPosY());
        road.render(window.getSize(), player.getPosY());
        player.updateAnimation();
        player.render(window.getSize(), endlessMode);

        if (endlessMode) {
            if (!player.getIsMove()) {
                timerStartTime = SDL_GetPerformanceCounter();
            }

            Uint64 now = SDL_GetPerformanceCounter();
            Uint64 freq = SDL_GetPerformanceFrequency();
            Uint64 ticks = now - timerStartTime;

            Uint64 totalSeconds = ticks / freq;
            Uint64 hours = totalSeconds / 3600;
            Uint64 minutes = (totalSeconds / 60) % 60;
            Uint64 secs = totalSeconds % 60;

            Uint64 fracTicks = ticks % freq;
            Uint64 frac = (fracTicks * 100000) / freq;

            snprintf(timerBuffer, sizeof(timerBuffer),
                "%02" PRIu64 ":%02" PRIu64 ":%02" PRIu64 ".%05" PRIu64,
                hours, minutes, secs, frac);

            timerText.setText(timerBuffer);
            timerText.render(window.getSize());
        }
        else {
            timerStartTime = 0;
        }

        if (player.hasLost()) {
            updateText(text, player.getPosY());
            if (!endlessMode
                || player.getPosY() < static_cast<int>(CheckPoint::A_START)) {
                text.render(window.getSize());
            }
            scale.render(
                window.getSize(),
                player.getSpeed(),
                player.getSpeedMin(),
                player.getSpeedMax(),
                player.getPosY(),
                endlessMode
            );
            renderer.present();

            SDL_Delay(200);

            player.reset();
            scale.reset();
            decor.reset();
            endlessModeText.animationStop();

            dimActive = true;
            dimFading = false;
            prevPlayerIsMove = false;

            fingerCount = 0;
            twoFingerStartTime = 0;

            timerBuffer[0] = '\0';

            renderer.setDrawColor({ 0x00, 0x00, 0x00, SDL_ALPHA_OPAQUE });
            SDL_Rect fillRect = { 0, 0, window.getSize().x, window.getSize().y };
            SDL_RenderFillRect(renderer.getSdlRenderer(), &fillRect);

            renderer.present();

            if (const int channel = audioManager.play(
                reload, 0, AudioManager::CHANNEL_UNDEFINED
            ); channel != AudioManager::CHANNEL_UNDEFINED) {
                while (Mix_Playing(channel) == SDL_TRUE) {
                    SDL_Delay(16);
                }
            }

            while (SDL_PollEvent(&event)) {}
            continue;
        }
        
        if (!endlessMode) {
            if (player.getPosY() >= static_cast<int>(CheckPoint::FINAL_START)) {
                if (player.isFinalAnimationFinished()) {
                    end.render(window.getSize());
                    if (player.getPosY() >= static_cast<int>(CheckPoint::FINAL_STOP))
                        isRunning = false;
                }
            }
        }

        if (!player.getIsMove()) {
            if (fingerCount == 2 && twoFingerStartTime > 0) {
                Uint32 heldTime = SDL_GetTicks() - twoFingerStartTime;
                if (heldTime >= 2000) {
                    endlessMode = !endlessMode;
                    twoFingerStartTime = 0;
                }
            }
        }

        if (dimActive) {
            SDL_Renderer* sdlR = renderer.getSdlRenderer();
            SDL_SetRenderDrawBlendMode(sdlR, SDL_BLENDMODE_BLEND);

            Uint8 alpha = dimInitialAlpha;

            if (dimFading) {
                Uint32 elapsed = SDL_GetTicks() - dimFadeStart;
                if (elapsed >= dimFadeDuration) {
                    dimActive = false;
                    dimFading = false;
                    alpha = 0;
                }
                else {
                    float t = static_cast<float>(elapsed) / static_cast<float>(dimFadeDuration);
                    float inv = 1.0f - t;
                    alpha = static_cast<Uint8>(inv * static_cast<float>(dimInitialAlpha));
                }
            }

            if (alpha > 0) {
                SDL_SetRenderDrawColor(sdlR, 0, 0, 0, alpha);
                SDL_Rect fullRect = { 0, 0, window.getSize().x, window.getSize().y };
                SDL_RenderFillRect(sdlR, &fullRect);
            }
        }

        updateText(text, player.getPosY());
        if (!endlessMode
            || player.getPosY() < static_cast<int>(CheckPoint::A_START)) {
            text.render(window.getSize());
        }

        if (endlessMode) {
            endlessModeText.setText(getCheckpointText(CheckPoint::ENDLESS_MODE_TEXT));
            endlessModeText.render(window.getSize());
            if (player.getIsMove()
                && static_cast<CheckPoint>(player.getPosY()) == CheckPoint::BEGIN)
                endlessModeText.animationStart(false);
        }

        scale.render(
            window.getSize(),
            player.getSpeed(),
            player.getSpeedMin(),
            player.getSpeedMax(),
            player.getPosY(),
            endlessMode
        );

        renderer.present();
        SDL_Delay(16);
    }

    audioManager.stop();
}

Game::~Game() {
    for (auto controller : controllers) {
        SDL_GameControllerClose(controller);
    }
    controllers.clear();
    TTF_Quit();
    Mix_Quit();
    IMG_Quit();
    SDL_Quit();

    m_isInit = false;
}
