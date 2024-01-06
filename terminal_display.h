//
// Created by PC1 on 30/12/2023.
//


#ifndef POS_TERMINAL_DISPLAY_H
#define POS_TERMINAL_DISPLAY_H
#include "shared.h"

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

typedef struct game_terminal {
    int game_move;
    int game_timer;
    int game_size;
    TERMINAL_UI * game_TerminalPrint;
}GAME_TERMINAL;

//void initPlayers(GAME_TERMINAL * game, char player1Name[MAX_NAME_LENGTH], char player2Name[MAX_NAME_LENGTH]);
void initGameSettings(GAME_TERMINAL *game,TERMINAL_UI *game_TerminalPrint ,int gameSize,int gameMove,int gameTimer);
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
//void printTimeHeader(const char *label,const uint8_t * time);


#endif //POS_TERMINAL_DISPLAY_H
