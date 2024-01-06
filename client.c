#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct klient {
    char msg[256];
    int sockfd;
    pthread_mutex_t mutex;
} KLIENT;

bool receiveMsg (int sockfd, char* buffer) {
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

bool sendMsg (int sockfd, char* buffer) {
    int n = write(sockfd, buffer, strlen(buffer));
    if (n < 0)
    {
        perror("Error writing to socket");
        return false;
    }
    return true;
}

void* klientF (void *arg) {
    KLIENT *klient = arg;

    while (strcmp(klient->msg, "koniec\0") != 0) {
        bzero(klient->msg, 256);
        receiveMsg(klient->sockfd, klient->msg);
        printf("%s\n", klient->msg);
    }
    sendMsg(klient->sockfd, "koniec\0");
    return NULL;
}

int main(int argc, char *argv[])
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
    serv_addr.sin_port = htons(10002);

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

    KLIENT klientData;
    klientData.sockfd = sockfd;
    pthread_mutex_init(&klientData.mutex,NULL);

    pthread_t klient;
    pthread_create(&klient, NULL, klientF, &klientData);

    while (strcmp(klientData.msg, "koniec\0") != 0) {
        pthread_mutex_lock(&klientData.mutex);
        bzero(klientData.msg,256);
        scanf("%s", klientData.msg);
        sendMsg(klientData.sockfd, klientData.msg);
        pthread_mutex_unlock(&klientData.mutex);
    }

    pthread_join(klient,NULL);

    pthread_mutex_destroy(&klientData.mutex);
    close(sockfd);

    return 0;
}




//
// Created by PC1 on 04/01/2024.
//

#include "client.h"
