#pragma once

#include "tile.hpp"
#include "tile_shapes.hpp"
#include <deque>
#include <vector>
#include <functional>

class TileQueue {
  private:
    std::deque<Tile> tiles; // Using a deque to access tiles by index

  public:
    TileQueue(size_t nbPlayers);

    // Draw the next tile from the queue.
    Tile drawTile();

    // Draw a tile at a specific index and place every tile before that at the back of the queue.
    Tile exchangeTile(int index); // Using int for user interface

    // Get constant references to the next n tiles without removing them from the queue.
    std::vector<std::reference_wrapper<const Tile>> nextTiles(size_t n = 5) const; // Returns constant references without // exposing internal structure

    // Print the next n tiles in the exchange queue.
    void printExchangeQueue(const std::vector<std::reference_wrapper<const Tile>> &tiles) const;
};