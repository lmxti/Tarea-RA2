#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
using namespace std;

int main() {
    
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "\n Socket creation error \n";
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        cout << "\nInvalid address/ Address not supported \n";
        return -1;
    }
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "\nConnection Failed \n";
        return -1;
    }

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        valread = read(sock, buffer, sizeof(buffer));
        cout << "Server : " << buffer << endl;

        memset(buffer, 0, sizeof(buffer));
        cout << "Client : ";
        cin.getline(buffer, 1024);
        send(sock, buffer, strlen(buffer), 0);
        
    }
    return 0;
}