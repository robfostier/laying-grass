#include "board.hpp"
#include "utils.hpp"
#include <random>
#include <stdexcept>
#include <iostream>
#include <array>

Board::Board(size_t nbPlayers) : size(nbPlayers < 5 ? 20 : 30), grid(nullptr) {
    setup(nbPlayers);
}

Board::~Board() {
    for (size_t i = 0; i < size; ++i) {
        delete[] grid[i];
    }
    delete[] grid;
}

void Board::setup(size_t nbPlayers) {
    // Allocate grid
    grid = new Cell *[size];
    for (size_t i = 0; i < size; ++i) {
        grid[i] = new Cell[size];
        for (size_t j = 0; j < size; ++j) {
            grid[i][j].type = EMPTY;
            grid[i][j].owner = nullptr;
        }
    }

    // Place bonuses on the board
    size_t nbExchange = (nbPlayers * 3 + 1) / 2; // 1.5x per player (rounded up)
    size_t nbStone = (nbPlayers + 1) / 2;        // 0.5x per player (rounded up)
    size_t nbRobbery = nbPlayers;                // 1x per player

    for (size_t i = 0; i < nbExchange; ++i)
        placeBonus(BONUS_EXCHANGE);
    for (size_t i = 0; i < nbStone; ++i)
        placeBonus(BONUS_STONE);
    for (size_t i = 0; i < nbRobbery; ++i)
        placeBonus(BONUS_ROBBERY);
}

void Board::placeBonus(CellType bonusType) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist( 1, size - 2); // Exclude edges from placement

    const size_t MAX_ATTEMPTS = 100; // Prevent infinite loops
    size_t attempts = 0;
    size_t x, y;

    do {
        x = dist(gen);
        y = dist(gen);

        if (++attempts > MAX_ATTEMPTS)
            throw std::runtime_error("Board::placeBonus failed: no valid empty cell found");
    } while (grid[x][y].type != EMPTY ||
             grid[x + 1][y].type != EMPTY ||
             grid[x][y + 1].type != EMPTY ||
             grid[x - 1][y].type != EMPTY ||
             grid[x][y - 1].type != EMPTY);

    grid[x][y].type = bonusType;
}

// Display the board in the terminal.
void Board::display() const {
    for (size_t y = 0; y < size; ++y) {
        for (size_t x = 0; x < size; ++x) {
            switch (grid[x][y].type) {
            case EMPTY:
                std::cout << "・";
                break;
            case GRASS:
                if (!grid[x][y].owner) // Avoid segfault
                    throw std::runtime_error("Board::display: GRASS cell has no owner");

                std::cout << colorize(grid[x][y].owner->getColor())
                          << "██"
                          << resetColor;
                break;
            case STONE:
                std::cout << "\033[37m" // Light gray color
                          << "██"
                          << resetColor;
                break;
            case BONUS_EXCHANGE:
                std::cout << "Ｅ";
                break;
            case BONUS_STONE:
                std::cout << "Ｓ";
                break;
            case BONUS_ROBBERY:
                std::cout << "Ｒ";
                break;
            }
        }
        std::cout << std::endl;
    }
}

bool Board::canPlaceTile(std::pair<size_t, size_t> coords, const Tile &tile, const Player &player) const {
    const Shape &shape = tile.getShape();
    const bool isStartingTile = (shape == STARTING_TILE);

    const std::array<std::pair<int,int>,4> directions = {{{-1,0}, {1,0}, {0,-1}, {0,1}}};
    bool touchesOwnCell = false;

    for (size_t i = 0; i < shape.size(); ++i) {
        for (size_t j = 0; j < shape[0].size(); ++j) {
            if (!shape[i][j])
                continue;

            size_t x = coords.first + i;
            size_t y = coords.second + j;

            // Make sure tile is within board bounds
            if (x >= size || y >= size)
                return false;

            // Make sure overlaping cell is not grass or stone
            if (grid[x][y].type == GRASS || grid[x][y].type == STONE)
                return false;

            // Checking orthogonal neighbours
            for (const auto &dir : directions) {
                int newX = static_cast<int>(x) + dir.first;
                int newY = static_cast<int>(y) + dir.second;

                if (newX < 0 || newY < 0 || newX >= static_cast<int>(size) || newY >= static_cast<int>(size))
                    continue;

                const Player *owner = grid[newX][newY].owner;

                if (owner) {
                    // Return false if touching another player's cell
                    if (owner != &player)
                        return false;

                    // Mark that we are touching our territory
                    if (!isStartingTile)
                        touchesOwnCell = true;
                }
            }
        }
    }

    // Can place if it's the starting tile or touches own cell
    return isStartingTile || touchesOwnCell;
}

void Board::placeTile(std::pair<size_t, size_t> coords, const Tile &tile, Player *player) {
    const Shape &shape = tile.getShape();

    // Place the tile on the board
    for (size_t i = 0; i < shape.size(); ++i) {
        for (size_t j = 0; j < shape[i].size(); ++j) {
            // Skip empty parts of the tile
            if (!shape[i][j])
                continue;

            Cell &cell = grid[coords.first + i][coords.second + j];
            cell.type = GRASS;
            cell.owner = player;
        }
    }

    // Check orthogonal neighbours for bonuses (second loop to make sure tile is fully placed before checking neighbours)
    const std::array<std::pair<int,int>,4> directions = {{{-1,0}, {1,0}, {0,-1}, {0,1}}};
    for (size_t i = 0; i < shape.size(); ++i) {
        for (size_t j = 0; j < shape[i].size(); ++j) {
            if (!shape[i][j])
                continue;

            size_t x = coords.first + i;
            size_t y = coords.second + j;

            for (const auto &dir : directions) {
                int newX = static_cast<int>(x) + dir.first;
                int newY = static_cast<int>(y) + dir.second;

                if (newX < 0 || newY < 0 || newX >= static_cast<int>(size) || newY >= static_cast<int>(size))
                    continue;

                Cell &neighbourCell = grid[newX][newY];

                // Apply bonus effects
                switch (neighbourCell.type) {
                    case BONUS_EXCHANGE:
                        player->addCoupon();
                        neighbourCell.type = GRASS;
                        neighbourCell.owner = player;
                        break;
                    case BONUS_STONE:
                        player->addStoneBonus();
                        neighbourCell.type = GRASS;
                        neighbourCell.owner = player;
                        break;
                    case BONUS_ROBBERY:
                        player->addRobberyBonus();
                        neighbourCell.type = GRASS;
                        neighbourCell.owner = player;
                        break;
                    default:
                        break;
                }
            }
        }
    }
}

void Board::setCell(std::pair<size_t, size_t> coords, CellType type, Player *owner) {
    grid[coords.first][coords.second].type = type;
    grid[coords.first][coords.second].owner = owner;
}