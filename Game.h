#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>

using namespace std;

class Game {
public:
    Game();
    const string play(int playerChoice);
    const string imprimirBarco();
    const string checkTurn();
    const string playerTurn(int x, int y);
    const string placePlayerShip(int index, int x, int y, int dir);
    const string placementInfo(int index);
    bool isPlayerTurn = false;
private:
    struct Cell {
        char entity;
        bool isHit;
        bool hasShip() const;
        Cell();
    };

    struct Board {
        Cell grid[15][15];
        Board();
        void placeShip(int x, int y, int direccion, int size, char ship);
        bool isValidPosition(int x, int y) const;
        void markHit(int x, int y);
        bool allShipsDestroyed() const;
    };

    void generateRandomPosition(int& x, int& y, int maxX, int maxY);
    void generateRandomPosition(int& x, int& y);
    void placeRandomShip(Board& board, int size, char ship);
    void initializeBoard(Board& board);
    const string printBoard(const Board& board, bool showShips = false);
    const string aiTurn(Board& board);
    
    Board playerBoard;
    Board aiBoard;
};

#endif // GAME_H
