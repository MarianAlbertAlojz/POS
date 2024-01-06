//
// Created by PC1 on 04/01/2024.
//


#include "terminal_display.h"
#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <netinet/in.h>
#include <time.h>
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
    for (int i= 0; i < 3; i++) {
        printf("%d. %s\n", i, playerMoveModeToString(i));
    }
}

void clearMessage(KLIENT *klient) {
    // Use memset to set all elements of msg to 0
    memset(klient->msg, 0, sizeof(klient->msg));
}

void initGameSettings(GAME_TERMINAL *game,TERMINAL_UI *game_TerminalPrint ,int gameSize,int gameMove,int gameTimer) {
    enum PLAYER_MOVE_MODE movesArray[] = {MOVES_30, MOVES_35, MOVES_40};
    enum TIMER_MODE timerArray[] = {MINUTE, TWO_MINUTES, THREE_MINUTES, FOUR_MINUTES};
    game->game_TerminalPrint = game_TerminalPrint;
    game->game_size = 4;
    game_TerminalPrint->boardSize = game->game_size;
    game->game_move = movesArray[gameMove];
    game->game_timer = timerArray[gameTimer];
}

void pickRole(TERMINAL_UI * gameTerminal, KLIENT * klient, char * role) {
    if (strcmp(role,"host\0") == 0) {
        klient->role = HOST;
        klient->playerBoard = gameTerminal->boardClient_1;
        printf("Tvoja rola je HOST, nastavujes hru\n");
    }else {
        klient->role = CLIENT;
        klient->playerBoard = gameTerminal->boardClient_2;
        printf("Tvoja rola je CLIENT, cakas na hru\n");
    }
}
void initPlayer(KLIENT * klient) {
    klient->playerMove = 0;
    klient->score = 0;
}
void* klientF (void *arg) {
    KLIENT *klient = arg;
    initPlayer(klient);

    while (strcmp(klient->msg, "koniec\0") != 0) {
        bzero(klient->msg, 256);
        receiveMsg_Client(klient->sockfd, klient->msg);
        printf("%s\n", klient->msg);

    }
    sendMsg_Client(klient->sockfd, "koniec\0");

    return NULL;
}

int client()
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent* server;

    server = gethostbyname("frios2.fri.uniza.sk");
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
    serv_addr.sin_port = htons(18488);

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
    createBoard(&gameTerminal);
    KLIENT klientData;
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
        sprintf(klientData.msg, "%d;%d", timer, moves);
        printf("Concatenated String: %s\n", klientData.msg);
        sendMsg_Client(klientData.sockfd,klientData.msg);
        clearMessage(&klientData);
        initGameSettings(&gameClient, &gameTerminal, 4, moves - 1, timer - 1);
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

    //ak je to klient tak musi obdrzat nastavenia hry teda gameMoves,gameTimer
    if (klientData.role == CLIENT) {
        sendMsg_Client(klientData.sockfd,"mam\0");
        receiveMsg_Client(klientData.sockfd,klientData.msg);
        printf("%s\n", klientData.msg);
    }
    //printBoard(&gameClient);

    /*if (strcmp(klientData.msg,"hraj\0") == 0) {
        //server bude mat na svojej strane pole intov jedneho a druheho hraca
        /*
         * kazdy hrac zoberie svoje hodnoty z boardu a od 0 po 15 index
         * posle pole tychto hodnot 0;2;4;0;16...atd kazdu sekundu servru
         * server spracuje a da vediet druhemu klientovi hodnoty
         * Sekunda
         * klient1  generuj cislo
         * klient2  generuj cislo
         * spracuj do pola na odoslianie
         * klient1  Send hodnoty
         * klient2  Send hodnoty
         * serverrecv1
         * serverrecv2
         * spracuj do pola na odoslianie
         * serverSend1
         * serverSend2
         * */
    /*
    }else if(strcmp(klientData.msg,"preskoc\0") == 0) {
        break;
    }*/
    pthread_t klient;
    pthread_create(&klient, NULL, klientF, &klientData);

    while (strcmp(klientData.msg, "k\0") != 0) {
        pthread_mutex_lock(&klientData.mutex);
        bzero(klientData.msg,256);
        printf("zadaj:\n");
        scanf("%s", klientData.msg);
        sendMsg_Client(klientData.sockfd, klientData.msg);
        pthread_mutex_unlock(&klientData.mutex);
    }
    printf("KONIEC:\n");
    pthread_join(klient,NULL);

    pthread_mutex_destroy(&klientData.mutex);
    close(sockfd);
    freeBoard(&gameTerminal);
    return 0;
}