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

        board.placeTile(coords, startingTile, &player);
        return;
    }
    
    // --- Display game state ---
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

    // --- Turn logic goes here ---
    size_t exchangeCoupons = player.getCoupons();
    std::cout << "1 - Take Tile" << std::endl;
    std::cout << "2 - Exchange (" << exchangeCoupons << " available)" << std::endl;

    int choiceBuffer = getIntegerInputInRange("> ", 1, 2);

    if (choiceBuffer == 2) {
        if (exchangeCoupons == 0)
            std::cout << "No exchange coupon available. Continuing with first tile in queue." << std::endl;
        else {
            // EXCHANGE LOGIC GOES HERE
            // REMOVE STONE OR PICK TILE IN QUEUE
        }
    }

    bool ready = false;
    while(!ready) {
        std::cout << "1 - Place Tile" << std::endl;
        std::cout << "2 - Rotate Tile" << std::endl;
        std::cout << "3 - Flip Tile" << std::endl;

        choiceBuffer = getIntegerInputInRange("> ", 1, 3);

        switch (choiceBuffer) {
            case 1:
                ready = true;
                break;
            case 2:
                currentTile.rotateClockwise();
                break;
            case 3:
                // FLIP LOGIC GOES HERE
                break;
        }
    }

    std::pair<size_t, size_t> coords = {0, 0};
    while (true) {
        coords = getCoordinatesInput(board.getSize());
        bool canPlace = board.canPlaceTile(coords, currentTile, player);
        if (canPlace)
            break;
        std::cout << "Cannot place starting tile at (" << coords.first << ", "
                  << coords.second << "). Try again." << std::endl;
    }

    board.placeTile(coords, currentTile, &player);

    size_t pendingStone = player.getStoneBonus();
    while (pendingStone > 0) {
        std::cout << "You have " << pendingStone << " pending stone bonus to use immediately." << std::endl;
        applyStoneBonus(player);
    }

    size_t pendingRobbery = player.getRobberyBonus();
    while (pendingRobbery > 0) {
        std::cout << "You have " << pendingRobbery << " pending robbery bonus to use immediately." << std::endl;
        applyRobberyBonus(player);
    }
    
    return;
}

void Game::applyStoneBonus(Player &player) {
    std::pair<size_t, size_t> coords = {0, 0};

    while (true) {
        coords = getCoordinatesInput(board.getSize());

        if (board.getCell(coords).type == EMPTY)
            break;
        else
            std::cout << "Cannot place stone tile at (" << coords.first
                      << ", " << coords.second << "). Try again." << std::endl;
    }

    board.setCell(coords, STONE, nullptr);

    player.useStoneBonus();
    std::cout << "Stone bonus used successfully at (" << coords.first << " " << coords.second << ")" << std::endl;
}

void Game::applyRobberyBonus(Player &player) {
    std::pair<size_t, size_t> coords = {0, 0};

    // while (true) {
    //     coords = getCoordinatesInput(board.getSize());

    //     if (board.getCell(coords).type == GRASS && board.getCell(coords).owner != &player)
    //         break;
    //     else
    //         std::cout << "Cannot rob tile at (" << coords.first
    //                   << ", " << coords.second << "). Try again." << std::endl;
    // }

    // board.setCell(coords, GRASS, &player);

    player.useStoneBonus();
    std::cout << "Stone bonus used successfully at (" << coords.first << " " << coords.second << ")" << std::endl;
}