//
// Created by PC1 on 04/01/2024.
//
#include "terminal_display.h"
/*
 * Tu  bude zobrazovanie od Maja,malo by tu byt vsetko co chceme zobrazit
 * premysliet kolko rozmerov chceme mat(ci len 4x4 alebo aj 5x5),co vsetko sa bude zobrazovat
 *
 * */

int countNumberDigits(FIELD * policko){
    char strNumber[MAX_NUMBER_LENGTH + NULL_CHAR_LENGTH];
    int length = 0;
    snprintf(strNumber,MAX_NUMBER_LENGTH + NULL_CHAR_LENGTH, "%d", policko->value);
    while (strNumber[length] != '\0') {
        length++;
    }
    return length;
}

void printSymbolFix(const TERMINAL_UI terminalPrint, const int row, const int column, bool boardClient_1) {
    FIELD field;
    if (boardClient_1) {
        field = terminalPrint.boardClient_1.policka[row][column];
    } else {
        field = terminalPrint.boardClient_2.policka[row][column];
    }

    switch (countNumberDigits(&field)) {
        case 1:
            printf("  %d  ", field.value);
            break;
        case 2:
            printf("  %d ", field.value);
            break;
        case 3:
            printf(" %d ", field.value);
            break;
        case 4:
            printf(" %d", field.value);
            break;
        case 5:
            printf("%d", field.value);
            break;
    }
}
void createBoard(TERMINAL_UI * terminalPrint)
{
    createBoardClient_1(terminalPrint);
    createBoardClient_2(terminalPrint);
}

void freeBoard(TERMINAL_UI * terminalPrint) {
    for (int i = 0; i < terminalPrint->boardSize; ++i) {
        free(terminalPrint->boardClient_1.policka[i]);
        free(terminalPrint->boardClient_2.policka[i]);
    }
    free(terminalPrint->boardClient_1.policka);
    free(terminalPrint->boardClient_2.policka);
}

void createBoardClient_1(TERMINAL_UI *terminalPrint) {
    terminalPrint->boardClient_1.policka = ((FIELD **) calloc(terminalPrint->boardSize, sizeof(FIELD *)));
    for (int i = 0; i < terminalPrint->boardSize; ++i) {
        terminalPrint->boardClient_1.policka[i] = (FIELD *) calloc((terminalPrint->boardSize), sizeof(FIELD));
    }
    for (int riadok = 0; riadok < terminalPrint->boardSize; ++riadok) {
        for (int stlpec = 0; stlpec < terminalPrint->boardSize; ++stlpec) {
            terminalPrint->boardClient_1.policka[riadok][stlpec].value = 0;
        }
    }
}

void createBoardClient_2(TERMINAL_UI *terminalPrint) {
    terminalPrint->boardClient_2.policka = ((FIELD **) calloc(terminalPrint->boardSize, sizeof(FIELD *)));
    for (int i = 0; i < terminalPrint->boardSize; ++i) {
        terminalPrint->boardClient_2.policka[i] = (FIELD *) calloc((terminalPrint->boardSize), sizeof(FIELD));
    }
    for (int riadok = 0; riadok < terminalPrint->boardSize; ++riadok) {
        for (int stlpec = 0; stlpec < terminalPrint->boardSize; ++stlpec) {
            terminalPrint->boardClient_2.policka[riadok][stlpec].value = 0;
        }
    }
}

void printBlankSectionWithMessage(const int size)
{
    bool printOne = false;
    bool printSecond = false;
    for (int column = 0; column < size-2; ++column) {
        printf("%s",empty );
        //printf("%s",empty );
    }


    for (int column = 0; column < size-2; ++column) {
        printf("%s",empty );
        //printf("%s",empty );
    }
}

void printEmptyVertical(const int size)
{
    for (int column = 0; column < size; ++column) {
        printf("%s",empty );
        if(column != size - 1){
            printf("%c",vertical);
        }
    }
}

void printSymbol(const TERMINAL_UI terminalPrint, const int row, bool boardClient_1)
{
    for (int column = 0; column < terminalPrint.boardSize; ++column)
    {
        printSymbolFix(terminalPrint, row, column, boardClient_1);
        if(column != terminalPrint.boardSize - 1){
            printf("%c",vertical);
        }
    }
}

void printOnlyVerticalSection(const int size)
{
    printEmptyVertical(size);
    printBlankSectionWithMessage(size);
    printEmptyVertical(size);
    printf("\n");
}

void printMiddleSection(const int size,const int row)
{
    if(row != size - 1) {
        for (int k = 0; k < size; ++k) {
            if(k < size - 1){
                printf("%s%c",horizontal,vertical);
            }else {
                printf("%s",horizontal);
            }
        }
    }
}

void printSymbolSection(const TERMINAL_UI terminalPrint, const int row)
{
    if(row == terminalPrint.boardSize - 1) {
        printf("\n");
    }
    printSymbol(terminalPrint,row, true);//boardClient_1 == true znaci ci to je board clienta 1, v tomto pripade true
    printBlankSectionWithMessage(terminalPrint.boardSize);
    printSymbol(terminalPrint,row, false);//boardClient_1 == false znaci ci to je board clienta 1, v tomto pripade false a teda vieme ze to je board clienta 2 lebo mame len dva boardy
    if(row != terminalPrint.boardSize - 1) {
        printf("\n");
    }
}

void printColumnNumber(const int columnSize)
{
    printf("  ");
    for (int i = 1; i <= columnSize; ++i) {
        printf("%d%s",i,empty);
    }
    printf("\n");
}

void printBoard(GAME_TERMINAL * game)
{
    //printColumnNumber(game->game_TerminalPrint->boardSize);
    printf("\n");
    printOnlyVerticalSection(game->game_TerminalPrint->boardSize);
    for (int i = 0; i < game->game_TerminalPrint->boardSize ; ++i) {
        printSymbolSection(*game->game_TerminalPrint, i);
        printMiddleSection(game->game_TerminalPrint->boardSize, i);
        printBlankSectionWithMessage(game->game_TerminalPrint->boardSize);
        printMiddleSection(game->game_TerminalPrint->boardSize, i);
        if(i != game->game_TerminalPrint->boardSize - 2) {
            printf("\n");
        }
    }
    printOnlyVerticalSection(game->game_TerminalPrint->boardSize);
    printTimeHeader("time",&game->game_Timer->gameTimer_ActualTime_Seconds);
    //printDoubleHeader("klient",game->players);
    //printDoubleHeader("score",game->players);
    //printDoubleHeader("move",game->players);
}

/*void printDoubleHeader(const char *label, PLAYER * players) {
    if (strcmp(label,"score") == 0) {
        printf("%s%s: %-37d%s: %d\n",empty, label, players[CLIENT_1].score, label, players[CLIENT_2].score);
    }else if(strcmp(label,"move") == 0) {
        printf("%s%s: %-38u%s: %u\n",empty, label, players[CLIENT_1].playerMove, label, players[CLIENT_2].playerMove);
    }else {
        printf("%s%s: %-36s%s: %s\n",empty, label, players[CLIENT_1].name, label, players[CLIENT_2].name);
    }
}

void printTimeHeader(const char *label,const uint8_t * time) {
    printf("%s%s    %s: %u \n",emptyLonger, empty, label, *time);
}*/