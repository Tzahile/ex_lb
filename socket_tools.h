#ifndef SOCKET_TOOLS
#define SOCKET_TOOLS

int AcceptConnection(int socket, struct sockaddr_in connection);
char* RecieveMessage(int socket, bool isClient);
void makeSocket_Bind_Listen(int* socket, FILE* port_documentation_file, char* path, int number_of_connections);

#endif