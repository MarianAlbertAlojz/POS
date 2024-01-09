#include "terminal_display.h"
#include "client.h"

int sendMsg_Client (int writeSocket, char* buffer) {
    int n;
    n = write(writeSocket, buffer, strlen(buffer));
    if (n < 0)
    {
        perror("Error writing to socket");
        return -5;
    }

    return 0;
}
int receiveMsg_Client (int readSocket, char* buffer) {
    int n;
    bzero(buffer,BUFFER_LENGTH);
    n = read(readSocket, buffer, BUFFER_LENGTH - 1);
    if (n < 0)
    {
        perror("Error reading from socket");
        return -6;
    }

    return 0;
}
bool pohyb(char smer,TERMINAL_UI *terminalPrint, enum ROLE role) {
    BOARD *p_board;
    /*if (role == HOST) {
        p_board = &terminalPrint->boardClients[HOST];
    } else if (role == CLIENT) {
        p_board = &terminalPrint->boardClients[CLIENT];
    }*/
    p_board = &terminalPrint->boardClients[role];
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

const char* timerModeToString(enum TIMER_MODE mode) {
    switch (mode) {
        case MINUTE: return "1 Minute";
        case TWO_MINUTES: return "2 Minutes";
        case THREE_MINUTES: return "3 Minutes";
        case FOUR_MINUTES: return "4 Minutes";
        default: return "Unknown Timer Mode";
    }
}

const char* playerMoveModeToString(enum PLAYER_MOVE_MODE mode) {
    switch (mode) {
        case MOVES_30: return "30 Moves";
        case MOVES_35: return "35 Moves";
        case MOVES_40: return "40 Moves";
        default: return "Unknown Player Move Mode";
    }
}

void printTimerModes() {
    printf("Timer Modes:\n");
    for (enum TIMER_MODE mode = MINUTE; mode <= FOUR_MINUTES; ++mode) {
        printf("%d. %s\n", mode, timerModeToString(mode));
    }
}

void printPlayerMoveModes() {
    printf("Player Move Modes:\n");
    int poradie = 1;
    for (int i = 30; i <= 40; i += 5) {
        printf("%d. %s\n", poradie, playerMoveModeToString(i));
        poradie++;
    }
}

void clearMessage(CLIENT_STRUCT_CLIENT *klient) {
    // Use memset to set all elements of msg to 0
    memset(klient->msg, 0, sizeof(klient->msg));
}

void initGameSettings(GAME_TERMINAL *game,TERMINAL_UI *game_TerminalPrint ,int gameSize,int gameMove,int gameTimer,enum ROLE role) {
    enum PLAYER_MOVE_MODE movesArray[] = {MOVES_30, MOVES_35, MOVES_40};
    enum TIMER_MODE timerArray[] = {MINUTE, TWO_MINUTES, THREE_MINUTES, FOUR_MINUTES};
    game->game_TerminalPrint = game_TerminalPrint;
    game->game_sizeMode = 4;
    game_TerminalPrint->boardSize = game->game_sizeMode;
    game->game_moveMode = movesArray[gameMove];
    game->game_timerMode = timerArray[gameTimer];
    strncpy(game->playersInfo[0].name, "Andrej", sizeof(game->playersInfo[0].name) - 1);
    strncpy(game->playersInfo[1].name, "Marian", sizeof(game->playersInfo[1].name) - 1);
    for (int i = 0; i < PLAYERS_MAX; i++) {
        game->playersInfo[i].score = 0;
        game->playersInfo[i].playerMove = 0;
        game->playersInfo[i].name[sizeof(game->playersInfo[i].name) - 1] = '\0';
    }
}

void pickRole(TERMINAL_UI * gameTerminal, CLIENT_STRUCT_CLIENT * klient, char * role) {
    if (strcmp(role,"host\0") == 0) {
        klient->role = HOST;
        printf("Tvoja rola je HOST, nastavujes hru\n");
    }else {
        klient->role = CLIENT;
        printf("Tvoja rola je CLIENT, cakas na hru\n");
    }
}

void generuj(TERMINAL_UI *terminalPrint, enum ROLE role) {
    int suradnicaX;
    int suradnicaY;
    int *p_valueOfField;

    do {
        suradnicaX = rand() % terminalPrint->boardSize;
        suradnicaY = rand() % terminalPrint->boardSize;
        p_valueOfField = &terminalPrint->boardClients[role].policka[suradnicaX][suradnicaY].value;
        /*if (role == HOST) {

        } else if (role == CLIENT) {
            p_valueOfField = &terminalPrint->boardClients[CLIENT].policka[suradnicaX][suradnicaY].value;
        }*/
    } while (*p_valueOfField != 0);

    *p_valueOfField = (rand() % 2 + 1) * 2;
}

void concatenateArray(GAME_TERMINAL * gameTerminal, char  resultString[BUFFER_LENGTH], enum ROLE role) {
    int offset = 0;
    int value = 0;

    value = gameTerminal->game_timerPrint;
    offset += sprintf(resultString + offset, "%d:",value);
    //printf("Toto je debug pri spajani: %s\n", resultString);
    value = gameTerminal->playersInfo[role].playerMove;
    offset += sprintf(resultString + offset, "%d:",value);
    //printf("Toto je debug pri spajani: %s\n", resultString);
    for (int riadok = 0; riadok < gameTerminal->game_TerminalPrint->boardSize; ++riadok) {
        for (int stlpec = 0; stlpec < gameTerminal->game_TerminalPrint->boardSize; ++stlpec) {
            value = gameTerminal->game_TerminalPrint->boardClients[role].policka[riadok][stlpec].value;
            if (riadok == 3 && stlpec == 3) {
                offset += sprintf(resultString + offset, "%d",value);
            }else {
                offset += sprintf(resultString + offset, "%d;",value);
            }

        }
    }
}

void parseString(const char* concatenatedString, GAME_TERMINAL * gameTerminal, enum ROLE role) {
    int offset = 0;
    int value = 0;
    enum ROLE druhyHrac;
    if (role == HOST) {
        druhyHrac = CLIENT;
    }else {
        druhyHrac = HOST;
    }
    for (int i = 0; i < 2; ++i) {
        sscanf(concatenatedString + offset, "%d:", &value);
        if(i == 0) {
            gameTerminal->game_timerPrint = value;
        }else {
            gameTerminal->playersInfo[druhyHrac].playerMove = value;
        }
    }
    for (int riadok = 0; riadok < gameTerminal->game_TerminalPrint->boardSize; ++riadok) {
        for (int stlpec = 0; stlpec < gameTerminal->game_TerminalPrint->boardSize; ++stlpec) {
            sscanf(concatenatedString + offset, "%d;", &value);
            gameTerminal->game_TerminalPrint->boardClients[druhyHrac].policka[riadok][stlpec].value = value;
            // najdi ;
            while (concatenatedString[offset] != ';' && concatenatedString[offset] != '\0') {
                offset++;
            }
            // posun sa za ;
            if (concatenatedString[offset] == ';') {
                offset++;
            }
        }
    }
    gameTerminal->playersInfo[HOST].score = countScore(gameTerminal,HOST);
    gameTerminal->playersInfo[CLIENT].score = countScore(gameTerminal,CLIENT);
    printf("Toto je debug pri rozdelovani: %s\n", concatenatedString);
}

int countScore(GAME_TERMINAL * gameTerminal, enum ROLE role) {
    int score = 0;
    for (int riadok = 0; riadok < gameTerminal->game_TerminalPrint->boardSize; ++riadok) {
        for (int stlpec = 0; stlpec < gameTerminal->game_TerminalPrint->boardSize; ++stlpec) {
            score += gameTerminal->game_TerminalPrint->boardClients[role].policka[riadok][stlpec].value;
        }
    }
    return score;
}

int config (int * readSocket, int * writeSocket) {
    struct sockaddr_in serv_addr;
    struct hostent* server;
    server = gethostbyname("127.0.0.1");
    //server = gethostbyname("frios2.fri.uniza.sk");
    if (server == NULL) {
        fprintf(stderr, "Error, no such host\n");
        return -2;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(
            (char*)server->h_addr,
            (char*)&serv_addr.sin_addr.s_addr,
            server->h_length
    );
    serv_addr.sin_port = htons(PORT);

    *readSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (*readSocket < 0) {
        perror("Error creating socket");
        return -3;
    }
    if(connect(*readSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting to socket");
        return -4;
    }
    *writeSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (*writeSocket < 0) {
        perror("Error creating socket");
        return -3;
    }

    if(connect(*writeSocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        close(*readSocket);
        perror("Error connecting to socket");
        return -4;
    }

    return 0;
}
void * move (void * arg) {
    SEND * send = arg;
    char buffer[BUFFER_LENGTH];
    bzero(buffer, BUFFER_LENGTH);
    generuj(send->client->boards->game_TerminalPrint,send->client->role);
    while (true) {
        pthread_mutex_lock(&send->client->mutex);
        if (send->client->koniec) {
            pthread_mutex_unlock(&send->client->mutex);
            break;
        }
        pthread_mutex_unlock(&send->client->mutex);
        printf("Zadaj znak: \n");
        scanf(" %s", buffer);
        pthread_mutex_lock(&send->client->mutex);
        if (strcmp(buffer, "k") == 0) {
            sendMsg_Client(send->writeSocket, buffer);
            bzero(buffer,BUFFER_LENGTH);
            sprintf(buffer, "%d", send->client->boards->playersInfo[send->client->role].score);
            sendMsg_Client(send->writeSocket,buffer);
            send->client->koniec = true;
            pthread_mutex_unlock(&send->client->mutex);
            break;
        }
        if (pohyb(buffer[0],send->client->boards->game_TerminalPrint,send->client->role)){
            generuj(send->client->boards->game_TerminalPrint,send->client->role);
            send->client->boards->playersInfo[send->client->role].playerMove++;

        }
        concatenateArray(send->client->boards,buffer,send->client->role);
        printf("tu je debugg :  %s\n", buffer);

        pthread_mutex_unlock(&send->client->mutex);
        sendMsg_Client(send->writeSocket,buffer);// posielam hodnoty timer:move:hodnotypola;pole;pole
        usleep(1000);
    }

    return NULL;
}

void * update (void * arg) {
    RECEIVE * receive = arg;
    char buffer[BUFFER_LENGTH];
    bzero(buffer, BUFFER_LENGTH);

    while (true) {
        pthread_mutex_lock(&receive->client->mutex);
        if (receive->client->koniec) {
            pthread_mutex_unlock(&receive->client->mutex);
            break;
        }
        pthread_mutex_unlock(&receive->client->mutex);
        receiveMsg_Client(receive->readSocket, buffer); // timer:pohyb:datapoleň

        if (strcmp(buffer,"REMIZA!") == 0 ||strcmp(buffer,"VYHRAL SI!") == 0 || strcmp(buffer,"PREHRAL SI!") == 0 ) {
            printf("%s\n", buffer);
            break;
        }
        printf("%s\n", buffer);
        pthread_mutex_lock(&receive->client->mutex);
        parseString(buffer,receive->client->boards,receive->client->role);//hodnoty precitam a rovno zapisem do board
        printBoard(receive->client->boards);
        pthread_mutex_unlock(&receive->client->mutex);
        usleep(1000);
    }

    return NULL;
}

int client()
{
    srand(time(NULL));
    int error;
    CLIENT_STRUCT_CLIENT klientData;
    klientData.zadalZnak = false;
    klientData.koniec = false;
    pthread_mutex_init(&klientData.mutex, NULL);

    RECEIVE receiveData;
    receiveData.client = &klientData;

    SEND sendData;
    sendData.client = &klientData;
    error = config(&receiveData.readSocket, &sendData.writeSocket);
    if (error) {
        return abs(error);
    }
    char localBuffer[BUFFER_LENGTH];
    GAME_TERMINAL gameClient;
    TERMINAL_UI gameTerminal;
    gameClient.game_timerPrint = 0;
    gameClient.game_sizeMode = 0;
    gameClient.game_moveMode = 0;
    gameClient.game_timerMode = 0;
    for (int i = 0; i < PLAYERS_MAX; ++i) {
        bzero(gameClient.playersInfo[i].name,MAX_NAME_LENGTH);
        gameClient.playersInfo[i].playerMove = 0;
        gameClient.playersInfo[i].score = 0;
    }
    gameClient.game_TerminalPrint = &gameTerminal;
    receiveMsg_Client(receiveData.readSocket,localBuffer);
    if(strcmp(localBuffer,"host\0") == 0){
        pickRole(&gameTerminal, &klientData, "host\0");
        int timer,moves;
        printTimerModes();
        printf("Vyber si z tychto moznosti casomiery(cislo od 1 po 4)\n");
        scanf("%d", &timer);
        printPlayerMoveModes();
        printf("Vyber si z tychto moznosti poctu pohybov(cislo od 1 po 3)\n");
        scanf("%d", &moves);
        sprintf(localBuffer, "%d;%d", moves, timer);
        printf("Concatenated String: %s\n", localBuffer);
        sendMsg_Client(sendData.writeSocket,localBuffer);
        initGameSettings(&gameClient, &gameTerminal, 4, moves - 1, timer - 1,klientData.role);
        //createBoard(&gameTerminal);
        klientData.boards = &gameClient;
    } else {
        pickRole(&gameTerminal,&klientData,"klient\0");
        printf("Hrac host vybera nastavenia hry,pockaj..\n");
    }
    receiveMsg_Client(receiveData.readSocket,localBuffer);
    printf("%s\n", localBuffer);
    //

    //ak je to klient tak musi obdrzat nastavenia hry teda gameMoves,gameTimer
    if (klientData.role == CLIENT) {
        //sendMsg_Client(sendData.writeSocket,"mam\0"); // tu sa dohaduje client a server
        receiveMsg_Client(receiveData.readSocket,localBuffer);
        char movesCH = localBuffer[0];
        int moves = movesCH  - '0';
        char timerCH = localBuffer[2];
        int timer = timerCH - '0';
        initGameSettings(&gameClient, &gameTerminal, 4, moves - 1, timer - 1,klientData.role);
        //createBoard(&gameTerminal);
        klientData.boards = &gameClient;
    }
    createBoard(&gameTerminal);
    //generuj(klientData.boards->game_TerminalPrint,klientData.role);
    //concatenateArray(klientData.boards,localBuffer,klientData.role);
    if (klientData.role == CLIENT) {
        usleep(10000);

    }
    sendMsg_Client(sendData.writeSocket,localBuffer);
    printf("%s\n", localBuffer);
//koniec nastavenia klientov

    pthread_t receiving;
    pthread_t sending;

    pthread_create(&sending, NULL, move, &sendData);
    pthread_create(&receiving, NULL, update, &receiveData);

    pthread_join(sending, NULL);
    pthread_join(receiving, NULL);

    //vitaz
    bzero(localBuffer,BUFFER_LENGTH);
    sprintf(localBuffer, "%d", klientData.boards->playersInfo[klientData.role].score);
    sendMsg_Client(sendData.writeSocket,localBuffer);
    receiveMsg_Client(receiveData.readSocket,localBuffer);
    //printf("%s\n",localBuffer);
    printf("oka DEBUG KONIEC\n");

    pthread_mutex_destroy(&klientData.mutex);
    close(receiveData.readSocket);
    close(sendData.writeSocket);
    freeBoard(&gameTerminal);
    return 0;
}
