#include "Game.h"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <sstream>
#include <string>

const int BOARD_SIZE = 15;
const char AGUA = 'o';
struct Ship {
    int size;
    char type;
};

const Ship SHIPS[] = {
    {5, 'P'},  // PORTAAVIONES
    {4, 'B'},  // BUQUE
    {4, 'B'},  // BUQUE
    {3, 'S'},  // SUBMARINO
    {3, 'S'},  // SUBMARINO
    {1, 'L'},  // LANCHA
    {1, 'L'},  // LANCHA
    {1, 'L'}   // LANCHA 
};

const string ANSI_COLOR_RED = "\x1b[31m";
const string ANSI_COLOR_BLUE = "\x1b[34m";
const string ANSI_COLOR_RESET = "\x1b[0m";

const string Game::imprimirBarco() {
    string boat =
        "                                              _____            __\n"
        "                                             |     |    __    |__|      |-._\n"
        "                                   ___       |_____|___|__|___|__|__    |-._|\n"
        "                                  |___|      |     | . . . . . . . |    |\n"
        "                          ________|___|______|_____|_______________|____|__\n"
        "                          \\    |.---------------------------------       /\n"
        "                           \\---'  o        o        o        o   '------/\n"
        "                            \\__________________________________________/\n"
        "~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \\________________________________________/~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
        "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    return boat;
}

Game::Game() {
    srand(time(NULL));
}

const string Game::play(int playerChoice) {
    if (playerChoice == 1) {
        initializeBoard(playerBoard);
    }

    initializeBoard(aiBoard);
    isPlayerTurn = rand() % 2 == 0;
    string ret = "El primero en jugar es " + string(isPlayerTurn == 0 ? "la IA" : "el Jugador") + "\n";
    return ret;
}

const string Game::checkTurn() {
    string state = "";

    if (!playerBoard.allShipsDestroyed() && !aiBoard.allShipsDestroyed()) {
        state += "TURNO DE: " + string(isPlayerTurn == 0 ? "IA" : "Jugador") + "\n";

        if (isPlayerTurn == 0) {
            state += aiTurn(playerBoard);
        }
        state += "\t\t\t\t\t\t\t TU TABLERO:\n";
        state += printBoard(playerBoard, true);
        state += "\t\t\t\t\t\t\t TABLERO DE IA\n";
        state += printBoard(aiBoard, false);

    } else if (playerBoard.allShipsDestroyed()) {
        state += "0";
    } else {
        state += "1";
    }

    return state;
}

bool Game::Cell::hasShip() const {
    return entity != AGUA;
}

Game::Cell::Cell() : entity(AGUA), isHit(false) {}

Game::Board::Board() : grid() {}

void Game::Board::placeShip(int x, int y, int direccion, int size, char ship) {
    for (int i = 0; i < size; i++) {
        if (direccion == 0) {
            grid[x + i][y].entity = ship;
        } else {
            grid[x][y + i].entity = ship;
        }
    }
}

bool Game::Board::isValidPosition(int x, int y) const {
    return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
}

void Game::Board::markHit(int x, int y) {
    grid[x][y].isHit = true;
}

bool Game::Board::allShipsDestroyed() const {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            if (grid[i][j].hasShip() && !grid[i][j].isHit) {
                return false;
            }
        }
    }
    return true;
}

void Game::generateRandomPosition(int& x, int& y, int maxX, int maxY) {
    x = rand() % maxX;
    y = rand() % maxY;
}

void Game::generateRandomPosition(int& x, int& y) {
    generateRandomPosition(x, y, BOARD_SIZE, BOARD_SIZE);
}

const string Game::printBoard(const Board& board, bool showShips) {
    string ret = "\t\t\t\t     0   1   2   3   4   5   6   7   8   9  10  11  12  13  14\n";
    ret += "\t\t\t\t     ----------------------------------------------------------\n";

    for (int i = 0; i < BOARD_SIZE; i++) {
        ret += "\t\t\t\t" + to_string(i) + " " + string(i < 10 ? " |" : "|");

        for (int j = 0; j < BOARD_SIZE; j++) {
            // Verifica si la celda ha sido golpeada
            if (board.grid[i][j].isHit) {
                if (board.grid[i][j].hasShip()) {
                    ret += " " + ANSI_COLOR_RED + board.grid[i][j].entity + ANSI_COLOR_RESET + "  ";
                } else {
                    // Tiro fallido
                    ret += " " + ANSI_COLOR_BLUE + AGUA + ANSI_COLOR_RESET + "  ";
                }
            } else {
                if (showShips && board.grid[i][j].hasShip()) {
                    ret += " " + string(1, board.grid[i][j].entity) + "  ";
                } else {
                    ret += " .  ";
                }
            }
        }
        ret += " \n";
    }
    ret += "\n";

    return ret;
}

