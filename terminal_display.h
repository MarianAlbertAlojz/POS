//
// Created by PC1 on 30/12/2023.
//


#ifndef POS_TERMINAL_DISPLAY_H
#define POS_TERMINAL_DISPLAY_H
#include "shared.h"
#include "client.h"
#define MAX_NUMBER_LENGTH 5
#define NULL_CHAR_LENGTH 1
#define BUFFER_LENGTH 301
#define GAME_SIZE_MIN 3
#define GAME_SIZE_MAX 9

// definicie pre nastavenie hry
#define MAX_PLAYER_MOVES
//server.h

#define CONVERT_TO_SECOND 60
//client.h

static char seprator_symbol = ';';
static char empty[] = "     ";
static char emptyLonger[] = "                    ";
static char vertical = '|';
static char horizontal[] = "-----";
static char player1NameGlob[MAX_NAME_LENGTH];
static char player2NameGlob[MAX_NAME_LENGTH];

extern char *endMsg;
extern char *sendMsg;

void printError(char *str);

// SERVER/CLIENT --> treba vytvorit header subor
// nasledujuce struct,enum su aj pre klienta aj pre server

typedef struct game_terminal {
    //PLAYER players[PLAYERS_MAX];
    uint8_t game_ConnectedPlayers;
    uint8_t game_MaxPlayerMoves;
    enum SIZE_MODE game_Size;
    TIMER * game_Timer;
    TERMINAL_UI * game_TerminalPrint;
    pthread_mutex_t game_Mutex;
    pthread_cond_t server;
    pthread_cond_t client;
    pthread_cond_t timer;

}GAME_TERMINAL;

void initPlayers(GAME_TERMINAL * game, char player1Name[MAX_NAME_LENGTH], char player2Name[MAX_NAME_LENGTH]);
void initGameSettings(GAME_TERMINAL *game, TIMER *game_Timer, enum SIZE_MODE gameSize, enum PLAYER_MOVE_MODE moveMode, enum TIMER_MODE timerMode);
void initGame(GAME_TERMINAL * game, TIMER * game_Timer, TERMINAL_UI * game_TerminalPrint);
void initTimer(TIMER * game_Timer);
void * timerThread(void * arg);
void createBoard(TERMINAL_UI *terminalPrint);
void freeBoard(TERMINAL_UI *terminalPrint);
void createBoardClient_1(TERMINAL_UI * terminalPrint);
void createBoardClient_2(TERMINAL_UI * terminalPrint);
void printOnlyVerticalSection(int size);
void printMiddleSection(const int size, const int row);
void printSymbolSection(const TERMINAL_UI board, const int row);
void printColumnNumber(const int columnSize);
void printBoard(GAME_TERMINAL* game);
//void printDoubleHeader(const char *label, PLAYER * players);
void printTimeHeader(const char *label,const uint8_t * time);


#endif //POS_TERMINAL_DISPLAY_H
