#include <utils.hpp>

void clear_terminal(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif

    std::cout << "+------------------------------+" << std::endl;
    std::cout << "|       The Devil's Plan       |" << std::endl;
    std::cout << "+------------------------------+" << std::endl << std::endl;
}