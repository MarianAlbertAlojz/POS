#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>
#include "terminalDefinition.h"

/*
 * Tu  bude logika od Andreja,teda pohyb hore,dolu,vpravo,vlavo
 * premysliet:
 *           -ze hrac je aj server aj hrac teda ze prvy hrac bude server aj hrac
 *
 * */

void vypis(int velkost, int pole[][velkost]) {
    for (int i = 0; i < velkost; ++i) {
        for (int j = 0; j < velkost; ++j) {
            printf("%d  ", pole[i][j]);
        }
        printf("\n");
    }
}

void inicializuj(int velkost, int pole[][velkost]) {
    for (int i = 0; i < velkost; ++i) {
        for (int j = 0; j < velkost; ++j) {
            pole[i][j] = 0;
        }
    }
}

void generuj(TERMINAL_UI *terminalPrint, enum ROLE role) {
    int suradnicaX;
    int suradnicaY;
    int *p_valueOfField;

    do {
        suradnicaX = rand() % terminalPrint->boardSize;
        suradnicaY = rand() % terminalPrint->boardSize;

        if (role == CLIENT_1) {
            p_valueOfField = &terminalPrint->boardClient_1.policka[suradnicaX][suradnicaY].value;
        } else if (role == CLIENT_2) {
            p_valueOfField = &terminalPrint->boardClient_2.policka[suradnicaX][suradnicaY].value;
        }
    } while (*p_valueOfField != 0);

    *p_valueOfField = (rand() % 2 + 1) * 2;
}

bool pohyb(char smer,TERMINAL_UI *terminalPrint, enum ROLE role) {
    BOARD *p_board;
    if (role == CLIENT_1) {
        p_board = &terminalPrint->boardClient_1;
    } else if (role == CLIENT_2) {
        p_board = &terminalPrint->boardClient_2;
    }
    int index;
    bool zmena;
    switch (smer) {
        case 'w':
            zmena = false;
            for (int i = 0; i < terminalPrint->boardSize; ++i) {
                index = -1;
                for (int j = 0; j < terminalPrint->boardSize; ++j) {
                    if (p_board->policka[j][i].value != 0) {
                        if (index == -1) {
                            index = j;
                        } else {
                            if (p_board->policka[j][i].value == p_board->policka[index][i].value) {
                                p_board->policka[index][i].value += p_board->policka[j][i].value;
                                p_board->policka[j][i].value = 0;
                                index = -1;
                                zmena = true;
                            } else {
                                index = j;
                            }
                        }
                    }
                }
                index = -1;
                for (int j = 0; j < terminalPrint->boardSize; ++j) {
                    if (p_board->policka[j][i].value == 0 && index == -1) {
                        index = j;
                    }
                    if (p_board->policka[j][i].value != 0 && index != -1) {
                        p_board->policka[index][i].value = p_board->policka[j][i].value;
                        p_board->policka[j][i].value = 0;
                        index++;
                        zmena = true;
                    }
                }
            }
            break;
        case 'a':
            zmena = false;
            for (int i = 0; i < terminalPrint->boardSize; ++i) {
                index = -1;
                for (int j = 0; j < terminalPrint->boardSize; ++j) {
                    if (p_board->policka[i][j].value != 0) {
                        if (index == -1) {
                            index = j;
                        } else {
                            if (p_board->policka[i][j].value == p_board->policka[i][index].value) {
                                p_board->policka[i][index].value += p_board->policka[i][j].value;
                                p_board->policka[i][j].value = 0;
                                index = -1;
                                zmena = true;
                            } else {
                                index = j;
                            }
                        }
                    }
                }
                index = -1;
                for (int j = 0; j < terminalPrint->boardSize; ++j) {
                    if (p_board->policka[i][j].value == 0 && index == -1) {
                        index = j;
                    }
                    if (p_board->policka[i][j].value != 0 && index != -1) {
                        p_board->policka[i][index].value = p_board->policka[i][j].value;
                        p_board->policka[i][j].value = 0;
                        index++;
                        zmena = true;
                    }
                }
            }
            break;
        case 's':
            zmena = false;
            for (int i = terminalPrint->boardSize - 1; i >= 0; --i) {
                index = -1;
                for (int j = terminalPrint->boardSize - 1; j >= 0; --j) {
                    if (p_board->policka[j][i].value != 0) {
                        if (index == -1) {
                            index = j;
                        } else {
                            if (p_board->policka[j][i].value == p_board->policka[index][i].value) {
                                p_board->policka[index][i].value += p_board->policka[j][i].value;
                                p_board->policka[j][i].value = 0;
                                index = -1;
                                zmena = true;
                            } else {
                                index = j;
                            }
                        }
                    }
                }
                index = -1;
                for (int j = terminalPrint->boardSize- 1; j >= 0; --j) {
                    if (p_board->policka[j][i].value == 0 && index == -1) {
                        index = j;
                    }
                    if (p_board->policka[j][i].value != 0 && index != -1) {
                        p_board->policka[index][i].value = p_board->policka[j][i].value;
                        p_board->policka[j][i].value = 0;
                        index--;
                        zmena = true;
                    }
                }
            }
            break;
        case 'd':
            zmena = false;
            for (int i = terminalPrint->boardSize - 1; i >= 0; --i) {
                index = -1;
                for (int j = terminalPrint->boardSize - 1; j >= 0; --j) {
                    if (p_board->policka[i][j].value != 0) {
                        if (index == -1) {
                            index = j;
                        } else {
                            if (p_board->policka[i][j].value == p_board->policka[i][index].value) {
                                p_board->policka[i][index].value += p_board->policka[i][j].value;
                                p_board->policka[i][j].value = 0;
                                index = -1;
                                zmena = true;
                            } else {
                                index = j;
                            }
                        }
                    }
                }
                index = -1;
                for (int j = terminalPrint->boardSize - 1; j >= 0; --j) {
                    if (p_board->policka[i][j].value == 0 && index == -1) {
                        index = j;
                    }
                    if (p_board->policka[i][j].value != 0 && index != -1) {
                        p_board->policka[i][index].value = p_board->policka[i][j].value;
                        p_board->policka[i][j].value = 0;
                        index--;
                        zmena = true;
                    }
                }
            }
            break;
        default:
            zmena = false;
            printf("%d", (int )smer);
            break;
    }
    return zmena;
}


