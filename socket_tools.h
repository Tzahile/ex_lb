#ifndef SOCKET_TOOLS
#define SOCKET_TOOLS

int MakeSocket();
int BindToPort(int socket, const char* ip);
int ListenToConnections(int socket, int number_of_connections);
int AcceptConnection(int socket, struct sockaddr_in connection);
int SendMessage(int socket, char* message);
char* RecieveMessage(int socket, bool isClient);
int makeSocket_Bind_Listen(int* socket, FILE* port_documentation_file, char* path, int number_of_connections);

#endif