#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstring>
#include <iostream>
#include <regex>

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cout << "Argumentos insuficientes. Para usar: ./Client IP Puerto" << endl;
        return 1; 
    } else {
        string ipPattern = R"(\b(?:\d{1,3}\.){3}\d{1,3}\b)";  
        string portPattern = R"(\b\d{1,5}\b)";              

        regex ipRegex(ipPattern);
        regex portRegex(portPattern);

        if (!regex_match(string(argv[1]), ipRegex)) {
            cout << "formato IP invalido" << endl;
            return 1;
        }

        if (!regex_match(string(argv[2]), portRegex)) {
            cout << "formato Puerto invalido" << endl;
            return 1;
        }
    }

    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[10048] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        cout << "\n Error en la creacion del socket \n";
        return -1;
    }
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(stoi(argv[2]));

    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        cout << "\nIp no valida\n";
        return -1;
    }
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "\nFAllo la conexion \n";
        return -1;
    }

    while (true) {
        memset(buffer, 0, sizeof(buffer));
        if (read(sock, buffer, sizeof(buffer)) == 0) {
            cout << "Servidor desconectado" << endl;
            break;
        }

        cout << buffer;
        memset(buffer, 0, sizeof(buffer));

        cin.getline(buffer, 10048);
        send(sock, buffer, strlen(buffer), 0);

        system("clear");
    }
    return 0;
}
