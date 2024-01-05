#include <time.h>
#include <pthread.h>
#include "terminal_display.h"
#include "server/pos_sockets/passive_socket.h"
#include "server/pos_sockets/char_buffer.h"
#include "server/pos_sockets/active_socket.h"
#include "server/buffer.h"
typedef struct thread_data {
    pthread_mutex_t mutex;
    pthread_cond_t zapis;
    pthread_cond_t citaj;
    bool citanie;
    GAME game;

    short port;
    ACTIVE_SOCKET* my_socket;
} THREAD_DATA;
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

void* serverThread(void* arg) {
    GAME* game = (GAME*)arg;

    while (1) {
        pthread_mutex_lock(&game->game_Mutex);
        pthread_cond_wait(&game->server,&game->game_Mutex);
        // Perform server-related actions

        printf("Server: Performing actions\n");

        // Signal the client thread that the server has updated
        pthread_cond_signal(&game->timer);

        // Wait for the client thread to complete its actions
        // pthread_cond_wait(&game->server_Condition, &game->game_Mutex);

        pthread_mutex_unlock(&game->game_Mutex);

        // Sleep or perform other server actions
    }

    return NULL;
}

void* clientThread(void* arg) {
    GAME* game = (GAME*)arg;

    while (1) {
        pthread_mutex_lock(&game->game_Mutex);
        pthread_cond_wait(&game->client,&game->game_Mutex);
        // Perform server-related actions
        printBoard(game);

        //printf("Server: Performing actions\n");

        // Signal the client thread that the server has updated
        pthread_cond_signal(&game->timer);

        // Wait for the client thread to complete its actions
        // pthread_cond_wait(&game->server_Condition, &game->game_Mutex);

        pthread_mutex_unlock(&game->game_Mutex);

        // Sleep or perform other server actions
    }

    return NULL;
}

void * timerThread(void * arg) {
    GAME * game= (GAME *) arg;
    TIMER * timer = (TIMER*)game->game_Timer;
    bool timerLoop = true;
    timer->gameTimer_ActualTime_Seconds += 1;
    while(timerLoop) {
        sleep(1);
        timer->gameTimer_ActualTime_Seconds -= 1;

        pthread_cond_signal(&game->client);
        pthread_cond_wait(&game->timer,&game->game_Mutex);
        if (timer->gameTimer_ActualTime_Seconds <= 0) {
            timerLoop = false;
        }
        pthread_mutex_unlock(&game->game_Mutex);
    }
    return NULL;
}

void* createConnection(void* thread_data) {
    THREAD_DATA *data = (THREAD_DATA *)thread_data;
    PASSIVE_SOCKET p_socket;
    passive_socket_init(&p_socket);
    passive_socket_start_listening(&p_socket, data->port);
    passive_socket_wait_for_client(&p_socket, data->my_socket);

    printf("connected1\n");
    /*passive_socket_wait_for_client(&p_socket, data->my_socket);
    printf("connected2\n");*/
    passive_socket_stop_listening(&p_socket);
    passive_socket_destroy(&p_socket);

    active_socket_start_reading(data->my_socket);
    return NULL;
}

void* receiveF(void* thread_data) {
    THREAD_DATA *data = (THREAD_DATA *) thread_data;
    printf("receiving\n");
    while (data->citanie) {
        printf("try get data\n");
        CHAR_BUFFER r_buf;
        char_buffer_init(&r_buf);
        pthread_mutex_lock(&data->mutex);
        if(active_socket_try_get_read_data(data->my_socket, &r_buf)) {
            if(r_buf.size > 0) {
                printf("%s\n", r_buf.data);
                data->citanie = false;
                active_socket_stop_reading(data->my_socket);
                pthread_cond_signal(&data->zapis);
            }
        }
        pthread_mutex_unlock(&data->mutex);
        char_buffer_destroy(&r_buf);
    }
    return NULL;
}

