#include "board.hpp"
#include "utils.hpp"
#include <random>
#include <stdexcept>
#include <iostream>
#include <array>
#include <set>

Board::~Board() {
    for (size_t i = 0; i < size; ++i) {
        delete[] grid[i];
    }
    delete[] grid;
}

void Board::setCell(std::pair<size_t, size_t> coords, CellType type, Player *owner) {
    grid[coords.first][coords.second].type = type;
    grid[coords.first][coords.second].owner = owner;
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

bool Board::canPlaceTile(std::pair<size_t, size_t> coords, const Tile &tile, const Player &player, bool bIsStartingTile) const {
    Shape shape = tile.getShape();

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
                    if (!bIsStartingTile)
                        touchesOwnCell = true;
                }
            }
        }
    }

    // Can place if it's the starting tile or touches own cell
    return bIsStartingTile || touchesOwnCell;
}

bool Board::canPlaceTileAnywhere(const Tile &tile, const Player &player) const {
    for (size_t x = 0; x < size; ++x) {
        for (size_t y = 0; y < size; ++y) {
            if (canPlaceTile({x, y}, tile, player, false))
                return true;
        }
    }
    return false;
}

void Board::placeTile(std::pair<size_t, size_t> coords, const Tile &tile, Player *player, bool bStealable) {
    Shape shape = tile.getShape();

    const std::array<std::string, 12> symbols = {"██", "██", "▒▒", "░░", "##", "[]", "@@", "&&", "$$", "++", "00", "OO"};
    std::set<std::string> neighbourSymbols;

    const std::array<std::pair<int,int>,4> directions = {{{-1,0}, {1,0}, {0,-1}, {0,1}}};

    // Check orthogonal neighbours for printing characters
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

                // Fill neighbourChars set
                if (neighbourCell.type == GRASS && !neighbourCell.printSymbol.empty())
                    neighbourSymbols.insert(neighbourCell.printSymbol);
            }
        }
    }

    // Chose available character for printing
    std::string availableSymbol = "██";
    for (std::string s : symbols) {
        if (neighbourSymbols.find(s) == neighbourSymbols.end()) {
            availableSymbol = s;
            break;
        }
    }

    // Place the tile on the board
    for (size_t i = 0; i < shape.size(); ++i) {
        for (size_t j = 0; j < shape[i].size(); ++j) {
            // Skip empty parts of the tile
            if (!shape[i][j])
                continue;

            Cell &cell = grid[coords.first + i][coords.second + j];
            cell.type = GRASS;
            cell.owner = player;
            cell.printSymbol = availableSymbol;
        }
    }

    // Recheck orthogonal neighbours for bonuses (second loop because needed to override with cells first)
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
                        neighbourCell.printSymbol = "Ｅ";
                        break;
                    case BONUS_STONE:
                        player->addStoneBonus();
                        neighbourCell.type = GRASS;
                        neighbourCell.owner = player;
                        neighbourCell.printSymbol = "Ｓ";
                        break;
                    case BONUS_ROBBERY:
                        player->addRobberyBonus();
                        neighbourCell.type = GRASS;
                        neighbourCell.owner = player;
                        neighbourCell.printSymbol = "Ｒ";
                        break;
                    default:
                        break;
                }
            }
        }
    }

    placedTiles.push_back({tile, coords, player, bStealable});
}

std::optional<Tile> Board::stealTile(std::pair<size_t, size_t> target, Player *newOwner) {
    // Go through every tile in the board
    for (auto it = placedTiles.begin(); it != placedTiles.end(); ++it) {
        PlacedTile &placedTile = *it;

        // Avoid starting tiles
        if (!placedTile.bStealable)
            continue;

        Shape shape = placedTile.tile.getShape();
        size_t posX = placedTile.coords.first;
        size_t posY = placedTile.coords.second;

        // Go through every cell covered by that tile
        for (size_t i = 0; i < shape.size(); ++i) {
            for (size_t j = 0; j < shape[i].size(); ++j) {
                if (!shape[i][j])
                    continue;

                if (posX + i == target.first && posY + j == target.second) {
                    Player *oldOwner = placedTile.owner;

                    // Player can't steal tile from itself
                    if (oldOwner == newOwner)
                        return std::nullopt;

                    // Remove stolen tile from the board
                    for (size_t i2 = 0; i2 < shape.size(); ++i2) {
                        for (size_t j2 = 0; j2 < shape[i2].size(); ++j2) {
                            if (!shape[i2][j2])
                                continue;

                            Cell &cell = grid[posX + i2][posY + j2];
                            cell.type = EMPTY;
                            cell.owner = nullptr;
                            cell.printSymbol = "";
                        }
                    }

                    Tile stolenTile = placedTile.tile;

                    placedTiles.erase(it);

                    return stolenTile;
                }
            }
        }
    }

    // No tile found there
    return std::nullopt;
}

// Display the board in the terminal.
void Board::display() const {
    std::vector<std::string> labels;

    for (size_t i = 0; i < size; ++i) {
        char32_t ch;
        if (i < 26)
            ch = 0xFF21 + i; // "Ａ" to "Ｚ"
        else
            ch = 0xFF41 + (i - 26); // "ａ" to "ｚ"

        // Converting UTF-32 to UTF-8
        std::string utf8;
        if (ch <= 0x7F)
            utf8.push_back(ch);
        else if (ch <= 0x7FF) {
            utf8.push_back(0xC0 | ((ch >> 6) & 0x1F));
            utf8.push_back(0x80 | (ch & 0x3F));
        } else if (ch <= 0xFFFF) {
            utf8.push_back(0xE0 | ((ch >> 12) & 0x0F));
            utf8.push_back(0x80 | ((ch >> 6) & 0x3F));
            utf8.push_back(0x80 | (ch & 0x3F));
        } else {
            utf8.push_back(0xF0 | ((ch >> 18) & 0x07));
            utf8.push_back(0x80 | ((ch >> 12) & 0x3F));
            utf8.push_back(0x80 | ((ch >> 6) & 0x3F));
            utf8.push_back(0x80 | (ch & 0x3F));
        }
        labels.push_back(utf8);
    }

    // First row
    std::cout << "     ";
    for (const auto &label : labels)
        std::cout << label;
    std::cout << std::endl;

    // Separator row
    std::cout << "   +-";
    for (size_t i = 0; i < size; ++i) 
        std::cout << "--";
    std::cout << "-+";
    std::cout << std::endl;

    for (size_t x = 0; x < size; ++x) {
        std::cout << labels[x] << " | ";
        for (size_t y = 0; y < size; ++y) {
            const Cell &cell = grid[x][y];

            switch (cell.type) {
            case EMPTY:
                std::cout << "・";
                break;
            case GRASS:
                std::cout << colorize(cell.owner->getColor())
                          << cell.printSymbol
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
            default:
                break;
            }
        }
        std::cout << " | " << labels[x];
        std::cout << std::endl;
    }

    // Separator row
    std::cout << "   +-";
    for (size_t i = 0; i < size; ++i) 
        std::cout << "--";
    std::cout << "-+";
    std::cout << std::endl;

    // Last row
    std::cout << "     ";
    for (const auto &label : labels)
        std::cout << label;

    std::cout << std::endl << std::endl;
}