#include "my_socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>



MySocket* createConnection(const char* hostName, short port) {
    MySocket* mySocket = (MySocket*)malloc(sizeof(MySocket));
    if (mySocket == NULL) {
        perror("Failed to allocate memory for MySocket");
        exit(EXIT_FAILURE);
    }

    // Inicializace soketu
    mySocket->connectSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (mySocket->connectSocket < 0) {
        perror("Error opening socket");
        free(mySocket);
        exit(EXIT_FAILURE);
    }

    // Získání informací o adrese hosta
    struct addrinfo hints, *result;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    int status = getaddrinfo(hostName, NULL, &hints, &result);
    if (status != 0) {
        fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(status));
        close(mySocket->connectSocket);
        free(mySocket);
        exit(EXIT_FAILURE);
    }

    // Nastavení parametrů pro připojení
    struct sockaddr_in serverAddress;
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    memcpy(&serverAddress.sin_addr, &((struct sockaddr_in*)result->ai_addr)->sin_addr, sizeof(struct in_addr));

    // Připojení ke serveru
    if (connect(mySocket->connectSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {
        perror("Error connecting");
        close(mySocket->connectSocket);
        free(mySocket);
        freeaddrinfo(result);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);

    return mySocket;
}

void destroyConnection(MySocket* mySocket) {
    if (mySocket != NULL) {
        close(mySocket->connectSocket);
        free(mySocket);
    }
}

void sendData(MySocket* mySocket, const char* data) {
    size_t data_length = strlen(data);
    char* buffer = (char*)calloc(data_length + 1, sizeof(char));
    memcpy(buffer, data, data_length);
    buffer[data_length] = '\0';

    ssize_t bytes_sent = send(mySocket->connectSocket, buffer, data_length + 1, 0);
    if (bytes_sent < 0) {
        perror("Error writing to socket");
        free(buffer);
        destroyConnection(mySocket);
        exit(EXIT_FAILURE);
    }

    free(buffer);
}

void sendEndMessage(MySocket* mySocket) {
    sendData(mySocket, ":end");
}

void receiveData(MySocket* mySocket) {
    char buffer[256];

    int bytes_received = recv(mySocket->connectSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0) {
        perror("Error reading from socket");
        destroyConnection(mySocket);
        exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0';
}
