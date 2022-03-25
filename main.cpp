#include "Game.h"

int main()
{

    // INIT SRAND
    std::srand(static_cast<unsigned>(time(NULL)));

    // INIT GAME ENGINE
    Game game;

    // GAME LOOP
    while (game.isRunning() && !game.getEndGame()) {

        // Update
        game.update();
        
        // Render
        game.render();
    }
    

    return 0;
}