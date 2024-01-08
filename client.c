#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct client {
    bool koniec;
    pthread_mutex_t mutex;
} CLIENT;

typedef struct receive {
    int readSocket;
    CLIENT * client;
} RECEIVE;

typedef struct send {
    int writeSocket;
    CLIENT * client;
} SEND;

int config (int * readSocket, int * writeSocket) {
    struct sockaddr_in serv_addr;
    struct hostent* server;

    server = gethostbyname("frios2.fri.uniza.sk");
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
    serv_addr.sin_port = htons(10002);

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

int sendMsg (int writeSocket, char* buffer) {
    int n;
    n = write(writeSocket, buffer, strlen(buffer));
    if (n < 0)
    {
        perror("Error writing to socket");
        return -5;
    }

    return 0;
}

int receiveMsge (int readSocket, char* buffer) {
    int n;
    bzero(buffer,50);
    n = read(readSocket, buffer, 49);
    if (n < 0)
    {
        perror("Error reading from socket");
        return -6;
    }

    return 0;
}

void * move (void * arg) {
    SEND * send = arg;
    char buffer[50];

    while (true) {
        pthread_mutex_lock(&send->client->mutex);
        if (send->client->koniec) {
            pthread_mutex_unlock(&send->client->mutex);
            break;
        }
        pthread_mutex_unlock(&send->client->mutex);
        printf("move: ");
        scanf(" %s", buffer);
        pthread_mutex_lock(&send->client->mutex);
        if (strcmp(buffer, "k") == 0) {
            sendMsg(send->writeSocket, buffer);
            send->client->koniec = true;
            pthread_mutex_unlock(&send->client->mutex);
            break;
        }
        //pohyb v smere a az potom poslanie udajov
        sendMsg(send->writeSocket, buffer);
        pthread_mutex_unlock(&send->client->mutex);
        usleep(1000);
    }

    return NULL;
}

void * update (void * arg) {
    RECEIVE * receive = arg;
    char buffer[50];
    bzero(buffer, 50);

    while (true) {
        pthread_mutex_lock(&receive->client->mutex);
        if (receive->client->koniec) {
            pthread_mutex_unlock(&receive->client->mutex);
            break;
        }
        pthread_mutex_unlock(&receive->client->mutex);
        receiveMsge(receive->readSocket, buffer);
        printf("%s\n", buffer);
        //vypis na aktualneho pola
        //pthread_mutex_unlock(&receive->client->mutex);
        usleep(1000);
    }

    return NULL;
}

int main() {
    int error;
    CLIENT clientData;
    clientData.koniec = false;
    pthread_mutex_init(&clientData.mutex, NULL);

    RECEIVE receiveData;
    receiveData.client = &clientData;

    SEND sendData;
    sendData.client = &clientData;

    error = config(&receiveData.readSocket, &sendData.writeSocket);
    if (error) {
        return abs(error);
    }

    sendMsg(sendData.writeSocket, "start");

    pthread_t receiving;
    pthread_t sending;

    pthread_create(&sending, NULL, move, &sendData);
    pthread_create(&receiving, NULL, update, &receiveData);

    pthread_join(sending, NULL);
    pthread_join(receiving, NULL);

    printf("oka\n");

    pthread_mutex_destroy(&clientData.mutex);
    close(receiveData.readSocket);
    close(sendData.writeSocket);

    return 0;
}
