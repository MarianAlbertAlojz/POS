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
    bool koniec;
    pthread_mutex_t mutex;
} CLIENT_STRUCT_CLIENT;

typedef struct receive {
    int readSocket;
    CLIENT_STRUCT_CLIENT * client;
} RECEIVE;

typedef struct send {
    int writeSocket;
    CLIENT_STRUCT_CLIENT * client;
} SEND;



int sendMsg_Client(int sockfd, char* buffer);
int receiveMsg_Client(int sockfd, char* buffer);
bool pohyb(char smer, TERMINAL_UI* terminalPrint, enum ROLE role);
const char* timerModeToString(enum TIMER_MODE mode);
const char* playerMoveModeToString(enum PLAYER_MOVE_MODE mode);
void printTimerModes(void);
void printPlayerMoveModes(void);
void initGameSettings(GAME_TERMINAL* game, TERMINAL_UI* game_TerminalPrint, int gameSize, int gameMove, int gameTimer, enum ROLE role);
void pickRole(TERMINAL_UI* gameTerminal, CLIENT_STRUCT_CLIENT* klient, char* role);
void generuj(TERMINAL_UI* terminalPrint, enum ROLE role);
void concatenateArray(GAME_TERMINAL* gameTerminal, char resultString[], enum ROLE role);
void parseString(const char* concatenatedString, GAME_TERMINAL* gameTerminal, enum ROLE role);
int countScore(GAME_TERMINAL* gameTerminal, enum ROLE role);
int client(void);
#endif //POS_CLIENT_H
