#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <random>


using namespace std;
// Tamaño de tablero. [15]x[15]
const int BOARD_SIZE = 15;
// 1 PORTAAVION (P) de 5 casillas
const char PORTAAVIONES = 'P';
// 2 BUQUES (B) de 4 casillas
const char BUQUE = 'B';
// 2 SUBMARINO (S) de 3 casillas.
const char SUBMARINO ='S';
//  3 LANCHAS (L) de 1 casilla.
const char LANCHA = 'L';
//  Caracter que representa el agua
const char AGUA = '~';

// Colores de texto ANSI
const string ANSI_COLOR_RED = "\x1b[31m";
const string ANSI_COLOR_BLUE = "\x1b[34m";
const string ANSI_COLOR_RESET = "\x1b[0m";

// Estructura de cada celda del tablero.
struct Cell {
    // Entidad que representa
    char entity;
    // Estado de la celda
    bool isHit;
    // Verificacion de que celda sea una un barco
    bool hasShip() const { return entity != AGUA; }
    // Constructor de estructura "Cell" para establecer el agua y estado de la celda.
    Cell(): entity(AGUA), isHit(false) {}
};

// Estructura del tablero
struct Board {
    // Matriz bidimensional que representa las celdas individuales del tablero.
    Cell grid[BOARD_SIZE][BOARD_SIZE];
    // Constructor de la estructura Board que inicializa el constructor de cell para cada elemento de la matriz grid
    Board() : grid() {}

