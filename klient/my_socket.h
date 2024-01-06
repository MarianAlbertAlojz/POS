#ifndef SOCKETS_CLIENT_MY_SOCKET_H
#define SOCKETS_CLIENT_MY_SOCKET_H

typedef struct {
    int connectSocket;
} MySocket;

MySocket* createConnection(const char* hostName, short port);

void destroyConnection(MySocket* socket);

void sendData(MySocket* socket, const char* data);

void sendEndMessage(MySocket* socket);

void receiveData(MySocket* socket);

#endif // SOCKETS_CLIENT_MY_SOCKET_H