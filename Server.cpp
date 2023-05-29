#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <thread>
#include <regex>

#include "Game.h"

using namespace std;

const char* invalidInput = "Input invalido intenta denuevo: ";
const char* requestPlay = "TURNO DEL JUGADOR: \n\t\t\t\t\t Ingresa la coordenada [Columna][Fila] en la que atacar: ";
const char* welcome = "\n\t\t\t\t\tBIENVENIDO A BATTLESHIP\n\n\t\t\t\t\t      Presiona: \n\t\t\t\t 1.Posicionar barcos aleatoriamente \n\t\t\t\t 2.Seleccionar posicion de barcos \n\t\t\t\t\t\t>>";

// Función para analizar y convertir los números de un buffer de entrada

int* parseNumbers(const char* buffer) {
    string input(buffer);

    size_t spacePos = input.find(' ');

    if (spacePos == string::npos) {
        if (input.find_first_not_of("0123456789") == string::npos) {
            int* numbers = new int[1];
            try {
                numbers[0] = stoi(input);
                return numbers;
            } catch (const invalid_argument& e) {
                delete[] numbers;
                return nullptr;
            }
        }

    } else if (spacePos > 0 && spacePos < input.length() - 1) {
        string firstNum = input.substr(0, spacePos);
        string secondNum = input.substr(spacePos + 1);

        if (firstNum.find_first_not_of("0123456789") == string::npos &&
            secondNum.find_first_not_of("0123456789") == string::npos) {
            int* numbers = new int[2];

            try {
                numbers[0] = stoi(firstNum);
                numbers[1] = stoi(secondNum);
                return numbers;
            } catch (const invalid_argument& e) {
                delete[] numbers;
                return nullptr;
            }
        }
    }

    return nullptr; // Devuelve nullptr para entrada inválida
}

// Función para leer y validar la entrada del cliente
int readAndValidateInput(int socket, int*& numbers) {
    char buffer[1024] = {0};
    int bytesRead = read(socket, buffer, sizeof(buffer));

    if (bytesRead == 0) {  // Client disconnected
        return -1;
    }

    numbers = parseNumbers(buffer);

    if (numbers == nullptr) {
        send(socket, invalidInput, strlen(invalidInput), 0);
        return 0;
    }

    return 1;
}

// Función para mostrar un mensaje cuando un cliente se desconecta
void signalStop(char client_ip[]) {
    cout << "El cliente: " << client_ip << " se desconecto" << endl;
}


// Función que representa el hilo de juego para un cliente
void gameThread(int server_fd, int new_socket, sockaddr_in address) {
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(address.sin_addr), client_ip, INET_ADDRSTRLEN);

    cout << "El cliente: [" << client_ip << "] se unio al servidor, iniciando juego nuevo..." << endl;

    Game game;

    string aux = "";
    aux = game.imprimirBarco();
    send(new_socket, aux.c_str(), strlen(aux.c_str()), 0);
    send(new_socket, welcome, strlen(welcome), 0);
    // Menu de inicio "Battleship"

    int option = 0;

    while (true) {
        int* auxNums;
        int res = readAndValidateInput(new_socket, auxNums);

        if (res == -1) {
            signalStop(client_ip);
            return;
        } else if (res == 1) {
            if (auxNums[0] == 1 || auxNums[0] == 2) {
                option = auxNums[0];
                break;
            } else {
                send(new_socket, invalidInput, strlen(invalidInput), 0);
            }
        }
    }



    if (option == 2) {
        int i = 0;
        int* auxNums;

        while (i < 8) {
            int x, y, dir;
            aux = "";

            while (true) {
                string placement = game.placementInfo(i);
                send(new_socket, placement.c_str(), strlen(placement.c_str()), 0);

                do {
                    int res = readAndValidateInput(new_socket, auxNums);

                    if (res == -1) {
                        signalStop(client_ip);
                        return;
                    } else if (res == 1) {
                        y = auxNums[0];
                        x = auxNums[1];
                        break;
                    }
                } while (true);

                send(new_socket, "Dirección (0 - horizontal, 1 - vertical): ", strlen("Dirección (0 - horizontal, 1 - vertical): "), 0);

                do {
                    int res = readAndValidateInput(new_socket, auxNums);

                    if (res == -1) {
                        signalStop(client_ip);
                        return;
                    } else if (res == 1) {
                        dir = auxNums[0];
                        break;
                    }
                } while (true);

                if (strcmp((aux = game.placePlayerShip(i, x, y, dir)).c_str(), "ok") == 0) {
                    break;
                } else {
                    send(new_socket, aux.c_str(), strlen(aux.c_str()), 0);
                }
            }
            i++;
        }

         
        aux = game.play(2);
        cout << "Juego [" << client_ip << "]: " + aux; 
        send(new_socket, aux.c_str(), strlen(aux.c_str()), 0);

    } else {
        aux = game.play(1);
        cout << "Juego [" << client_ip << "]: " + aux;
        send(new_socket, aux.c_str(), strlen(aux.c_str()), 0);
    }



    aux = game.checkTurn();
    send(new_socket, aux.c_str(), strlen(aux.c_str()), 0);
    while (true) {
        if (game.isPlayerTurn) {
            send(new_socket, requestPlay, strlen(requestPlay), 0);

            int* auxNums;
            int x, y;
            do {
                int res = readAndValidateInput(new_socket, auxNums);

                if (res == -1) {
                    signalStop(client_ip);
                    return;
                } else if (res == 1) {
                    y = auxNums[0];
                    x = auxNums[1];
                    break;
                }
            } while (true);
            
            cout << "Juego [" << client_ip << "]: ";
            aux = game.playerTurn(x, y);
            send(new_socket, aux.c_str(), strlen(aux.c_str()), 0);
        }
        if(!game.isPlayerTurn){ 
            cout << "Juego [" << client_ip << "]: ";
        }
        aux = game.checkTurn();

        if(strcmp(aux.c_str(), "1") == 0){
            send(new_socket, "Has Ganado! \n Goodbye...", strlen("Has Ganado! \n Goodbye..."), 0);
            cout << "El usuario a ganado en el Juego[" << client_ip  << "]";
            return;
        }else if(strcmp(aux.c_str(), "0") == 0){
            send(new_socket, "Has Perdido :(\n Goodbye...", strlen("Has Perdido :(\n Goodbye..."), 0);
            cout << "La IA a ganado en el Juego[" << client_ip  << "]";
            return;
        }else{
            send(new_socket, aux.c_str(), strlen(aux.c_str()), 0);

        }
    }
    signalStop(client_ip);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Argumentos insuficientes. Para usar: ./Servidor Puerto" << endl;
        return 1;  
    } else {
        string portPattern = R"(\b\d{1,5}\b)";               

        regex portRegex(portPattern);

        if (!regex_match(string(argv[1]), portRegex)) {
            cout << "formato Puerto invalido" << endl;
            return 1;
        }
    }

    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(stoi(argv[1]));

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    cout << "Preparado y escuchando" << endl;

    do {
        int new_socket;
        struct sockaddr_in p_address = address;

        if ((new_socket = accept(server_fd, (struct sockaddr*)&p_address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        thread myThread(gameThread, server_fd, new_socket, p_address);

        myThread.detach();
    } while (true);

    return 0;
}