void Game::placeRandomShip(Board& board, int size, char ship) {
    int x, y, direccion;
    bool validPosition = false;

    while (!validPosition) {
        generateRandomPosition(x, y);
        direccion = rand() % 2;
        validPosition = true;

        for (int i = 0; i < size; i++) {
            if (direccion == 0) {
                if (!board.isValidPosition(x + i, y) || board.grid[x + i][y].hasShip()) {
                    validPosition = false;
                    break;
                }
            } else {
                if (!board.isValidPosition(x, y + i) || board.grid[x][y + i].hasShip()) {
                    validPosition = false;
                    break;
                }
            }
        }
    }

    board.placeShip(x, y, direccion, size, ship);
}

const string Game::placementInfo(int index) {
    string ret = printBoard(playerBoard, true) + "\nIngresa la coordenada [columna] [Fila] para posicionar el barco (";
    ret += SHIPS[index].type;
    ret += "-" + to_string(SHIPS[index].size) + "): ";

    return ret;
}

const string Game::placePlayerShip(int typeShip, int x, int y, int direccion) {
    bool validPosition = false;
    for (int i = 0; i < SHIPS[typeShip].size; i++) {
        if (direccion == 0) {
            if (!playerBoard.isValidPosition(x + i, y) || playerBoard.grid[x + i][y].hasShip()) {
                validPosition = false;
                return "Posicion invalida";
            }
        } else {
            if (!playerBoard.isValidPosition(x, y + i) || playerBoard.grid[x][y + i].hasShip()) {
                validPosition = false;
                return "Posicion invalida";
            }
        }
    }

    playerBoard.placeShip(x, y, direccion, SHIPS[typeShip].size, SHIPS[typeShip].type);
    return "ok";
}

void Game::initializeBoard(Board& board) {
    board = Board();
    int length = sizeof(SHIPS) / sizeof(SHIPS[0]);
    for (int i = 0; i < length; i++) {
        placeRandomShip(board, SHIPS[i].size, SHIPS[i].type);
    }
}

const string Game::playerTurn(int x, int y) {
    if (!aiBoard.isValidPosition(x, y) || aiBoard.grid[x][y].isHit) {
        return "\t\t\t\t\t  Disparo inválido. Inténtalo de nuevo.\n[" + to_string(y) + "," + to_string(x) + "]\n";
    }

    aiBoard.markHit(x, y);

    string ret = "\t\t\t\t\t\t Disparaste en [" + to_string(y) + "," + to_string(x) + "] y ";

    if (aiBoard.grid[x][y].hasShip()) {
        cout << "El usuario golpeo un barco de la IA" << endl;
        ret += "¡Has alcanzado un barco enemigo!\n";
    } else {
        cout << "El usuario fallo" << endl;
        ret += "Fallaste el tiro\n";
    }

    isPlayerTurn = !isPlayerTurn;
    return ret;
}

const string Game::aiTurn(Board& board) {
    int x, y;
    bool validShot = false;

    while (!validShot) {
        generateRandomPosition(x, y);
        validShot = !board.grid[x][y].isHit;
    }

    board.markHit(x, y);

    string ret = "\t\t\t\t\t\t  La IA displaro en [" + to_string(y) + "," + to_string(x) + "]\n";

    if (board.grid[x][y].hasShip()) {
        cout << "La IA alcanzo un barco enemigo" << endl;
        ret += "\t\t\t\t\t\t  La IA ha alcanzado uno de tus barcos.\n";
    } else {
        cout << "La IA fallo" << endl;
        ret += "\t\t\t\t\t\t  La IA ha disparado al agua.\n";
    }

    isPlayerTurn = !isPlayerTurn;
    return ret;
}
