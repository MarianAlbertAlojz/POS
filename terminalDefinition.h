//
// Created by PC1 on 30/12/2023.
//

#ifndef POS_TERMINALDEFINITION_H
#define POS_TERMINALDEFINITION_H
#define MAX_NAME_LENGTH 20
#define MAX_NUMBER_LENGTH 5
#define NULL_CHAR_LENGTH 1
#define BUFFER_LENGTH 301
#define TURN_X  1
#define TURN_O  0
#define GAME_SIZE_MIN 3
#define GAME_SIZE_MAX 9
#define TIMER_MIN 5
#define TIMER_MAX 20
#define HOST 0
#define GUEST 1
#define START 0
#define PICK 1
#define WAIT 2
#define SIZE 3
#define SYMBOL 4
#define END 5
#define STARTING_MODE 10

// definicie pre nastavenie hry
#define MAX_PLAYER_MOVES
//server.h
#define PLAYERS_MAX 2
#define CONVERT_TO_SECOND 60
//client.h

static char * instruction[] = {"START","PICK","WAIT","SIZE","SYMBOL","END"};
static char * roles[] = {"HOST","GUEST"};
static char x_symbol = 'X';
static char o_symbol = 'O';
static char seprator_symbol = ';';
static char empty[] = "     ";
static char vertical = '|';
static char horizontal[] = "-----";

extern char *endMsg;
extern char *sendMsg;

void printError(char *str);

// SERVER/CLIENT --> treba vytvorit header subor
// nasledujuce struct,enum su aj pre klienta aj pre server

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

void initPlayerServer(PLAYER * server);
void initPlayerClient_1(PLAYER * client_1);
void initGameSettings(GAME *game, TIMER *game_Timer, enum SIZE_MODE gameSize, enum PLAYER_MOVE_MODE moveMode, enum TIMER_MODE timerMode);
void initGame(GAME * game, TIMER * game_Timer, TERMINAL_UI * game_TerminalPrint);
void initTimer(TIMER * game_Timer);
void * timerThread(void * arg);
void waitOneSecond();
void createBoard(TERMINAL_UI *terminalPrint);
void createBoardClient_1(TERMINAL_UI * terminalPrint);
void createBoardClient_2(TERMINAL_UI * terminalPrint);
void printOnlyVerticalSection(int size);
void printMiddleSection(const int size, const int row);
void printSymbolSection(const TERMINAL_UI board, const int row);
void printColumnNumber(const int columnSize);
void printBoard(TERMINAL_UI *board);


#endif //POS_TERMINALDEFINITION_H
