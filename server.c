#include "server.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct threadData {
    bool start;
    bool koniec;
    pthread_mutex_t mutex;
} THREAD_DATA;

typedef struct player {
    int serverSock;
    int playerSock;
    int id;
    char data[100];
    char msg[256];
    THREAD_DATA threadData;
} PLAYER;

typedef struct game {
    int time;
    int** board;
    PLAYER* players;
    THREAD_DATA threadData;
} GAME;

bool receiveMsg (int sockfd, char* buffer) {
    bzero(buffer,256);
    int n = read(sockfd, buffer, 255);
    if (n < 0)
    {
        perror("Error reading from socket");
        return false;
    }
    return true;
}

bool sendMsg (int sockfd, char* buffer) {
    int n = write(sockfd, buffer, strlen(buffer)+1);
    if (n < 0)
    {
        perror("Error writing to socket");
        return false;
    }
    return true;
}

void* hracF (void *arg) {
    PLAYER *player = arg;

    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);
    player->playerSock = accept(player->serverSock, (struct sockaddr*)&cli_addr, &cli_len); // akceptovanie klienta
    if (player->playerSock < 0)
    {
        perror("ERROR on accept");
        return NULL;
    }

    player->threadData.start = true;

    pthread_mutex_lock(&player->threadData.mutex);
    sendMsg(player->playerSock, "start\0");
    pthread_mutex_unlock(&player->threadData.mutex);

    while (strcmp(player->msg, "koniec\0") != 0) {
        pthread_mutex_lock(&player->threadData.mutex);
        receiveMsg(player->playerSock, player->msg);
        printf("HRAC %d: %s\n", player->id, player->msg);
        pthread_mutex_unlock(&player->threadData.mutex);
    }
    pthread_mutex_lock(&player->threadData.mutex);
    player->threadData.koniec = true;
    pthread_mutex_unlock(&player->threadData.mutex);

    close(player->playerSock);
    return NULL;
}

void* timeF (void *arg) {
    GAME *game = arg;
    while (!game->players[0].threadData.start || !game->players[1].threadData.start) {

    }
    while (!game->players[0].threadData.koniec && !game->players[1].threadData.koniec) {
        if (game->time == 0) {
            printf("SERVER: cas vyprsal!\n");
            pthread_mutex_lock(&game->threadData.mutex);
            game->players[0].threadData.koniec = true;
            pthread_mutex_unlock(&game->threadData.mutex);
        } else {
            printf("SERVER: zostavajuci cas: %d\n", game->time);
            pthread_mutex_lock(&game->threadData.mutex);
            sendMsg(game->players[0].playerSock, "1\0");
            sendMsg(game->players[1].playerSock, "0\0");
            pthread_mutex_unlock(&game->threadData.mutex);
        }
        game->time--;
        sleep(1);
    }

    pthread_mutex_lock(&game->threadData.mutex);
    sendMsg(game->players[0].playerSock, "koniec\0");
    sendMsg(game->players[1].playerSock, "koniec\0");
    pthread_mutex_unlock(&game->threadData.mutex);

    return NULL;
}

int main(int argc, char *argv[])
{
    printf("SERVER: spustam server!\n");
    int sockfd;
    struct sockaddr_in serv_addr;

    bzero((char*)&serv_addr, sizeof(serv_addr)); // inicializacia sietovej adresy
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(10002);

    sockfd = socket(AF_INET, SOCK_STREAM, 0); // vytvorenie socketu
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) // priradenie adresu socketu
    {
        perror("Error binding socket address");
        return 2;
    }

    listen(sockfd, 5); // prijimanie klientov

    printf("SERVER: cakam na pripojenie hracov!\n");

    THREAD_DATA threadData;
    threadData.start = false;
    threadData.koniec = false;
    pthread_mutex_init(&threadData.mutex,NULL);

    PLAYER playerData[2];
    for (int i = 0; i < 2; ++i) {
        playerData[i].id = i;
        playerData[i].serverSock = sockfd;
        playerData[i].threadData = threadData;
    }

    GAME gameData;
    gameData.time = 60;
    gameData.board = (int**)calloc(5, sizeof(int*));
    for (size_t i = 0; i < 5; ++i) {
        gameData.board[i] = (int*)calloc(5, sizeof(int));
    }
    gameData.players = playerData;
    gameData.threadData = threadData;

    pthread_t players[2];
    pthread_t timer;
    for (int i = 0; i < 2; ++i) {
        pthread_create(&players[i], NULL, hracF, &playerData[i]);
    }
    pthread_create(&timer, NULL, timeF, &gameData);

    for (int i = 0; i < 2; ++i) {
        pthread_join(players[i], NULL);
    }
    pthread_join(timer, NULL);

    pthread_mutex_destroy(&threadData.mutex);
    close(sockfd);
    free(gameData.board);

    return 0;
}





