//
// Created by PC1 on 04/01/2024.
//

#ifndef POS_CLIENT_H
#define POS_CLIENT_H

#include "shared.h"
#include "terminal_display.h"
typedef struct klient {
    bool zadalZnak;
    GAME_TERMINAL * boards;
    enum ROLE role;
    char msg[BUFFER_LENGTH];
    int sockfd;
    pthread_mutex_t mutex;
} KLIENT;

int client();
#endif //POS_CLIENT_H
