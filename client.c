#include "terminal_display.h"
#include "client.h"

bool receiveMsg_Client (int sockfd, char* buffer) {
    bzero(buffer,256);
    int n;
    n = read(sockfd, buffer, 255);
    if (n < 0)
    {
        perror("Error reading from socket");
        return false;
    }
    return true;
}

bool sendMsg_Client (int sockfd, char* buffer) {
    int n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
    {
        perror("Error writing to socket");
        return false;
    }
    return true;
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

void clearMessage(KLIENT *klient) {
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

void pickRole(TERMINAL_UI * gameTerminal, KLIENT * klient, char * role) {
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

void concatenateArray(GAME_TERMINAL * gameTerminal, char resultString[BUFFER_LENGTH], enum ROLE role) {
    int offset = 0;
    int value = 0;

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


void* klientF (void *arg) {
    KLIENT *klient = arg;
    //initPlayer(klient);
    pthread_mutex_lock(&klient->mutex);
    printf("printujemC2 %u\n", klient->role);
    generuj(klient->boards->game_TerminalPrint,klient->role);
    printBoard(klient->boards);
    pthread_mutex_unlock(&klient->mutex);
    while (strcmp(klient->msg, "koniec\0") != 0) {
        //pthread_mutex_lock(&klient->mutex);
        bzero(klient->msg, 256);
        receiveMsg_Client(klient->sockfd, klient->msg);// timer od servra
        klient->boards->game_timerPrint = atoi(klient->msg);

        printf("hodnoty klient2 su: %s\n",klient->msg);
        //parseString("512;512;512;512;1024;16;16;16;16;32;32;32;32;1024;1024;1024",klient->boards,klient->role);
        //sendMsg_Client(klient->sockfd,);
        //printBoard(klient->boards);
        //printf("%d cas\n", klient->boards->game_timerPrint);
        if (klient->boards->game_timerPrint != 0) {
            if (klient->zadalZnak) {
                klient->boards->playersInfo[klient->role].playerMove++;
                generuj(klient->boards->game_TerminalPrint,klient->role);
                concatenateArray(klient->boards,klient->msg,klient->role);
                sendMsg_Client(klient->sockfd,klient->msg);// posielam hodnoty pola
                receiveMsg_Client(klient->sockfd,klient->msg);// prijimam hodnoty druheho hraca
                snprintf(klient->msg, sizeof(klient->msg), "%d", klient->boards->playersInfo[klient->role].playerMove);
                sendMsg_Client(klient->sockfd,klient->msg);// posielam pocet mojich pohybov
                receiveMsg_Client(klient->sockfd,klient->msg);// prijimam pocet pohybov druheho
                if (klient->role == HOST) {
                    klient->boards->playersInfo[CLIENT].playerMove = atoi(klient->msg);//ak je klient host nastavi pohyb pre klienta
                }else {
                    klient->boards->playersInfo[HOST].playerMove = atoi(klient->msg);//ak je klient client nastavi pohyb pre hosta
                }
                printf("Prijate hodnoty klient2 su: %s\n",klient->msg);
                parseString(klient->msg,klient->boards,klient->role);//hodnoty precitam a rovno zapisem do board
                klient->zadalZnak = false;

                klient->boards->playersInfo[HOST].score = countScore(klient->boards,HOST);
                klient->boards->playersInfo[CLIENT].score = countScore(klient->boards,CLIENT);
            }
            //pthread_mutex_unlock(&klient->mutex);
            printBoard(klient->boards);
        }
    }
    sendMsg_Client(klient->sockfd, "k\0");

    return NULL;
}

int client()
{
    srand(time(NULL));
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;
    server = gethostbyname("127.0.0.1");
    //server = gethostbyname("frios2.fri.uniza.sk");
    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host\n");
        return 2;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(
            (char*)server->h_addr,
            (char*)&serv_addr.sin_addr.s_addr,
            server->h_length
    );
    serv_addr.sin_port = htons(18489);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 3;
    }

    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error connecting to socket");
        return 4;
    }
    GAME_TERMINAL gameClient;
    TERMINAL_UI gameTerminal;

    KLIENT klientData;
    klientData.zadalZnak = false;
    klientData.sockfd = sockfd;
    pthread_mutex_init(&klientData.mutex,NULL);


    receiveMsg_Client(klientData.sockfd,klientData.msg);
    if(strcmp(klientData.msg,"host\0") == 0){
        pickRole(&gameTerminal, &klientData, "host\0");
        int timer,moves;
        printTimerModes();
        printf("Vyber si z tychto moznosti casomiery(cislo od 1 po 4)\n");
        scanf("%d", &timer);
        printPlayerMoveModes();
        printf("Vyber si z tychto moznosti poctu pohybov(cislo od 1 po 3)\n");
        scanf("%d", &moves);
        sprintf(klientData.msg, "%d;%d", moves, timer);
        printf("Concatenated String: %s\n", klientData.msg);
        sendMsg_Client(klientData.sockfd,klientData.msg);
        clearMessage(&klientData);
        initGameSettings(&gameClient, &gameTerminal, 4, moves - 1, timer - 1,klientData.role);
        createBoard(&gameTerminal);
        klientData.boards = &gameClient;
        //kazdy klient si bude generovat sam cisla na svoju tabulku a len posle data
        //tym padom by bolo mozno vhodne posielat data o tabulke kazdu sekundu
        //pride sprava start od servra (mvyber mena zatial nechame tak)
        //klient ktory bude prvy vyberie dlzku,pocet pohybov
        // dalsi klient caka
        //server obdrzi dlzku hry,pocet pohybov a posle to druhemu klientovi
        //server posle init pre oboch klientov nech zobrazia boards a spusti casomieru
    } else {
        pickRole(&gameTerminal,&klientData,"klient\0");
        printf("Hrac host vybera nastavenia hry,pockaj..\n");
    }
    receiveMsg_Client(klientData.sockfd,klientData.msg);
    printf("%s\n", klientData.msg);
    //

    //ak je to klient tak musi obdrzat nastavenia hry teda gameMoves,gameTimer
    if (klientData.role == CLIENT) {
        sendMsg_Client(klientData.sockfd,"mam\0");
        receiveMsg_Client(klientData.sockfd,klientData.msg);
        char movesCH = klientData.msg[0];
        int moves = movesCH  - '0';
        char timerCH = klientData.msg[2];
        int timer = timerCH - '0';
        initGameSettings(&gameClient, &gameTerminal, 4, moves - 1, timer - 1,klientData.role);
        createBoard(&gameTerminal);
        klientData.boards = &gameClient;
    }

    pthread_t klient;
    pthread_create(&klient, NULL, klientF, &klientData);

    while (strcmp(klientData.msg, "k\0") != 0) {
        // pthread_mutex_lock(&klientData.mutex);
        bzero(klientData.msg,256);
        printf("zadaj:\n");
        scanf("%s", klientData.msg);
        if (klientData.zadalZnak) {
            printf("Znak si uz zadal cakaj na aktualizaciu policok\n");
        }else {
            pohyb(klientData.msg[0],&gameTerminal,klientData.role);
            klientData.zadalZnak = true;
        }
        // pthread_mutex_unlock(&klientData.mutex);
        sendMsg_Client(klientData.sockfd, klientData.msg);

    }
    printf("KONIEC:\n");
    pthread_join(klient,NULL);

    pthread_mutex_destroy(&klientData.mutex);
    close(sockfd);
    freeBoard(&gameTerminal);
    return 0;
}
