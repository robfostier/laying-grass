#include "game.hpp"
#include "utils.hpp"
#include <iostream>

Game::Game(size_t nbPlayers)
    : board(nbPlayers), tileQueue(nbPlayers), nbPlayers(nbPlayers), players(), currentRound(0) {
    setup();
}

void Game::setup() {
    // Initialize players
    players.reserve(nbPlayers);
    for (size_t i = 0; i < nbPlayers; ++i) {
        std::string name = "Player " + std::to_string(i + 1);
        PlayerColor color = static_cast<PlayerColor>(i % 9); // Cycle colors
        players.emplace_back(name, color);
    }
}

void Game::play() {
    // board.placeTile({10, 10}, tileQueue.drawTile(), players[0]); // Initial tile placement for testing

    for (currentRound = 0; currentRound <= maxRounds; ++currentRound) {
        for (auto &player : players)
            playTurn(player);
    }

    playTurn(players[0]); // Single turn for testing
}

void Game::playTurn(Player &player) {
    clearTerminal();

    // Place starting tile in the first round
    if (currentRound == 0) {
        board.display();

        Tile startingTile = Tile(STARTING_TILE);

        std::cout << player.getName() << " - Starting location:" << std::endl
                  << std::endl;

        std::pair<size_t, size_t> coords = {0, 0};
        while(true) {
            coords = getCoordinatesInput(board.getSize());
            bool canPlace = board.canPlaceTile(coords, startingTile, player);
            if (canPlace)
                break;
            std::cout << "Cannot place starting tile at (" << coords.first << ", " << coords.second << "). Try again." << std::endl;
        }

        board.placeTile(coords, startingTile, player);
        return;
    }
    
    Tile currentTile = tileQueue.drawTile();

    std::cout << player.getName() << " - Round " << currentRound << ":" << std::endl
              << std::endl;

    std::cout << "Current Tile:" << std::endl;
    currentTile.print();
    std::cout << std::endl;

    std::cout << "Next Tiles:" << std::endl;
    auto nextTiles = tileQueue.nextTiles();
    tileQueue.printExchangeQueue(nextTiles);
    std::cout << std::endl;

    board.display();
}