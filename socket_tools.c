#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define LOCALHOST "127.0.0.1"
#define UPPER_PORTS_RANGE 64000
#define LOWER_PORTS_RANGE 1024
#define END_OF_MESSAGE "\r\n\r\n"
#define RECIEVE_BUFFER_SIZE 1024

int GetRandomPort();

int MakeSocket() { return socket(AF_INET, SOCK_STREAM, 0); }

int BindToPort(int socket, const char *ip)
{
  struct sockaddr_in server;
  int port_number = GetRandomPort();

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(ip);
  server.sin_port = htons(port_number);

  while (bind(socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
    port_number = GetRandomPort();
  }
  return port_number;
}

int ListenToConnections(int socket, int number_of_connections) { return listen(socket, number_of_connections); }

int AcceptConnection(int socket, struct sockaddr_in connection)
{
  int addr_len = sizeof(struct sockaddr_in);
  return accept(socket, (struct sockaddr *)&connection, (socklen_t *)&addr_len);
}

int SendMessage(int socket, char *message) { return send(socket, message, strlen(message), 0); }

int RecieveMessage(int socket)
{
  char *message = NULL;
  message = (char *)calloc(RECIEVE_BUFFER_SIZE, sizeof(char));
  int total_read_size = 0, new_read_size = 0;
  while ((new_read_size = recv(socket, message + total_read_size, RECIEVE_BUFFER_SIZE, 0)) > 0) {
    message = (char *)realloc(message, total_read_size + new_read_size);
    total_read_size += new_read_size;
    if (FirstNeedleInHaystack(message, total_read_size, END_OF_MESSAGE, strlen(END_OF_MESSAGE)) != NULL) {
      break;
    } else {
      message = (char *)realloc(message, RECIEVE_BUFFER_SIZE + sizeof(message));
    }
  }
}

int GetRandomPort() { return rand() % (UPPER_PORTS_RANGE - LOWER_PORTS_RANGE) + LOWER_PORTS_RANGE; }

int makeSocket_Bind_Listen(int *socket, FILE *port_documentation_file, char *path, int number_of_connections)
{
  int port_number = 0;
  struct sockaddr_in communication_side;
  socket = MakeSocket();
  port_number = BindToPort(socket, LOCALHOST);
  WriteIntToFile(port_documentation_file, path, port_number);
  ListenToConnections(socket, number_of_connections);
}