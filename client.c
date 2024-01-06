//
// Created by PC1 on 04/01/2024.
//

#include "client.h"
#include <stdio.h>
#include "klient/my_socket.h"

int client() {
    const char* serverAddress = "frios2.fri.uniza.sk";
    short serverPort = 10001;
    MySocket* mySocket = createConnection(serverAddress, serverPort);
    sendData(mySocket, "Hello, server!");

    // Odeslání koncové zprávy
    sendEndMessage(mySocket);

    // Zničení pripojení
    destroyConnection(mySocket);
    return 0;
}