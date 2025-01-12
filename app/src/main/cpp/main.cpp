#include <cstdlib>
#include <application/game.hpp>


int main(int, char*[]) {
    Game game = Game();

    if(game.isInit()) {
        game.run();
    }

    return EXIT_SUCCESS;
}
