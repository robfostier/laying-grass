#include "utils.hpp"
#include "game.hpp"
#include <iostream>
#include <limits>

int main(void) {
    clearTerminal();

    std::cout << "Welcome to The Devil's Plan!" << std::endl;

    int nbPlayers = getIntegerInputInRange("Please enter number of players (2 - 9): ", 2, 9);
    Game game(static_cast<size_t>(nbPlayers)); // Safe to cast after validation

    game.play();

    return 0;
}