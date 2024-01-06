#include <time.h>
#include <pthread.h>
#include "terminal_display.h"
#include "server.h"
#include "client.h"
//#include "client.h"
/*
 * Tu  bude logika od Andreja,teda pohyb hore,dolu,vpravo,vlavo
 * premysliet:
 *           -ze hrac je aj server aj hrac teda ze prvy hrac bude server aj hrac
 *
 * */


void generuj(TERMINAL_UI *terminalPrint, enum ROLE role) {
    int suradnicaX;
    int suradnicaY;
    int *p_valueOfField;

    do {
        suradnicaX = rand() % terminalPrint->boardSize;
        suradnicaY = rand() % terminalPrint->boardSize;

        if (role == HOST) {
            p_valueOfField = &terminalPrint->boardClient_1.policka[suradnicaX][suradnicaY].value;
        } else if (role == CLIENT) {
            p_valueOfField = &terminalPrint->boardClient_2.policka[suradnicaX][suradnicaY].value;
        }
    } while (*p_valueOfField != 0);

    *p_valueOfField = (rand() % 2 + 1) * 2;
}

bool pohyb(char smer,TERMINAL_UI *terminalPrint, enum ROLE role) {
    BOARD *p_board;
    if (role == HOST) {
        p_board = &terminalPrint->boardClient_1;
    } else if (role == CLIENT) {
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

int main(int argc, char * argv[]) {
   if (strcmp(argv[1],"server") == 0){
        server();
   }else {
        client();
   }
    /* generuj(&game_TerminalPrint,HOST);
     generuj(&game_TerminalPrint,CLIENT);
     printf("idem hore");
     printBoard(&game_TerminalPrint);
     pohyb('w',&game_TerminalPrint,HOST);
     generuj(&game_TerminalPrint,HOST);
     printBoard(&game_TerminalPrint);
     pohyb('w',&game_TerminalPrint,HOST);
    */
    return 0;
}

/*
 * Tu sa bude inicializovat hra a potrebne komponenty (velkost,cas,role a mena hracov)
 * Zaroven sa tu bude vykonavat prvotne nastavenie hry host hracom --> este neviem uplne ako to bude, uvidime podla threadov
 *
 * */

/*
 * void initPlayers(GAME * game, char player1Name[MAX_NAME_LENGTH], char player2Name[MAX_NAME_LENGTH]) {
    strcpy(game->players[HOST].name, player1Name);
    game->players[HOST].playerMove = 10;
    game->players[HOST].score = 1000000;
    game->players[HOST].playerBoard = game->game_TerminalPrint->boardClient_1;
    strcpy(game->players[CLIENT].name, player2Name);
    game->players[CLIENT].playerMove = 20;
    game->players[CLIENT].score = 2000000;
    game->players[CLIENT].playerBoard = game->game_TerminalPrint->boardClient_2;
}

*/


