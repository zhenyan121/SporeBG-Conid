// main.cpp
#include "Game.h"
#include <iostream>

int main() {
    Game game;
    if (!game.initialize()) {
        std::cerr << "Failed to initialize game.\n";
        return -1;
    }

    // 简单控制台循环
    while (true) {
        std::cout << "Current player: " 
                  << (game.getCurrentPlayer() == PlayerID::P1 ? "P1" : "P2") << "\n";
        std::cout << "Action type (0=GROW, 1=MOVE, 2=SPORE): ";
        int act;
        std::cin >> act;
        game.setPlayerAction(static_cast<ActionType>(act));

        std::cout << "Select piece (row col): ";
        int r, c;
        std::cin >> r >> c;
        game.playerSelectPiece(r, c);

        std::cout << "Target position (row col): ";
        std::cin >> r >> c;
        game.executeAction(r, c);

        game.printBoard();
    }
}