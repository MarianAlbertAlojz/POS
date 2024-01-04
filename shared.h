//
// Created by PC1 on 04/01/2024.
//

#ifndef POS_SHARED_H
#define POS_SHARED_H

#include "libraries.h"

#define MAX_NAME_LENGTH 10
#define PLAYERS_MAX 2
enum ROLE {
    CLIENT_1,
    CLIENT_2
};

enum SIZE_MODE {
    SIZE_4 = 4,
    SIZE_5 = 5
};

enum TIMER_MODE {
    MINUTE = 1,
    TWO_MINUTES = 2,
    THREE_MINUTES = 3,
    FOUR_MINUTES = 4
};

enum PLAYER_MOVE_MODE {
    MOVES_30 = 30,
    MOVES_35 = 35,
    MOVES_40 = 40
};

typedef struct field {
    int value;
}FIELD;

typedef struct board {
    FIELD ** policka;
}BOARD;

typedef struct terminalDisplay {
    BOARD boardClient_1;
    BOARD boardClient_2;
    uint8_t boardSize;
}TERMINAL_UI;

typedef struct player {
    enum ROLE role;
    uint8_t playerMove;
    int score;
    char name[MAX_NAME_LENGTH];
    BOARD playerBoard;
}PLAYER;

// toto skor pre server.h
typedef struct timer {
    uint8_t gameTimer_ActualTime_Seconds;
    enum TIMER_MODE gameTimer_Mode;// moze byt max 4 minuty lebo uint8 je 255
    pthread_mutex_t timer_Mutex;

}TIMER;

typedef struct game {
    PLAYER players[PLAYERS_MAX];
    uint8_t game_ConnectedPlayers;
    uint8_t game_MaxPlayers;
    uint8_t game_MaxPlayerMoves;
    enum SIZE_MODE game_Size;
    TIMER * game_Timer;
    TERMINAL_UI * game_TerminalPrint;
    pthread_mutex_t game_Mutex;
    pthread_cond_t server;
    pthread_cond_t client;
    pthread_cond_t timer;
}GAME;
#endif //POS_SHARED_H
