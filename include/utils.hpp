#pragma once

#include "player.hpp"
#include <iostream>
#include <utility>
#include <string>

// Clear the terminal and print the game header.
void clearTerminal();

// Helper struct to apply color formatting to output streams.
struct Colorize {
    PlayerColor color;
    explicit Colorize(PlayerColor c) : color(c) {}
};

// Create a Colorize object for the given PlayerColor.
Colorize colorize(PlayerColor color);

// Reset the output stream's color formatting to default.
std::ostream &resetColor(std::ostream &os);

/**
Overload the << operator to apply color formatting when a Colorize object is
sent to the stream.
*/
std::ostream &operator<<(std::ostream &os, const Colorize &c);

// Convert A-z to 0-51
int letterToIndex(char c);

// Get coordinate input from the user within specified bounds.
std::pair<size_t, size_t> getCoordinatesInput(size_t max);

int getIntegerInputInRange(const std::string &prompt, int min, int max);