//
// Created by PC1 on 04/01/2024.
//

#include "server.h"
/*int server(int argc, char *argv[]){
    srand(time(NULL));
    int sockfd;
    struct sockaddr_in serv_addr;

    if (argc < 2)
    {
        fprintf(stderr,"usage %s port\n", argv[0]);
        return 1;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error binding socket address");
        return 2;
    }
    listen(sockfd, 5);
    printf("------------------------------------------------------\n");
    printf("Server sa  spustil, nastavte parametre hry.\n");
    printf("------------------------------------------------------\n\n");

    while (maxPocetHracov < 2 || maxPocetHracov > 3){
        printf("*** Vyberte z maximalneho poctu hracov (2-3) ***\n");
        scanf("%d",&maxPocetHracov);
        if (maxPocetHracov < 2 || maxPocetHracov > 3){
            printf("Zadali ste nespravnu volbu, zadajte znova !\n");
        }
    }

    if (maxPocetHracov == 2){
        while (velkostMapy < 3 || velkostMapy > 9){
            printf("*** Vyberte z rozmedzia velkosti mapy, minimalne (3), maximalne (9) ***\n");
            scanf("%d",&velkostMapy);
            if (velkostMapy < 3 || velkostMapy > 9){
                printf("Zadali ste nespravnu volbu, zadajte znova !\n");
            }
        }
    } else{
        while (velkostMapy < 5 || velkostMapy > 9){
            printf("*** Vyberte z rozmedzia velkosti mapy, minimalne (5), maximalne (9) ***\n");
            scanf("%d",&velkostMapy);
            if (velkostMapy < 5 || velkostMapy > 9){
                printf("Zadali ste nespravnu volbu, zadajte znova !\n");
            }
        }
    }

    zacinajuci = rand() % maxPocetHracov;
    char **mapa = malloc(velkostMapy * sizeof(char *));
    for (int i = 0; i < velkostMapy; ++i) {
        mapa[i] = malloc(velkostMapy* sizeof(char));
        for(int j=0; j<velkostMapy; j++){
            mapa[i][j] = '-' ;
        }
    }

    printf("------------------------------------------------------\n");
    printf("Hra bola uspesne nastavena s parametrami:\npocet hracov %d\nvelkost mapy:%dx%d\n",maxPocetHracov,velkostMapy,velkostMapy);
    printf("------------------------------------------------------\n\n");

    pthread_t hraci[maxPocetHracov], kontrola;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex,NULL);
    pthread_barrier_t cakajHracov;
    pthread_barrier_init(&cakajHracov,NULL,maxPocetHracov);

    SERVERD servD = {0, maxPocetHracov, 0, menoVyhercu, 0, zacinajuci, mapa, velkostMapy, infoVyhry, &cakajHracov, &mutex};
    HRACD * hraciD = malloc(maxPocetHracov * sizeof(HRACD));

    printf("Caka sa na %d hracov...\n",servD.maximalnyPocHracov);

    // Pthread 'create' / 'join'
    for (int i = 0; i < maxPocetHracov; i++) {
        hraciD[i].idHraca = i;
        hraciD[i].sockfd = sockfd;
        hraciD[i].infoHraca = infoHraca;
        hraciD[i].infoHry = infoHry;
        hraciD[i].okInfo = okInfo;
        hraciD[i].spoluD = &servD;
        pthread_create(&hraci[i], NULL, hraciFun, &hraciD[i]);
    }


    for (int i = 0; i < maxPocetHracov; i++) {
        pthread_join(hraci[i], NULL);
    }





    printf("------------------------------------------------------\n");
    printf("Server Skoncil\n");
    printf("------------------------------------------------------\n\n");
    close(sockfd);

    return 0;
}*/