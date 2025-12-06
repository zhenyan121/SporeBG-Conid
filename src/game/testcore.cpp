// main.cpp
#include "GameSession.h"
#include <iostream>

int main() {
    GameSession game;
    if (!game.initialize()) {
        std::cerr << "Failed to initialize game.\n";
        return -1;
    }

    // 简单控制台循环
    while (true) {
        std::cout << "Current player: " 
                  << (game.getCurrentPlayer() == PlayerID::P1 ? "P1" : "P2") << "\n";
        

        std::cout << "input piece (row col): ";
        int r, c;
        std::cin >> r >> c;
        game.handleCoordinateInput(r, c);

        game.printBoard();
    }
}