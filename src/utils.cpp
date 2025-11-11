#include "utils.hpp"
#include <limits>

#ifdef _WIN32
#include <windows.h>
#endif

void clearTerminal(void) {
#ifdef _WIN32
    SetConsoleOutputCP(65001); // Set console to UTF-8 on Windows
#endif

    std::cout << "\033[2J\033[H"; // Clear terminal using ANSI escape codes

    // Header
    std::cout << "+--------------------------------+" << std::endl;
    std::cout << "| The Devil's Plan - 데블스 플랜 |" << std::endl;
    std::cout << "+--------------------------------+" << std::endl << std::endl;
}

Colorize colorize(PlayerColor color) {
    return Colorize(color);
}

std::ostream &resetColor(std::ostream &os) {
    return os << "\033[0m";
}

std::ostream& operator<<(std::ostream& os, const Colorize& c) {
    switch (c.color) {
        case PURPLE:
            return os << "\033[35m";
        case RED:
            return os << "\033[31m";
        case GREEN:
            return os << "\033[32m";
        case YELLOW:
            return os << "\033[33m";
        case BLUE:
            return os << "\033[34m";
        case MAGENTA:
            return os << "\033[95m";
        case CYAN:
            return os << "\033[36m";
        case TURQUOISE:
            return os << "\033[38;5;45m";
        case ORANGE:
            return os << "\033[38;5;208m";
        default:
            return os;
    }
}

int letterToIndex(char c) {
    if (c >= 'A' && c <= 'Z')
        return c - 'A';           // A–Z → 0–25
    if (c >= 'a' && c <= 'z')
        return 26 + (c - 'a');    // a–z → 26–51
    return -1;
}

std::pair<size_t, size_t> getCoordinatesInput(size_t max) {
    while (true) {
        std::string rowInput, colInput;
        std::cout << "Enter coordinates (row column): ";
        std::cin >> rowInput >> colInput;

        if (std::cin.fail() || rowInput.size() != 1 || colInput.size() != 1) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again." << std::endl
                      << std::endl;
            continue;
        }

        int x = letterToIndex(rowInput[0]);
        int y = letterToIndex(colInput[0]);

        // On letterToIndex failure
        if (x == -1 || y == -1) {
            std::cout << "Coordinates must be letters (A–Z or a–d). Try again." << std::endl << std::endl;
            continue;
        }

        if (x >= static_cast<int>(max) || y >= static_cast<int>(max)) {
            std::cout << "Coordinates out of range. Try again." << std::endl << std::endl;
            continue;
        }

        if (std::cin.peek() != '\n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Extra characters detected. Try again." << std::endl
                      << std::endl;
            continue;
        }

        return {static_cast<size_t>(x), static_cast<size_t>(y)};
    }
}

int getIntegerInputInRange(const std::string &prompt, int min, int max) {
    while (true) {
        std::cout << prompt;
        
        int x;
        std::cin >> x;

        if (std::cin.fail() || x < min || x > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Try again." << std::endl
                      << std::endl;
            continue;
        }

        if (std::cin.peek() != '\n') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Extra characters detected. Try again." << std::endl
                      << std::endl;
            continue;
        }
        
        return x;
    }
}