/*
 * Tu  bude riesenie vlakien na ktorom budeme robit pravdepodobne obaja
 * premysliet:
 *           -este nevieme :(
 * Ten co je prvy bude aj hrac aj server
 * ostatni budu len hraci
 *
 *  spravanie ked spustam ako server:
 *      server hrac nastavi
 *          svoje meno
 *          pocet maximalnych pohybov
 *          cas jednej hry
 *      client hrac nastavi
 *          svoje meno
 *testtest
 *
 * thready pri server aj klient
 *      prvy: vlakno pre bezanie servru
 *      prvy: dat tu vlakno na timer? premysliet
 *      prvy: vlakno pre bezanie klienta
 *      druhy: jedno vlakno klienta
 *
 * zobrazovanie : kazdy klient si zobrazi svoju tabulku to znamena ze sa budu posielat
 * data o jednotlivych polickach,case,tahoch,score kazdemu clientovi
 * klient posle data po kazdom tahu ( vzdy po tahu klienta posle info servru klient--> server)
 * info o tabulkach sa bude aktualizovat po kazdej sekunde( server--> klienti)
 * */



/*
 * Tu  bude riesenie socketov na ktorom budeme robit pravdepodobne obaja
 * premysliet:
 *           -este nevieme :(
 *
 * */


/*
 * Tu  budu ostatne riesenia na ktorych budeme robit pravdepodobne obaja
 * premysliet:
 *           -este nevieme :(
 *
 * */

int main() {
    /* GAME game;
     BOARD celepole;
     TERMINAL_UI game_TerminalPrint;
     TIMER game_Timer;
     PLAYER playerServer;
     PLAYER playerClient_1;


     //celepole.boardSize = 4;
     game_TerminalPrint.boardSize = 4;
     createBoard(&game_TerminalPrint);
     initGameSettings(&game,&game_Timer,SIZE_4,MOVES_30,MINUTE);
     initTimer(&game_Timer);
     initGame(&game,&game_Timer,&game_TerminalPrint);
     */printf("generujem");
    //generuj(&game_TerminalPrint,CLIENT_1);
    //pthread_t timerThreadId;

    // Initialize the mutex
    /* if (pthread_mutex_init(&game_Timer.timer_Mutex, NULL) != 0) {
         fprintf(stderr, "Error initializing mutex.\n");
         return EXIT_FAILURE;
     }

     // Create a thread for the timer
     if (pthread_create(&timerThreadId, NULL, timerThread, &game_Timer.timer_Mutex) != 0) {
         fprintf(stderr, "Error creating timer thread.\n");
         return EXIT_FAILURE;
     }

     celepole.policka[0][0].value = 0;
     celepole.policka[0][1].value = 16;
     celepole.policka[0][2].value = 128;
     celepole.policka[0][3].value = 1024;
     celepole.policka[1][1].value = 16;
     celepole.policka[2][2].value = 128;
     celepole.policka[3][3].value = 1024;*/
    // printBoard(&game_TerminalPrint);


}

/*
 * Tu sa bude inicializovat hra a potrebne komponenty (velkost,cas,role a mena hracov)
 * Zaroven sa tu bude vykonavat prvotne nastavenie hry host hracom --> este neviem uplne ako to bude, uvidime podla threadov
 *
 * */




void initGameSettings(GAME *game, TIMER *game_Timer, enum SIZE_MODE gameSize, enum PLAYER_MOVE_MODE moveMode,
                      enum TIMER_MODE timerMode) {
    game->game_Size = gameSize;
    game->game_MaxPlayerMoves = moveMode;
    game_Timer->gameTimer_Mode = timerMode;
}

