#include "server.h"

bool receiveMsg_Server (int sockfd, char* buffer) {
    bzero(buffer,256);
    int n = read(sockfd, buffer, 255);
    if (n < 0)
    {
        perror("Error reading from socket");
        return false;
    }
    return true;
}

bool sendMsg_Server (int sockfd, char* buffer) {
    int n = write(sockfd, buffer, strlen(buffer)+1);
    if (n < 0)
    {
        perror("Error writing to socket");
        return false;
    }
    return true;
}

uint8_t getTimerMode(uint8_t mode) {
    switch (mode) {
        case MINUTE: return MINUTE;
        case TWO_MINUTES: return TWO_MINUTES;
        case THREE_MINUTES: return THREE_MINUTES;
        case FOUR_MINUTES: return FOUR_MINUTES;
        default: return 130;
    }
}

void setTimerMode (uint8_t time,int * gameTime){
    *gameTime = TO_SECONDS * (time);
}


void* hracF (void *arg) {
    PLAYER *player = arg;
    player->threadData->start = true;

    pthread_mutex_lock(&player->threadData->mutex);
    pthread_cond_wait(&player->threadData->pokracuj,&player->threadData->mutex);
    pthread_mutex_unlock(&player->threadData->mutex);

    //receiveMsg_Server(player->playerSock,player->msg);

    while (strcmp(player->msg, "k\0") != 0) {
        //pthread_mutex_lock(&player->threadData->mutex);
        receiveMsg_Server(player->playerSock, player->msg);
        //printf("HRAC sa odpojil hra skoncila %d: %s\n", player->id, player->msg);
        //pthread_mutex_unlock(&player->threadData->mutex);

        pthread_mutex_lock(&player->threadData->mutex);
        bzero(player->data, 100);
        stpcpy(player->data, player->msg);
        printf("%s\n", player->data);
        pthread_mutex_unlock(&player->threadData->mutex);

    }
    pthread_mutex_lock(&player->threadData->mutex);
    player->threadData->koniec = true;
    pthread_mutex_unlock(&player->threadData->mutex);

    //printf("HRAC sa odpojil hra skoncila %d: %s\n", player->id, player->msg);

    close(player->playerSock);
    return NULL;
}

void* timeF (void *arg) {
    GAME *game = arg;
    char buffer0[100], buffer1[100];
    while (!game->players[0].threadData->start || !game->players[1].threadData->start) {

    }
    //pthread_mutex_lock(&game->threadData->mutex);
    sendMsg_Server(game->players[0].playerSock, "host");
    sendMsg_Server(game->players[1].playerSock, "klient");
    receiveMsg_Server(game->players[0].playerSock,game->players[0].msg);
    printf("Nastavenie v poradi time;moves %s\n", game->players[0].msg);
    char timerCH = game->players[0].msg[2];
    int timer = timerCH - '0';
    setTimerMode(getTimerMode(timer),&game->time);
    sendMsg_Server(game->players[0].playerSock,"zobrazH");
    sendMsg_Server(game->players[1].playerSock,"zobrazC");
    receiveMsg_Server(game->players[1].playerSock,game->players[1].msg);
    sendMsg_Server(game->players[1].playerSock,game->players[0].msg);

    pthread_mutex_lock(&game->threadData->mutex);
    pthread_cond_signal(&game->threadData->pokracuj);
    pthread_mutex_unlock(&game->threadData->mutex);

    bool podmienka;
    pthread_mutex_lock(&game->threadData->mutex);
    podmienka = !game->players[0].threadData->koniec && !game->players[1].threadData->koniec;
    pthread_mutex_unlock(&game->threadData->mutex);

    while (podmienka) {
        if (game->time <= 0) {
            printf("SERVER: cas vyprsal!\n");
            pthread_mutex_lock(&game->threadData->mutex);
            game->players[0].threadData->koniec = true;
            pthread_mutex_unlock(&game->threadData->mutex);
        } else {
            printf("SERVER: zostavajuci cas: %d\n", game->time);
            //pthread_mutex_lock(&game->threadData->mutex);
            sprintf(game->players[0].msg, "%d", game->time);
            sprintf(game->players[1].msg, "%d", game->time);
            sendMsg_Server(game->players[0].playerSock, game->players[0].msg);// timer 0
            sendMsg_Server(game->players[1].playerSock, game->players[1].msg);// timer 1
            receiveMsg_Server(game->players[0].playerSock, game->players[0].msg); // pole od HOST
            receiveMsg_Server(game->players[1].playerSock, game->players[1].msg);   // pole od CLIENT
           // strcpy(game->players[0].msg,"512;512;512;512;1024;16;16;16;16;32;32;32;32;1024;1024;1024");
            sendMsg_Server(game->players[0].playerSock, game->players[1].msg);// pole od CLIENT pre HOST
            sendMsg_Server(game->players[1].playerSock, game->players[0].msg);// pole od HOST pre CLIENT
            receiveMsg_Server(game->players[0].playerSock, game->players[0].msg); // move od HOST
            receiveMsg_Server(game->players[1].playerSock, game->players[1].msg);   // move od CLIENT
            sendMsg_Server(game->players[0].playerSock, game->players[1].msg);// move od CLIENT pre HOST
            sendMsg_Server(game->players[1].playerSock, game->players[0].msg);// move od HOST pre CLIENT
            /*bzero(buffer0, 100);
            bzero(buffer1, 100);
            pthread_mutex_lock(&game->threadData->mutex);
            stpcpy(buffer0, game->players[0].data);
            stpcpy(buffer1, game->players[1].data);
            pthread_mutex_unlock(&game->threadData->mutex);

            sendMsg_Server(game->players[1].playerSock, buffer0);
            sendMsg_Server(game->players[0].playerSock, buffer1);
            printf("%s , %s", buffer0, buffer1);*/
            //pthread_mutex_unlock(&game->threadData->mutex);
        }

        game->time -= 3;
        sleep(3);

        pthread_mutex_lock(&game->threadData->mutex);
        podmienka = !game->players[0].threadData->koniec && !game->players[1].threadData->koniec;
        pthread_mutex_unlock(&game->threadData->mutex);
    }

    //pthread_mutex_lock(&game->threadData->mutex);
    sendMsg_Server(game->players[0].playerSock, "k\0");
    sendMsg_Server(game->players[1].playerSock, "k\0");
    //pthread_mutex_unlock(&game->threadData->mutex);

    return NULL;
}

