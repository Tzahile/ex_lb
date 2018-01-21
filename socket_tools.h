#ifndef SOCKET_TOOLS
#define SOCKET_TOOLS

int AcceptConnection(int socket, struct sockaddr_in connection);
int ListenToConnections(int socket, int number_of_connections);
int SendMessage(int socket, char* message, int message_length);
char* RecieveMessage(int socket, int* buffer_final_size, bool isClient);
void FreeRecievedMessage(char* message);
void MakeSocketAndBind(int* socket, FILE* port_documentation_file, char* path, int number_of_connections);
int CloseSocket(int socket);
#endif