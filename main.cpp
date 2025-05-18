#include "GameOfLife.hpp"
#include <ctime>
#include <iostream>

int main() {
    std::srand(static_cast<unsigned>(time(nullptr)));

    GameOfLife game;          // default size 40×20
    char choice;

    std::cout << "CONWAY'S GAME OF LIFE\n"
              << "1. Random seed\n"
              << "2. Load pattern from file\n"
              << "3. Exit\n"
              << "Choice: ";
    std::cin >> choice;

    switch (choice) {
        case '1': game.randomSeed(); break;
        case '2': {
            std::string f;
            std::cout << "File: "; std::cin >> f;
            if (!game.loadPattern(f)) return 1;
            break;
        }
        case '3': return 0;
        default:  std::cout << "Invalid choice!\n"; return 1;
    }

    game.run();
    return 0;
}
