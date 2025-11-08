#pragma once

#include "player.hpp"
#include "tile.hpp"
#include <utility>

/**
Types of cells on the board.
Can be empty, grass, stone, or various bonus types.
*/
enum CellType {
    EMPTY,
    GRASS,
    STONE,
    BONUS_EXCHANGE,
    BONUS_STONE,
    BONUS_ROBBERY
};

/** 
Representation of a cell on the board.
Contains a reference to its type and a pointer to the owning player, if any.
*/
struct Cell {
    CellType type;
    Player *owner; // Pointer to the player who owns this cell, nullptr if unowned
};

// Representation of the game board.
class Board {
  private:
    const size_t size;
    Cell **grid; // Dynamic 2D array of Cells

  public:
    Board(size_t nbPlayers);
    ~Board();

    void setup(size_t nbPlayers);

    size_t getSize() const { return size; };
    const Cell& getCell(std::pair<size_t, size_t> coords) const { return grid[coords.first][coords.second]; }

    void setCell(std::pair<size_t, size_t> coords, CellType type, Player *owner);

    void placeBonus(CellType bonusType);

    bool canPlaceTile(std::pair<size_t, size_t> coords, const Tile &tile, const Player &player) const;
    void placeTile(std::pair<size_t, size_t> coords, const Tile &tile, Player *player);

    void display() const;
};