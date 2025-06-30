#include "window.h"
#include "game.h"
#include "ship.h"
#include "asteroid.h"
#include <iostream>
#define NUMBER_OF_INITIAL_ASTEROIDS 10

auto main(int argc, char *argv[]) -> int {
    
    Game game;
    game.run();

  return 0;
}