int server()
{
    printf("SERVER: spustam server!\n");
    int sockfd;
    struct sockaddr_in serv_addr;

    bzero((char*)&serv_addr, sizeof(serv_addr)); // inicializacia sietovej adresy
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(18489);

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

    if (listen(sockfd, PLAYERS_MAX) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    } // prijimanie klien tov

    printf("SERVER: cakam na pripojenie hracov!\n");

    THREAD_DATA threadData;
    threadData.start = false;
    threadData.koniec = false;
    pthread_mutex_init(&threadData.mutex,NULL);
    pthread_cond_init(&threadData.pokracuj,NULL);
    PLAYER playerData[2];
    for (int i = 0; i < 2; ++i) {
        playerData[i].id = i;
        playerData[i].serverSock = sockfd;
        playerData[i].threadData = &threadData;
    }

    GAME gameData;
    gameData.time = 60;
    /*gameData.board = (int**)calloc(5, sizeof(int*));
    for (size_t i = 0; i < 5; ++i) {
        gameData.board[i] = (int*)calloc(5, sizeof(int));
    }*/
    gameData.players = playerData;
    gameData.threadData = &threadData;

    pthread_t players[2];
    pthread_t timer;
    struct sockaddr_in cli_addr;
    socklen_t cli_len = sizeof(cli_addr);

    for (int i = 0; i < 2; ++i) {
        playerData[i].playerSock = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len);
        if (playerData[i].playerSock < 0)
        {
            perror("ERROR on accept");
            return 1;
        }
    }
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


/*
int server() {
    pthread_t th_produce;
    pthread_t th_receive;
    struct active_socket my_socket;
    struct thread_data data;
    active_socket_init(&my_socket);
    data.port = 30303;
    data.my_socket = &my_socket;
    printf("spusti\n");
    pthread_create(&th_receive, NULL, process_client_data, &data);

    pthread_join(th_receive, NULL);
    consume(&data);


    printf("%s\n",data.my_socket->received_data.first->data.data);
    active_socket_stop_reading(&my_socket);
    char * message = "client";
    active_socket_write_data(&my_socket,message,6);

    thread_data_destroy(&data);
    active_socket_destroy(&my_socket);
}

int initServerSocket(SERVER *serverInfo) {
    serverInfo->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverInfo->serverSocket < 0) {
        perror("Chyba - socket.");
        return -1;
    }

    serverInfo->opt = 1;
    if (setsockopt(serverInfo->serverSocket, SOL_SOCKET, SO_REUSEADDR, &(serverInfo->opt), sizeof(serverInfo->opt))) {
        perror("setsockopt");
        return -1;
    }

    return 0;
}
int server() {
    SERVER  serverInfo;
    serverInfo.port = 30303;
    printf("Server bol spusteny\n");
    socklen_t client_addr_len;
    pthread_t thread;

    //vytvorenie TCP socketu <sys/socket.h>
    int client_sock;
    initServerSocket(&serverInfo);
    //definovanie adresy servera <arpa/inet.h>
    struct sockaddr_in serverAddress,client_addr;;
    serverAddress.sin_family = AF_INET;         //internetove sockety
    serverAddress.sin_addr.s_addr = INADDR_ANY; //prijimame spojenia z celeho internetu
    serverAddress.sin_port = htons(serverInfo.port);       //nastavenie portu

    //prepojenie adresy servera so socketom <sys/socket.h>
    if (bind(serverInfo.serverSocket, (struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        perror("Chyba - bind.");
    }

    if (listen(serverInfo.serverSocket, PLAYERS_MAX) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    while (1) {
        client_addr_len = sizeof(client_addr);
        client_sock = accept(serverInfo.serverSocket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sock < 0) {
            perror("accept failed");
            return 1;
        }
        printf("Novy klient sa pripojil %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
        if (pthread_create(&thread, NULL, client_handler, &client_sock) != 0) {
            perror("pthread_create failed");
            return 1;
        }
    }
}

void * client_handler(void * arg) {
    int sockfd = *(int *)arg;
    char  buf[100];
    recv(sockfd,buf,99,0);
    send(sockfd,"borec",6,0);
    printf("%s\n", buf);

    return NULL;
}


void* consume(void* thread_data) {
    struct thread_data* data = (struct thread_data*)thread_data;


    return NULL;
}
*/