    /*-------------------  POSICIONAMIENTO DE BARCO -------------------*/
    void placeShip(int x, int y, int direccion, int size, char ship) {
        // Direccion horizontal
        if (direccion == 0) {
            for (int i = 0; i < size; i++) {
                grid[x][y + i].entity = ship;
            }
        } else {
            // Direccion vertical
            for (int i = 0; i < size; i++) {
                grid[x + i][y].entity = ship;
            }
        }
    }
    /*-------------------  AL DISPARAR -------------------*/
    bool isValidPosition(int x, int y) const {
         return x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE;
    }
    void markHit(int x, int y){
        grid[x][y].isHit = true;
    }
    /*------------------- VERIFICACION DE BARCOS EN TABLERO ------------------- */
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
// Funcion para generar una posicion aleatoria del barco en [x,y] que no sobrepase el tablero 
void generateRandomPosition(int& x, int& y, int maxX, int maxY) {
    x = rand() % maxX;
    y = rand() % maxY;
}
// Funcion para que la IA dispare aleatoriamente en el tablero
void generateRandomPosition(int& x, int& y) {
    x = rand() % BOARD_SIZE;
    y = rand() % BOARD_SIZE;
}
void printBoard(const Board& board, bool showShips = false) {
    cout << "\t\t\t\t     0   1   2   3   4   5   6   7   8   9  10   11  12  13  14" << endl;
    cout << "\t\t\t\t     ----------------------------------------------------------" << endl;
    for (int i = 0; i < BOARD_SIZE; i++) {
        cout << "\t\t\t\t";
        cout << i << " " << (i < 10 ? " |" : "|");
        for (int j = 0; j < BOARD_SIZE; j++) {
            // Verifica si la celda ha sido golpeada
            if (board.grid[i][j].isHit){
                if (board.grid[i][j].hasShip()){
                    cout <<  " " << ANSI_COLOR_RED << board.grid[i][j].entity << ANSI_COLOR_RESET << "  ";
                } 
                else {
                    // Tiro fallido
                    cout <<  " " << ANSI_COLOR_BLUE << AGUA << ANSI_COLOR_RESET << "  " ;
                }
            }
            else {
                if ( showShips && board.grid[i][j].hasShip()){
                    cout  << " " << board.grid[i][j].entity << "  " ;
                }
                else {
                    cout << " .  ";
                }
            }
        }
        cout << " " << endl;
    }
     cout << "" << endl;
}
// Funcion para el posicionamiento aleatorio de barcos
void placeRandomShip(Board& board, int size, char ship) {
    int x, y, direccion = rand() % 2;
    bool free = true;

    // Ejecucion hasta encontrar posicion valida para el barco(ship)
    do {
        do {
            generateRandomPosition(x, y, BOARD_SIZE - (direccion == 1 ? size : 0), BOARD_SIZE - (direccion == 0 ? size : 0));
        } while (board.grid[x][y].hasShip());

        for (int i = 1; i < size; i++) {
            if (direccion == 0) {
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
    // Posicionar barco en las coordenadas [x,y] y direccion.
    board.placeShip(x, y, direccion, size, ship);
}
// Funcion para el posicionamiento manual de barcos
void placeShipManually(Board& board, int size, char ship) {
    int x, y, direccion;
    bool free = true;
    do {
        cout << "Ingresa la coordenada [X Y] para posicionar el barco (" << ship << "): ";
        cin >> x >> y;
        cout << "Dirección (0 - horizontal, 1 - vertical): ";
        cin >> direccion;

        free = true; // Reiniciar el flag "free" en cada iteración

        if (direccion == 0) {
            for (int i = 0; i < size; i++) {
                if (y + i >= BOARD_SIZE || board.grid[x][y + i].hasShip()) {
                    free = false;
                    break;
                }
            }
        } else {
            for (int i = 0; i < size; i++) {
                if (x + i >= BOARD_SIZE || board.grid[x + i][y].hasShip()) {
                    free = false;
                    break;
                }
            }
        }
        if (!free) {
            system("clear");
            cout << "Posicion de " << ship << " no valida . Intente nuevamente." << endl;
            printBoard(board, true);
        }
    } while (!free);

    board.placeShip(x, y, direccion, size, ship);
    printBoard(board, true);
}

void initializeBoard(Board& board,int option) {
    // Posicionamiento aleatorio
    if (option == 1){
            placeRandomShip(board, 5, PORTAAVIONES);
        for (int i = 0; i < 2; i++) {
            placeRandomShip(board, 4, BUQUE);
        }

        for (int i = 0; i < 2; i++) {
            placeRandomShip(board, 3, SUBMARINO);
        }

        for (int i = 0; i < 3; i++) {
            placeRandomShip(board, 1, LANCHA);
        }
    // Seleccion de posiciones
    } else if(option == 2){
        placeShipManually(board, 5, PORTAAVIONES);
        for (int i = 0; i < 2; i++) {
            placeShipManually(board, 4, BUQUE);
        }

        for (int i = 0; i < 2; i++) {
            placeShipManually(board, 3, SUBMARINO);
        }

        for (int i = 0; i < 3; i++) {
            placeShipManually(board, 1, LANCHA);
        }
    } else {
        cout << "Opción inválida." << endl;
        return;
    }
}

void playerTurn(Board& board){
    int x, y;
    do {
        // Limpieza de entrada no valida
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
         cout << "\t\t\t\t\t  Ingresa la coordenada [Fila][Columna]: ";
         cin >> x >> y;
    } while(!board.isValidPosition(x,y) || board.grid[x][y].isHit);
    cout << "Disparaste en ["<< x << "," << y << "] y" << " ";
    if (board.grid[x][y].hasShip()) {
        cout << " Le diste a un barco!\n" << endl;
    } else {
        cout << " Fallaste el tiro.\n" << endl;
    }
    board.markHit(x, y);
}

void aiTurn(Board& board){
    int x, y;
    do{
        generateRandomPosition(x, y);
    } while ( board.grid[x][y].isHit);
    cout << "La IA disparo en las coordenadas ["<< x << "," << y << "] y " << "";
    if (board.grid[x][y].hasShip()){
        cout << "ha golpeado tu barco\n" << endl;
    }
    else{
        cout << "ha fallado su tiro \n" << endl;
    }
    board.markHit(x, y);
}

void imprimirBarco(){
    cout << "                                              _____            __\n";
    cout << "                                             |     |    __    |__|      |-._\n";
    cout << "                                   ___       |_____|___|__|___|__|__    |-._|\n";
    cout << "                                  |___|      |     | . . . . . . . |    |\n";
    cout << "                          ________|___|______|_____|_______________|____|__\n";
    cout << "                          \\    |.---------------------------------       /\n";
    cout << "                           \\---'  o        o        o        o   '------/\n";
    cout << "                            \\__________________________________________/\n";
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~ \\________________________________________/~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
}

int main() {
    srand(time(NULL));
    // Menu de inicio "Battleship"
    imprimirBarco();
    cout <<"\n\t\t\t\t\tBIENVENIDO A BATTLESHIP\n\n";
    cout << "\t\t\t\t\t      Presiona: \n\t\t\t\t 1.Posicionar barcos aleatoriamente \n\t\t\t\t 2.Seleccionar posicion de barcos \n\t\t\t\t\t\t>>" ;
    int option;
    cin >> option;
    system("clear");
    
    // Tablero Jugador
    Board playerBoard;
    // Tablero IA
    Board aiBoard;
    initializeBoard(playerBoard, option);
    initializeBoard(aiBoard,1);

    // Determinar quién comienza el primer turno de forma aleatoria
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 1);
    bool result = dis(gen) == 0;

    while (!playerBoard.allShipsDestroyed() && !aiBoard.allShipsDestroyed()) {
        cout << "TURNO DE: " << (result == 0 ? "IA" : "Jugador") << endl;
        cout << "\t\t\t\t\t\t\t TU TABLERO:" << endl;
        printBoard(playerBoard, true);
        cout << "\t\t\t\t\t\t\t TABLERO DE IA" << endl;
        printBoard(aiBoard, true);
    
        if (result) {
            // Turno del jugador
            cout << "\t\t\t\t\t\t\tTu turno: " << endl;
            playerTurn(aiBoard);
        } else {
            // Turno de la IA
            if (!aiBoard.allShipsDestroyed()) {
                cout << "\t\t\t\t\t\t\tTurno de IA:" << endl;
                aiTurn(playerBoard); 
            }   
        }
    result = !result; // Actualizar el valor de result para alternar los turnos
}

    if (playerBoard.allShipsDestroyed()) {
        cout << "Derrota!." << endl;
    } else {
        cout << "Victoria!" << endl;
    }
    return 0;
}




