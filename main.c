#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "socket_tools.h"

#define HTTP_PORT_FILE_PATH "http_port"
#define SERVER_PORT_FILE_PATH "server_port"
#define NUMBER_OF_SERVERS 3

int main()
{
  int server_side_socket, port_number, client_side_socket, client_side_accepted_socket, server_socket_index,
      new_read_size = 0, total_read_size = 0;
  int server_side_accepted_socket[NUMBER_OF_SERVERS];
  struct sockaddr_in client_side, server_side;
  char *client_message = NULL, *server_message = NULL;
  FILE *server_port_file = fopen("server_port", "w");
  FILE *http_port_file = fopen("server_port", "w");
  srand(time(NULL));

  makeSocket_Bind_Listen(&server_side_socket, server_port_file, SERVER_PORT_FILE_PATH, 3);

  for (server_socket_index = 0; server_socket_index < NUMBER_OF_SERVERS; server_socket_index++) {
    server_side_accepted_socket[server_socket_index] = AcceptConnection(server_side_socket, server_side);
  }
  makeSocket_Bind_Listen(&client_side_socket, http_port_file, HTTP_PORT_FILE_PATH, 1);

  client_side_accepted_socket = AcceptConnection(client_side_socket, client_side);

  server_socket_index = 0;
  while (strcmp(client_message, "EXIT PROGRAM\r\n\r\n")) {
    client_message = RecieveMessage(client_side_accepted_socket, true);
    send(server_side_accepted_socket[server_socket_index], client_message, total_read_size, 0);
    server_socket_index++;
    total_read_size = 0;
    server_message = RecieveMessage(server_side_accepted_socket[server_socket_index], false);
  }
}