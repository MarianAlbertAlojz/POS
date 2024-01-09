//
// Created by PC1 on 30/12/2023.
//


#ifndef POS_TERMINAL_DISPLAY_H
#define POS_TERMINAL_DISPLAY_H

#include "shared.h"

#define MAX_NUMBER_LENGTH 5
#define NULL_CHAR_LENGTH 1

static char seprator_symbol = ';';
static char empty[] = "     ";
static char emptyLonger[] = "                    ";
static char vertical = '|';
static char horizontal[] = "-----";


typedef struct playerTerminal {
    int score;
    int playerMove;
    char name[MAX_NAME_LENGTH];
}PLAYER_TERMINAL;

typedef struct terminalDisplay {
    BOARD boardClients[PLAYERS_MAX];
    uint8_t boardSize;
}TERMINAL_UI;

typedef struct game_terminal {
    int game_moveMode;
    int game_timerMode;
    int game_sizeMode;
    int game_timerPrint;
    PLAYER_TERMINAL playersInfo[PLAYERS_MAX];
    TERMINAL_UI * game_TerminalPrint;
}GAME_TERMINAL;

int countNumberDigits(FIELD * policko);
void printSymbolFix(TERMINAL_UI terminalPrint, int row, int column, enum ROLE role);
void initGameSettings(GAME_TERMINAL *game,TERMINAL_UI *game_TerminalPrint ,int gameSize,int gameMove,int gameTimer,enum ROLE role);
void createBoard(TERMINAL_UI *terminalPrint);
void freeBoard(TERMINAL_UI *terminalPrint);
void createBoardHost(TERMINAL_UI * terminalPrint);
void createBoardClient(TERMINAL_UI * terminalPrint);
void printBlankSectionWithMessage( int size);
void printEmptyVertical( int size);
void printSymbol( TERMINAL_UI terminalPrint, int row, enum ROLE role);
void printOnlyVerticalSection(int size);
void printMiddleSection(int size, int row);
void printSymbolSection(TERMINAL_UI board,int row);
void printBoard(GAME_TERMINAL* game);
void printDoubleHeader(const char *label, PLAYER_TERMINAL * players);
void printMiddleHeader(const char *label, int time);


#endif //POS_TERMINAL_DISPLAY_H
