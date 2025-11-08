#include "tile.hpp"
#include <iostream>

void Tile::print() const {
    for (const auto &row : shape) {
        for (uint8_t cell : row)
            std::cout << (cell ? "██" : "  ");
        std::cout << std::endl;
    }
}