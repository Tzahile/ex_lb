#ifndef SOCKET_TOOLS
#define SOCKET_TOOLS

int MakeSocket();
int BindToPort(int socket, const char *ip);
int ListenToConnections(int socket, int number_of_connections);
int AcceptConnection(int socket, struct sockaddr_in connection);
int SendMessage(int socket, char *message);
int RecieveMessage(int socket, char *reply, int buffer);
#endif