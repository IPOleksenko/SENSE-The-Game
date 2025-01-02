#include <window/window.hpp>
#include <assets/assets.hpp>

WINDOW::WINDOW() {
    SDL_Surface* icon = SDL_LoadBMP_RW(SDL_Incbin(ICON_BMP), SDL_TRUE);

    if (icon) {
        SDL_SetWindowIcon(window, icon); // Set window icon
        SDL_FreeSurface(icon);          // Free memory
    }
    else {
        std::cerr << "SDL_LoadBMP Error: " << SDL_GetError() << std::endl;
    }

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        exit(1);
    }

    if (!window) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(1);
    }

    if (!renderer) {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        exit(1);
    }
}

WINDOW::~WINDOW() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
