#ifndef SOCKET_TOOLS
#define SOCKET_TOOLS

#define HTTP_PORT_FILE_PATH "http_port"
#define SERVER_PORT_FILE_PATH "server_port"
#define LOCALHOST "127.0.0.1"
#define NOVA_IP "132.67.192.133"

int AcceptConnection(int socket, struct sockaddr_in connection);
int ListenToConnections(int socket, int number_of_connections);
int SendMessage(int socket, const char* message, int message_length);
char* RecieveMessage(int socket, int* buffer_final_size, bool isClient);
void FreeRecievedMessage(char* message);
void MakeSocketAndBind(int* socket, FILE* port_documentation_file, char* path, const char* ip);
int CloseSocket(int socket);

#endif