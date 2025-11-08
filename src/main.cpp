#include "utils.hpp"
#include "game.hpp"
#include <iostream>
#include <limits>

int main(void) {
    clearTerminal();

    std::cout << "Welcome to The Devil's Plan!" << std::endl;

    int nbPlayers = 0;

    while (true) {
        std::cout << "Please enter number of players (2 - 9): ";
        std::cin >> nbPlayers;

        if (std::cin.fail() || nbPlayers < 2 || nbPlayers > 9) {
            std::cin.clear(); // Clear fail state
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Empty the input buffer
            std::cout << "Invalid input. Try again." << std::endl << std::endl;
            continue;
        }
        
        if (std::cin.peek() != '\n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Empty the input buffer
            std::cout << "Extra characters detected. Try again." << std::endl << std::endl;
            continue;
        }

        break;
    }

    Game game(static_cast<size_t>(nbPlayers)); // Safe to cast after validation

    game.play();

    return 0;
}