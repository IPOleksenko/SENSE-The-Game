#pragma once

#include <SDL.h>
#include <string>
#include <vector>
#include <inttypes.h>

class Window;
class Renderer;
class AudioManager;
class Text;

class Game {
public:
    explicit Game();
    virtual ~Game();

    [[nodiscard]] bool isInit() const;
    void run() const;

    Game(const Game&) = delete;
    Game(Game&&) = delete;
    Game& operator=(const Game&) = delete;
    Game& operator=(Game&&) = delete;

private:
    static void play(Window& window, Renderer& renderer, AudioManager& audioManager);
    static void loadStartScreen(Window& window, Renderer& renderer);
    static void updateText(Text& text, const int& yPos);

    bool m_isInit;
    static std::vector<SDL_GameController*> controllers;

    static const std::string s_orientation;
    static const std::string s_name;
    static const SDL_Point s_windowSize;
    static const SDL_Point s_windowPos;
};
