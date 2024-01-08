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

bool receiveMsg_Client(int sockfd, char* buffer);
bool sendMsg_Client(int sockfd, char* buffer);
bool pohyb(char smer, TERMINAL_UI* terminalPrint, enum ROLE role);
const char* timerModeToString(enum TIMER_MODE mode);
const char* playerMoveModeToString(enum PLAYER_MOVE_MODE mode);
void printTimerModes(void);
void printPlayerMoveModes(void);
void clearMessage(KLIENT* klient);
void initGameSettings(GAME_TERMINAL* game, TERMINAL_UI* game_TerminalPrint, int gameSize, int gameMove, int gameTimer, enum ROLE role);
void pickRole(TERMINAL_UI* gameTerminal, KLIENT* klient, char* role);
void generuj(TERMINAL_UI* terminalPrint, enum ROLE role);
void concatenateArray(GAME_TERMINAL* gameTerminal, char resultString[], enum ROLE role);
void parseString(const char* concatenatedString, GAME_TERMINAL* gameTerminal, enum ROLE role);
int countScore(GAME_TERMINAL* gameTerminal, enum ROLE role);
void* klientF(void* arg);
int client(void);
#endif //POS_CLIENT_H
