//
// Created by PC1 on 04/01/2024.
//

#ifndef POS_SERVER_H
#define POS_SERVER_H

#include "shared.h"


typedef struct server {
    int socket;
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

typedef struct player {
    enum ROLE role;
    uint8_t playerMove;
    int score;
    char name[MAX_NAME_LENGTH];
    BOARD playerBoard;
    int serverSock;
    int playerSock;
    int id;
    char data[100];
    char msg[BUFFER_LENGTH];

} PLAYER;


int sendMsg (int writeSocket, char* buffer);
int receiveMsg (int readSocket, char* buffer);
uint8_t getTimerMode(uint8_t mode);
void setTimerMode(uint8_t time, int* gameTime);
void* hracF(void* arg);
void* timeF(void* arg);
int server(void);

#endif //POS_SERVER_H
