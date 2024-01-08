#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_CLIENTS 2

typedef struct server {
    int socket;
    bool koniec;
    pthread_mutex_t mutex;
} SERVER;

typedef struct client {
    int id;
    int writeSocket;
    int readSocket;
    char data[50];
    SERVER * server;
} CLIENT;

typedef struct timer {
    int time;
    CLIENT * clients[MAX_CLIENTS];
    SERVER * server;
} TIMER;

int serverConfig () {
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
    listen(sockfd, 4);
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
    printf("Pripojeny %d,%d\n", *writeSocket, *readSocket);

    return 0;
}

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

void* clientF (void *arg) {
    char buffer[50];
    CLIENT * client = arg;

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
            bzero(client->data, 50);
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
            pthread_mutex_lock(&timer->server->mutex);
            timer->server->koniec = true;
            sendMsg(timer->clients[0]->writeSocket, "k");
            sendMsg(timer->clients[1]->writeSocket, "k");
            pthread_mutex_unlock(&timer->server->mutex);

            break;
        }
        pthread_mutex_lock(&timer->server->mutex);
        if (timer->server->koniec) {
            pthread_mutex_unlock(&timer->server->mutex);
            break;
        }
        //pthread_mutex_unlock(&timer->server->mutex);
        /*sendMsg(timer->clients[0]->writeSocket, timer->clients[1]->data);
        sendMsg(timer->clients[1]->writeSocket, timer->clients[0]->data);*/
        sendMsg(timer->clients[0]->writeSocket, "prvy");
        sendMsg(timer->clients[1]->writeSocket, "druhy");
        pthread_mutex_unlock(&timer->server->mutex);

        printf("%d\n", timer->time);
        sleep(3);
        timer->time -= 3;
    }

    return NULL;
}

int main() {
    int error;
    SERVER serverData;
    serverData.socket = serverConfig();
    if (serverData.socket < 0) {
        return abs(serverData.socket);
    }
    serverData.koniec = false;
    pthread_mutex_init(&serverData.mutex, NULL);

    CLIENT clientData[MAX_CLIENTS];
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        clientData[i].id = i;
        bzero(clientData[i].data, 50);
        clientData[i].server = &serverData;
        error = abs(clientConfig(serverData.socket, &clientData[i].writeSocket, &clientData[i].readSocket));
        if (error) {
            close(serverData.socket);
            return error;
        }
    }

    TIMER timerData;
    timerData.time = 30;
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        timerData.clients[i] = &clientData[i];
    }
    timerData.server = &serverData;

    pthread_t clients[MAX_CLIENTS];
    pthread_t timer;

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        pthread_create(&clients[i], NULL, clientF, &clientData[i]);
    }
    pthread_create(&timer, NULL, timerF, &timerData);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        pthread_join(clients[i], NULL);
    }
    pthread_join(timer, NULL);

    pthread_mutex_destroy(&serverData.mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        close(clientData[i].writeSocket);
        close(clientData[i].readSocket);
    }
    close(serverData.socket);

    return 0;
}