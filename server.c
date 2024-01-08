#include "server.h"

int sendMsg (int writeSocket, char* buffer) {
    int n;
    n = write(writeSocket, buffer, strlen(buffer) + 1);
    if (n < 0)
    {
        perror("Error writing to socket");
        return -5;
    }

    return 0;
}

int receiveMsg (int readSocket, char* buffer) {
    int n;
    bzero(buffer,50);
    n = read(readSocket, buffer, 49);
    if (n < 0)
    {
        perror("Error reading from socket");
        return -4;
    }

    return 0;
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

int serverConfig () {
    printf("SERVER: spustam server!\n");
    int sockfd;
    struct sockaddr_in serv_addr;

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(10002);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        return -1;
    }

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error binding socket address");
        return -2;
    }

    printf("SERVER: Cakam na pripojenie hracov\n");

    if (listen(sockfd, PLAYERS_MAX * 2) < 0) {// lebo budu read a write sockety pre kazdeho hraca
        perror("listen");
        exit(EXIT_FAILURE);
    } // prijimanie klien tov

    return sockfd;
}

int clientConfig (int serverSocket, int * writeSocket, int * readSocket) {
    socklen_t writeLen, readLen;
    struct sockaddr_in writeAddr, readAddr;

    writeLen = sizeof(writeAddr);
    readLen = sizeof(readAddr);

    *writeSocket = accept(serverSocket, (struct sockaddr*)&writeAddr, &writeLen);
    if (*writeSocket < 0)
    {
        perror("ERROR on accept write socket");
        return -3;
    }

    *readSocket = accept(serverSocket, (struct sockaddr*)&readAddr, &readLen);
    if (*readSocket < 0)
    {
        close(*writeSocket);
        perror("ERROR on accept read socket");
        return -3;
    }
    //printf("Pripojeny %d,%d\n", *writeSocket, *readSocket);

    return 0;
}

void* clientF (void *arg) {
    char buffer[BUFFER_LENGTH];
    CLIENT_STRUCT_SERVER * client = arg;
    //pthread_mutex_lock(&player->threadData->mutex);
    //pthread_cond_wait(&player->threadData->pokracuj,&player->threadData->mutex);
   // pthread_mutex_unlock(&player->threadData->mutex);
    while (true) {
        pthread_mutex_lock(&client->server->mutex);
        if (client->server->koniec) {
            pthread_mutex_unlock(&client->server->mutex);
            break;
        }
        pthread_mutex_unlock(&client->server->mutex);
        receiveMsg(client->readSocket, buffer);
        printf("%s\n", buffer);
        pthread_mutex_lock(&client->server->mutex);
        if (strcmp(buffer, "k") == 0) {
            client->server->koniec = true;
            pthread_mutex_unlock(&client->server->mutex);
            break;
        } else {
            bzero(client->data, BUFFER_LENGTH);
            stpcpy(client->data, buffer);
        }
        pthread_mutex_unlock(&client->server->mutex);
    }

    return NULL;
}

void* timerF (void *arg) {
    TIMER * timer = arg;

    while (true) {
        if (timer->time == 0) {
            printf("SERVER: cas vyprsal!\n");
            pthread_mutex_lock(&timer->server->mutex);
            timer->server->koniec = true;
            sendMsg(timer->clients[HOST]->writeSocket, "k");
            sendMsg(timer->clients[CLIENT]->writeSocket, "k");
            pthread_mutex_unlock(&timer->server->mutex);

            break;
        }
        pthread_mutex_lock(&timer->server->mutex);
        if (timer->server->koniec) {
            pthread_mutex_unlock(&timer->server->mutex);
            break;
        }

        sendMsg(timer->clients[HOST]->writeSocket, timer->clients[CLIENT]->data);
        sendMsg(timer->clients[CLIENT]->writeSocket, timer->clients[HOST]->data);

        pthread_mutex_unlock(&timer->server->mutex);
        printf("SERVER: zostavajuci cas: %d\n", timer->time);
        sleep(3);
        timer->time -= 3;
    }

    return NULL;
}



int server()
{
    int error;
    SERVER serverData;
    serverData.socket = serverConfig();

    if (serverData.socket < 0) {
        return abs(serverData.socket);
    }
    serverData.koniec = false;
    pthread_mutex_init(&serverData.mutex, NULL);

    CLIENT_STRUCT_SERVER clientData[PLAYERS_MAX];
    for (int i = 0; i < PLAYERS_MAX; ++i) {
        clientData[i].id = i;
        bzero(clientData[i].data, BUFFER_LENGTH);
        clientData[i].server = &serverData;
        error = abs(clientConfig(serverData.socket, &clientData[i].writeSocket, &clientData[i].readSocket));
        if (error) {
            close(serverData.socket);
            return error;
        }
    }

    char buffer[BUFFER_LENGTH];

    TIMER timerData;
    //timerData.time = 30; nastavit potom
    for (int i = 0; i < PLAYERS_MAX; ++i) {
        timerData.clients[i] = &clientData[i];
    }
    timerData.server = &serverData;

    sendMsg(timerData.clients[HOST]->writeSocket, "host");
    sendMsg(timerData.clients[CLIENT]->writeSocket, "klient");

    receiveMsg(timerData.clients[HOST]->readSocket,buffer);// time;move hodnoty
    printf("Nastavenie v poradi time;moves %s\n", buffer);

    char timerCH = buffer[0];
    int timerSet = timerCH - '0';
    timerData.time = timerSet * TO_SECONDS;

    sendMsg(timerData.clients[HOST]->writeSocket,"zobrazH");
    sendMsg(timerData.clients[CLIENT]->writeSocket,"zobrazC");

    //receiveMsg(timerData.clients[HOST]->readSocket,timerData.clients[CLIENT]->data);
    //printf("Potvrdenie od clienta %s\n", timerData.clients[CLIENT]->data);
    sendMsg(timerData.clients[CLIENT]->writeSocket,buffer); //posielam klientovi nastavenia time;move

    pthread_t clients[PLAYERS_MAX];
    pthread_t timer;

    for (int i = 0; i < PLAYERS_MAX; ++i) {
        pthread_create(&clients[i], NULL, clientF, &clientData[i]);
    }
    pthread_create(&timer, NULL, timerF, &timerData);

    for (int i = 0; i < PLAYERS_MAX; ++i) {
        pthread_join(clients[i], NULL);
    }
    pthread_join(timer, NULL);

    pthread_mutex_destroy(&serverData.mutex);
    for (int i = 0; i < PLAYERS_MAX; ++i) {
        close(clientData[i].writeSocket);
        close(clientData[i].readSocket);
    }
    close(serverData.socket);

    return 0;
}