void initGame(GAME *game, TIMER *game_Timer, TERMINAL_UI *game_TerminalPrint) {
    game->game_ConnectedPlayers = 0;
    game->game_MaxPlayers = PLAYERS_MAX;
    game->game_Timer = game_Timer;
    game->game_TerminalPrint = game_TerminalPrint;
    //calloc pre server player
    //calloc pre client player
    //game.players = calloc(); //tu spravit calloc pre playerov v hre podla max players(max players moze byt spravene aj cez enumy
    pthread_mutex_init(&game->game_Mutex,NULL);
}

void initTimer(TIMER * game_Timer) {
    pthread_mutex_init(&game_Timer->timer_Mutex,NULL);
    game_Timer->gameTimer_ActualTime_Seconds = game_Timer->gameTimer_Mode * CONVERT_TO_SECOND;
}
void initPlayers() {
    //initPlayerServer();
    //initPlayerClient_1();
}

void initPlayerServer(PLAYER * server) {
    //init server player
}

void initPlayerClient_1(PLAYER * client_1) {
    //init client player
}


/*
 * Tu  bude casovac v threade , bude mat za ulohu odratavat cas
 *
 *
 * */
void * timerThread(void * arg) {
    TIMER * timer = arg;
    clock_t start_time = clock();
    clock_t end_time = start_time + CLOCKS_PER_SEC;
    //(timer->gameTimer_Mode * 60)
    for (int i = timer->gameTimer_ActualTime_Seconds; i >= 0 ; i--) {
        pthread_mutex_lock(&timer->timer_Mutex);
        while (clock() >= end_time) {
            // Wait for one second
            start_time = clock();
            end_time = start_time + CLOCKS_PER_SEC;
        }
        printf("time: %d",timer->gameTimer_ActualTime_Seconds);
        pthread_mutex_unlock(&timer->timer_Mutex);
    }


    return NULL;
}

void waitOneSecond() {
    clock_t start_time = clock();
    clock_t end_time = start_time + CLOCKS_PER_SEC;

    while (clock() < end_time) {
        // Wait for one second
    }
}
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


void createBoardClient_1(TERMINAL_UI *terminalPrint) {
    terminalPrint->boardClient_1.policka = ((FIELD **) calloc(terminalPrint->boardSize, sizeof(FIELD *)));
    for (int i = 0; i < terminalPrint->boardSize; ++i) {
        terminalPrint->boardClient_1.policka[i] = (FIELD *) calloc((terminalPrint->boardSize), sizeof(FIELD));
    }
    for (int riadok = 0; riadok < terminalPrint->boardSize; ++riadok) {
        for (int stlpec = 0; stlpec < terminalPrint->boardSize; ++stlpec) {
            terminalPrint->boardClient_1.policka[riadok][stlpec].value = 16;
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
            terminalPrint->boardClient_2.policka[riadok][stlpec].value = 8;
        }
    }
}

void printBlankSectionWithMessage(const int size)
{
    for (int column = 0; column < size; ++column) {
        printf("%s",empty );

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
    for (int column = 0; column < terminalPrint.boardSize; ++column) {
        printSymbolFix(terminalPrint, row, column, boardClient_1);
        if(column != terminalPrint.boardSize - 1){
            printf("%c",vertical);
        }
    }
}

void printOnlyVerticalSection(const int size){
    printEmptyVertical(size);
    printBlankSectionWithMessage(size);
    printEmptyVertical(size);
    printf("\n");
}

void printMiddleSection(const int size,const int row){
    if(row != size - 1){
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
    if(row == terminalPrint.boardSize - 1)
    {
        printf("\n");
    }
    printSymbol(terminalPrint,row, true);//boardClient_1 == true znaci ci to je board clienta 1, v tomto pripade true
    printBlankSectionWithMessage(terminalPrint.boardSize);
    printSymbol(terminalPrint,row, false);//boardClient_1 == false znaci ci to je board clienta 1, v tomto pripade false a teda vieme ze to je board clienta 2 lebo mame len dva boardy
    if(row != terminalPrint.boardSize - 1)
    {
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

void printBoard(TERMINAL_UI * terminalPrint)
{
    printColumnNumber(terminalPrint->boardSize);
    printOnlyVerticalSection(terminalPrint->boardSize);
    for (int i = 0; i < terminalPrint->boardSize ; ++i) {
        printSymbolSection(*terminalPrint, i);
        printMiddleSection(terminalPrint->boardSize, i);
        printBlankSectionWithMessage(terminalPrint->boardSize);
        printMiddleSection(terminalPrint->boardSize, i);
        if(i != terminalPrint->boardSize - 2){
            printf("\n");
        }
    }
    printOnlyVerticalSection(terminalPrint->boardSize);
}

