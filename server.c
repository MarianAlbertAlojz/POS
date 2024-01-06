//
// Created by PC1 on 04/01/2024.
//

#include "server.h"
#include "server/pos_sockets/passive_socket.h"
#include "server/pos_sockets/active_socket.h"


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
*/
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

