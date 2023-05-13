#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "Board.h"
#include <cstring>
#include <iostream>
#include <thread>
using namespace std;

void threadFunction(int server_fd, int new_socket, sockaddr_in address) {
    char buffer[1024] = {0};
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(address.sin_addr), client_ip, INET_ADDRSTRLEN);

    cout << "El cliente: " << client_ip << " se unio al servidor" << endl;

    send(new_socket, "Bienvenido al server", strlen("Bienvenido al server"), 0);
    int i = 0;

    while (true) {
        memset(buffer, 0, sizeof(buffer));

        if (read(new_socket, buffer, sizeof(buffer)) == 0) {
            // Client disconnected
            break;
        }
        
        cout << "Client : " << buffer << endl;
        // memset(buffer, 0, sizeof(buffer));
        // cin.getline(buffer, 1024);

        sprintf(buffer, "%d", i);
        send(new_socket, buffer, strlen(buffer), 0);
        cout << "Server : " << buffer << endl;

        i++;
    }

    cout << "El cliente: " << client_ip << " se desconecto" << endl;
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    const char* hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(8080);

    // Forcefully attaching socket to the port 8080
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
        // Copy the client address to a new struct
        struct sockaddr_in p_address = address;

        if ((new_socket = accept(server_fd, (struct sockaddr*)&p_address, (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Create a new thread
        thread myThread(threadFunction, server_fd, new_socket, p_address);

        // Wait for the thread to finish
        myThread.detach();
    } while (true);
    
    return 0;
}
