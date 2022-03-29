#include <iostream>
#include "Game.h"

int main(int argc, char** argv)
{
    // messes with the screen width and height. fixme really, should be able to dynamically resize...
    Game game(960*2/3,1080*2/3);
    game.run();

    return 0;
}
