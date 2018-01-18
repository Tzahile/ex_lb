#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "socket_tools.h"
#define LOCALHOST "127.0.0.1"
#define NUMBER_OF_SERVERS 3

int main() {
  int server_side_socket, port_number, client_side_socket,
      client_side_accepted_socket, i;
  int server_side_accepted_socket[NUMBER_OF_SERVERS];
  struct sockaddr_in client_side, server_side;
  // char client_message;
  FILE *server_port_file = fopen("server_port", "w");
  FILE *http_port_file = fopen("server_port", "w");
  void WriteIntToFile(FILE * target, char *path, int int_content);
  srand(time(NULL));

  client_side_socket = MakeSocket();
  server_side_socket = MakeSocket();

  port_number = BindToPort(client_side_socket, LOCALHOST);
  WriteIntToFile(http_port_file, "http_port", port_number);

  port_number = BindToPort(server_side_socket, LOCALHOST);
  WriteIntToFile(server_port_file, "server_port", port_number);

  ListenToConnections(server_side_socket, NUMBER_OF_SERVERS);
  for (i = 0; i < NUMBER_OF_SERVERS; i++) {
    server_side_accepted_socket[i] =
        AcceptConnection(server_side_socket, server_side);
  }

  ListenToConnections(client_side_socket, NUMBER_OF_SERVERS);
  client_side_accepted_socket =
      AcceptConnection(client_side_socket, client_side);
}

void WriteIntToFile(FILE *target, char *path, int int_content) {
  target = fopen(path, "w");
  fprintf(target, "%d", int_content);
  fclose(target);
}
