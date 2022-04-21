#include <iostream>
#include "Game.h"

int main(int argc, char** argv)
{
    // messes with the screen width and height. fixme really, should be able to dynamically resize...
    Game game(16*90,9*90);
    game.run();

    return 0;
}
