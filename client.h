//
// Created by PC1 on 04/01/2024.
//

#ifndef POS_CLIENT_H
#define POS_CLIENT_H
#include "libraries.h"

typedef struct klient {
    char msg[256];
    int sockfd;
    pthread_mutex_t mutex;
} KLIENT;

int client();
#endif //POS_CLIENT_H
