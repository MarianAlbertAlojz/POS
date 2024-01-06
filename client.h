//
// Created by PC1 on 04/01/2024.
//

#ifndef POS_CLIENT_H
#define POS_CLIENT_H

#include "shared.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <time.h>
typedef struct klient {
    uint8_t playerMove;
    int score;
    char name[MAX_NAME_LENGTH];
    BOARD playerBoard;
    enum ROLE role;
    char msg[BUFFER_LENGTH];
    int sockfd;
    pthread_mutex_t mutex;
} KLIENT;

int client();
#endif //POS_CLIENT_H
