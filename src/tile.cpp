#include "tile.hpp"
#include <algorithm>
#include <iostream>

Shape Tile::rotateShape(const Shape &shape) {
    size_t rows = shape.size();
    size_t cols = shape[0].size();

    Shape rotated(cols, std::vector<uint8_t>(rows));

    for (size_t i = 0; i < rows; ++i)
        for (size_t j = 0; j < cols; ++j)
            rotated[j][rows - 1 - i] = shape[i][j];

    return rotated;
}

Shape Tile::getShape() const {
    Shape shape = baseShape;

    for (int i = 0; i < rotation / 90; ++i)
        shape = rotateShape(shape);

    if (flipped)
        for (auto &row : shape)
            std::reverse(row.begin(), row.end());

    return shape;
}

void Tile::print() const {
    for (const auto &row : getShape()) {
        for (uint8_t cell : row)
            std::cout << (cell ? "██" : "  ");
        std::cout << std::endl;
    }
}