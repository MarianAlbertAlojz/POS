//
// Created by PC1 on 04/01/2024.
//

#ifndef POS_SERVER_H
#define POS_SERVER_H
#include "libraries.h"
#include "shared.h"
#include "server/pos_sockets/active_socket.h"

typedef struct thread_data {
    pthread_mutex_t mutex;
    pthread_cond_t is_full;
    pthread_cond_t is_empty;

    short port;
    ACTIVE_SOCKET* my_socket;
} THREAD_DATA;


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
