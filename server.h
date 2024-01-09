//
// Created by PC1 on 04/01/2024.
//

#ifndef POS_SERVER_H
#define POS_SERVER_H

#include "shared.h"


typedef struct server {
    int socket;
    int vysledneSkore[PLAYERS_MAX];
    bool koniec;
    pthread_mutex_t mutex;
} SERVER;

typedef struct client {
    int id;
    int writeSocket;
    int readSocket;
    char data[BUFFER_LENGTH];
    SERVER * server;
} CLIENT_STRUCT_SERVER;

typedef struct timer {
    int time;
    CLIENT_STRUCT_SERVER * clients[PLAYERS_MAX];
    SERVER * server;
} TIMER;


int sendMsg(int writeSocket, char* buffer);
int receiveMsg(int readSocket, char* buffer);
int serverConfig(void);
int clientConfig(int serverSocket, int* writeSocket, int* readSocket);
void* clientF(void* arg);
void* timerF(void* arg);
int server(void);

#endif //POS_SERVER_H
