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

std::pair<size_t, size_t> getCoordinatesInput(size_t max) {
    while (true) {
        int x, y;
        std::cout << "Enter coordinates (X Y): ";
        std::cin >> x >> y;

        if (std::cin.fail()
            || x < 0 || x >= static_cast<int>(max)
            || y < 0 || y >= static_cast<int>(max)) {
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

        return {static_cast<size_t>(x), static_cast<size_t>(y)};
    }
}