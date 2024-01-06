//
// Created by PC1 on 04/01/2024.
//

#ifndef POS_SERVER_H
#define POS_SERVER_H
#include "libraries.h"
#include "shared.h"


typedef struct threadData {
    bool start;
    bool koniec;
    pthread_mutex_t mutex;
} THREAD_DATA;


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
    char msg[256];
    THREAD_DATA threadData;
} PLAYER;




typedef struct game {
    int time;
    int** board;
    PLAYER* players;
    uint8_t game_ConnectedPlayers;
    uint8_t game_MaxPlayerMoves;
    enum SIZE_MODE game_Size;
    THREAD_DATA threadData;
}GAME;

typedef struct server {
    int numberOfClients;
    int serverSocket;
    int opt;
    int port;
    socklen_t clientAddrLen;
    struct sockaddr_in serverAddress;
    struct sockaddr_in client_addr[PLAYERS_MAX];
    pthread_t clients[PLAYERS_MAX];

}SERVER;
int server();
void initServer(SERVER * server);
void createSocket(SERVER * server);
void * client_handler(void * arg);
void* consume(void* thread_data);
#endif //POS_SERVER_H
