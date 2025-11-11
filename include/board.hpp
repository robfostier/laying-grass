#pragma once

#include "player.hpp"
#include "tile.hpp"
#include <utility>
#include <optional>

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
Contains a reference to its type,  a pointer to the owning player if any, and a printing character.
*/
struct Cell {
    CellType type;
    Player *owner; // Pointer to the player who owns this cell, nullptr if unowned
    std::string printSymbol = "";
};

/** 
Representation of a placed tile on the board.
Contains the tile, its coordinates and a pointer to its owner.
*/
struct PlacedTile {
  Tile tile;
  std::pair<size_t, size_t> coords;
  Player *owner;
  bool bStealable; // To protect starting tiles from robbery bonus
};

// Representation of the game board.
class Board {
  private:
    const size_t size;
    Cell **grid; // Dynamic 2D array of Cells
    std::vector<PlacedTile> placedTiles;

  public:
    Board(size_t nbPlayers);
    ~Board();

    void setup(size_t nbPlayers);

    size_t getSize() const { return size; };
    const std::vector<PlacedTile>& getPlacedTiles() const { return placedTiles; }
    Cell& getCell(std::pair<size_t, size_t> coords) { return grid[coords.first][coords.second]; }
    const Cell& getCell(std::pair<size_t, size_t> coords) const { return grid[coords.first][coords.second]; }

    void setCell(std::pair<size_t, size_t> coords, CellType type, Player *owner);

    void placeBonus(CellType bonusType);

    bool canPlaceTile(std::pair<size_t, size_t> coords, const Tile &tile, const Player &player, bool bIsStartingTile) const;
    bool canPlaceTileAnywhere(const Tile &tile, const Player &player) const;
    void placeTile(std::pair<size_t, size_t> coords, const Tile &tile, Player *player, bool bStealable);

    std::optional<Tile> stealTile(std::pair<size_t,size_t> target, Player* newOwner);

    void display() const;
};