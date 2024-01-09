//
// Created by PC1 on 04/01/2024.
//

#ifndef POS_SHARED_H
#define POS_SHARED_H

#include "libraries.h"
#define MAX_NAME_LENGTH 10
#define PLAYERS_MAX 2
#define BUFFER_LENGTH 256
#define TO_SECONDS 60
#define PORT 10003
enum ROLE {
    HOST,
    CLIENT
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

#endif //POS_SHARED_H
