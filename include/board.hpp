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
    const Cell& getCell(size_t x, size_t y) const { return grid[x][y]; }

    void placeBonus(CellType bonusType);

    bool canPlaceTile(std::pair<size_t, size_t> coords, const Tile &tile, const Player &player) const;
    void placeTile(std::pair<size_t, size_t> coords, const Tile &tile, const Player &player);

    void display() const;
};