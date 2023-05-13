#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

const int BOARD_SIZE = 15;

// A single cell in the game board
struct Cell {
    char entity;
    bool isHit;
    bool hasShip() const { return entity != 'x'; }

    Cell() : entity('x'), isHit(false) {}
};

// The game board
struct Board {
    Cell grid[BOARD_SIZE][BOARD_SIZE];

    Board() : grid() {}

    // Place a ship on the board
    void placeShip(int x, int y, int dir, int size) {
        if (dir == 0) {
            for (int i = 0; i < size; i++) {
                grid[x][y + i].entity = size + 48;
            }
        } else {
            for (int i = 0; i < size; i++) {
                grid[x + i][y].entity = size + 48;
            }
        }
    }

    // Check if the given position is valid on the board
    bool isValidPosition(int x, int y) const {
        return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
    }

    // Mark a cell as hit
    void markHit(int x, int y) {
        grid[x][y].isHit = true;
    }

    // Check if all the ships on the board have been hit
    bool allShipsDestroyed() const {
        for (int i = 0; i < BOARD_SIZE; i++) {
            for (int j = 0; j < BOARD_SIZE; j++) {
                if (grid[i][j].hasShip() && !grid[i][j].isHit) {
                    return false;
                }
            }
        }
        return true;
    }
};

// Print the game board to the console
void printBoard(const Board& board, bool showShips = false) {
    cout << "Y <------------------------------¬" << endl;
    for (int i = 0; i < BOARD_SIZE; i++) {
        cout << i << " " << (i < 10 ? " " : "");

        for (int j = 0; j < BOARD_SIZE; j++) {
            if (board.grid[i][j].isHit) {
                if (board.grid[i][j].hasShip()) {
                    cout << board.grid[i][j].entity << " ";
                } else {
                    cout << "O ";
                }
            } else {
                if (showShips && board.grid[i][j].hasShip()) {
                    cout << board.grid[i][j].entity << " ";
                } else {
                    cout << ". ";
                }
            }
        }
        cout << "|" << endl;
    }
    cout << "   A B C D F G H I J K L M N Ñ O X" << endl;
}

// Generate a random position on the board
void generateRandomPosition(int& x, int& y, int maxX, int maxY) {
    x = rand() % maxX;
    y = rand() % maxY;
}

void generateRandomPosition(int& x, int& y) {
    x = rand() % BOARD_SIZE;
    y = rand() % BOARD_SIZE;
}

// Place a ship randomly on the board
void placeRandomShip(Board& board, int size) {
    int x, y, dir = rand() % 2;  // dir = 0 abajo, 1 derecha
    // Por juego se deben definir: 1 portaaviones (P),       2 buques (B),       2 submarinos (S),       3 lanchas (L).
    //                             Portaaviones 5 casillas,  buque 4 casillas,   submarino 3 casillas,   lancha 1 casilla.

    bool free = true;
    do {
        do {
            generateRandomPosition(x, y, BOARD_SIZE - (dir == 1 ? size : 0), BOARD_SIZE - (dir == 0 ? size : 0));
        } while (board.grid[x][y].hasShip());

        for (int i = 1; i < size; i++) {
            if (dir == 0) {
                if (board.grid[x][y + i].hasShip()) {
                    free = false;
                    break;
                }
            } else {
                if (board.grid[x + i][y].hasShip()) {
                    free = false;
                    break;
                }
            }
        }

    } while (!free);
    board.placeShip(x, y, dir, size);
}

// Initialize the game board with ships
void initializeBoard(Board& board) {
    placeRandomShip(board, 5);

    for (int i = 0; i < 2; i++) {
        placeRandomShip(board, 4);
    }
    for (int i = 0; i < 2; i++) {
        placeRandomShip(board, 3);
    }
    for (int i = 0; i < 3; i++) {
        placeRandomShip(board, 1);
    }
}

// Player's turn to guess a cell on the board
void playerTurn(Board& board) {
    int x, y;
    do {
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        string input;
        cout << "Enter the coordinates of your guess [X Y]: ";

        getline(cin, input);

        istringstream iss(input);
        if (!(iss >> x >> y)) {
            cout << "Invalid input" << endl;
            continue;
        }

    } while (!board.isValidPosition(x, y) || board.grid[x][y].isHit);


    cout << "You Shoot ["<< x << "," << y << "]" << endl;
    if (board.grid[x][y].hasShip()) {
        cout << "Hit!" << endl;
    } else {
        cout << "Miss." << endl;
    }
    board.markHit(x, y);
}

// AI's turn to guess a cell on the board
void aiTurn(Board& board) {
    int x, y;
    do {
        generateRandomPosition(x, y);
    } while (board.grid[x][y].isHit);

    cout << "They AI Shoot ["<< x << "," << y << "]" << endl;

    if (board.grid[x][y].hasShip()) {
        cout << "The AI hit your ship!" << endl;
    } else {
        cout << "The AI missed." << endl;
    }
    board.markHit(x, y);
}

// Play the game
void playGame() {
    srand(time(NULL));  // Seed the random number generator
    Board playerBoard;
    Board aiBoard;
    initializeBoard(playerBoard);
    initializeBoard(aiBoard);

    while (!playerBoard.allShipsDestroyed() && !aiBoard.allShipsDestroyed()) {
        cout << "Your board:" << endl;
        printBoard(playerBoard, true);
        cout << "AI's board:" << endl;
        printBoard(aiBoard);

        cout << "Your turn:" << endl;
        playerTurn(aiBoard);

        if (!aiBoard.allShipsDestroyed()) {
            cout << "AI's turn:" << endl;
            aiTurn(playerBoard);
        }
    }

    if (playerBoard.allShipsDestroyed()) {
        cout << "You lost." << endl;
    } else {
        cout << "You won!" << endl;
    }
}

int main() {
    playGame();
    return 0;
}