void* produceF(void* thread_data) {
    THREAD_DATA *data = (THREAD_DATA *)thread_data;
    CHAR_BUFFER buffer;
    char_buffer_init(&buffer);

    char *message = "Hello Client!";
    snprintf(buffer.data, sizeof(buffer.data), "%s", message);

    pthread_mutex_lock(&data->mutex);
    pthread_cond_wait(&data->zapis,&data->mutex);
    active_socket_write_data(data->my_socket, &buffer);
    pthread_mutex_unlock(&data->mutex);
    printf("send\n");

     char_buffer_destroy(&buffer);

    return NULL;
}
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
    pthread_t th_produce;
    pthread_t th_receive;
    pthread_t connect;
    struct active_socket my_socket;
    active_socket_init(&my_socket);
    struct thread_data data;
    data.port = 10001;
    data.my_socket = &my_socket;
    pthread_mutex_init(&data.mutex, NULL);
    pthread_cond_init(&data.zapis, NULL);
    pthread_cond_init(&data.citaj, NULL);

    createConnection(&data);
    data.citanie = true;
    pthread_create(&th_receive, NULL, receiveF, &data);
    pthread_create(&th_produce, NULL, produceF, &data);
    pthread_join(th_receive, NULL);
    pthread_join(th_produce, NULL);

    active_socket_destroy(&my_socket);

/*    GAME game;
    BOARD celepole;
    TERMINAL_UI game_TerminalPrint;
    TIMER game_Timer;
    PLAYER  player_1;
    PLAYER  player_2;
    game.players[CLIENT_1] = player_1;
    game.players[CLIENT_2] = player_2;
    strcpy(player1NameGlob, "majco");
    strcpy(player2NameGlob, "vajco");
    //celepole.boardSize = 4;
    game_TerminalPrint.boardSize = 4;
    createBoard(&game_TerminalPrint);
    initGameSettings(&game,&game_Timer,SIZE_4,MOVES_30,MINUTE);
    initTimer(&game_Timer);
    initGame(&game,&game_Timer,&game_TerminalPrint);
    initPlayers(&game,player1NameGlob,player2NameGlob);
    printf("generujem");
    pthread_mutex_init(&game.game_Mutex, NULL);
    pthread_cond_init(&game.timer, NULL);
    pthread_cond_init(&game.server, NULL);
    pthread_cond_init(&game.client, NULL);
    pthread_t timerThreadID;
    pthread_t serverThreadID;
    pthread_t clientThreadID;
    //pthread_create(&serverThreadID, NULL, serverThread, &game);
    pthread_create(&timerThreadID, NULL, timerThread, &game);
    pthread_create(&clientThreadID, NULL, clientThread, &game);
    pthread_join(timerThreadID, NULL);
    pthread_join(clientThreadID, NULL);
    //pthread_join(serverThreadID, NULL);
    pthread_mutex_destroy(&game.game_Mutex);
    pthread_cond_destroy(&game.timer);
    pthread_cond_destroy(&game.server);
    pthread_cond_destroy(&game.server);
     generuj(&game_TerminalPrint,CLIENT_1);
     generuj(&game_TerminalPrint,CLIENT_2);
     printf("idem hore");
     printBoard(&game_TerminalPrint);
     pohyb('w',&game_TerminalPrint,CLIENT_1);
     generuj(&game_TerminalPrint,CLIENT_1);
     printBoard(&game_TerminalPrint);
     pohyb('w',&game_TerminalPrint,CLIENT_1);

    // Initialize the mutex
    /* if (pthread_mutex_init(&game_Timer.timer_Mutex, NULL) != 0) {
         fprintf(stderr, "Error initializing mutex.\n");
         return EXIT_FAILURE;
     }

     // Create a thread for the timer
     if (pthread_create(&timerThreadID, NULL, timerThread, &game_Timer.timer_Mutex) != 0) {
         fprintf(stderr, "Error creating timer thread.\n");
         return EXIT_FAILURE;
     }*/

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

void initPlayers(GAME * game, char player1Name[MAX_NAME_LENGTH], char player2Name[MAX_NAME_LENGTH]) {
    strcpy(game->players[CLIENT_1].name, "marianalb");
    game->players[CLIENT_1].playerMove = 10;
    game->players[CLIENT_1].score = 1000000;
    game->players[CLIENT_1].playerBoard = game->game_TerminalPrint->boardClient_1;
    strcpy(game->players[CLIENT_2].name, "vajco");
    game->players[CLIENT_2].playerMove = 20;
    game->players[CLIENT_2].score = 2000000;
    game->players[CLIENT_2].playerBoard = game->game_TerminalPrint->boardClient_2;
}

/*
 * Tu  bude casovac v threade , bude mat za ulohu odratavat cas
 *
 *
 * */

void waitOneSecond() {
    clock_t start_time = clock();
    clock_t end_time = start_time + CLOCKS_PER_SEC;

    while (clock() < end_time) {
        // Wait for one second
    }
}

