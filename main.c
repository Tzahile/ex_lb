#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#include "socket_tools.h"

#define NUMBER_OF_SERVERS 3
#define NUMBER_OF_CLIENTS 1

int main()
{
  int server_side_socket, client_side_socket, client_side_accepted_socket, server_socket_index;
  int server_side_accepted_socket[NUMBER_OF_SERVERS];
  int recieved_message_size = 0;
  struct sockaddr_in client_side, server_side;
  char *client_message = NULL, *server_message = NULL;
  FILE *server_port_file = NULL;
  FILE *http_port_file = NULL;
  srand(time(NULL));
  MakeSocketAndBind(&server_side_socket, server_port_file, SERVER_PORT_FILE_PATH, LOCALHOST);
  MakeSocketAndBind(&client_side_socket, http_port_file, HTTP_PORT_FILE_PATH, NOVA_IP);
  ListenToConnections(server_side_socket, NUMBER_OF_SERVERS);
  for (server_socket_index = 0; server_socket_index < 1; server_socket_index++) {
    server_side_accepted_socket[server_socket_index] = AcceptConnection(server_side_socket, server_side);
  }
  server_socket_index = 0;
  while (true) {
    ListenToConnections(client_side_socket, NUMBER_OF_CLIENTS);
    client_side_accepted_socket = AcceptConnection(client_side_socket, client_side);
    client_message = RecieveMessage(client_side_accepted_socket, &recieved_message_size, true);
    SendMessage(server_side_accepted_socket[server_socket_index], client_message, recieved_message_size);
    FreeRecievedMessage(client_message);
    server_message = RecieveMessage(server_side_accepted_socket[server_socket_index], &recieved_message_size, false);
    SendMessage(client_side_accepted_socket, server_message, recieved_message_size);
    FreeRecievedMessage(server_message);
    server_socket_index++;
    server_socket_index %= 3;
    CloseSocket(client_side_accepted_socket);
  }
}