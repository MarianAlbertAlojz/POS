#ifndef SOCKETS_CLIENT_MY_SOCKET_H
#define SOCKETS_CLIENT_MY_SOCKET_H

// Verejná štruktúra pre udržanie informácií o sokete
typedef struct {
    int connectSocket;
} MySocket;

// Verejná funkcia pre vytvorenie pripojenia
MySocket* createConnection(const char* hostName, short port);

// Verejná funkcia pre zničenie pripojenia
void destroyConnection(MySocket* socket);

// Verejná funkcia pre odoslanie dát
void sendData(MySocket* socket, const char* data);

// Verejná funkcia pre odoslanie koncovej správy
void sendEndMessage(MySocket* socket);

#endif // SOCKETS_CLIENT_MY_SOCKET_H