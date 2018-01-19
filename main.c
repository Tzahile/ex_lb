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

void WriteIntToFile(FILE *target, char *path, int int_content);
char *FirstNeedleInHaystack(char *haystack, int haystack_size, char *needle, int needle_size);
int main()
{
  int server_side_socket, port_number, client_side_socket, client_side_accepted_socket, server_socket_index,
      new_read_size = 0, total_read_size = 0;
  int server_side_accepted_socket[NUMBER_OF_SERVERS];
  struct sockaddr_in client_side, server_side;
  char *client_message = NULL, server_message = NULL;
  client_message = (char *)calloc(RECIEVE_BUFFER_SIZE, sizeof(char));
  server_message = (char *)calloc(RECIEVE_BUFFER_SIZE, sizeof(char));
  // bool end_of_client_message;
  FILE *server_port_file = fopen("server_port", "w");
  FILE *http_port_file = fopen("server_port", "w");
  srand(time(NULL));

  makeSocket_Bind_Listen(server_side_socket, server_port_file, SERVER_PORT_FILE_PATH, 3);

  for (server_socket_index = 0; server_socket_index < NUMBER_OF_SERVERS; server_socket_index++) {
    server_side_accepted_socket[server_socket_index] = AcceptConnection(server_side_socket, server_side);
  }
  makeSocket_Bind_Listen(client_side_socket, http_port_file, HTTP_PORT_FILE_PATH, 1);

  client_side_accepted_socket = AcceptConnection(client_side_socket, client_side);

  server_socket_index = 0;
  while (strcmp(client_message, "EXIT PROGRAM\r\n\r\n")) {
    // while ((new_read_size =
    //             recv(client_side_accepted_socket, client_message + total_read_size, RECIEVE_BUFFER_SIZE, 0)) > 0) {
    //   client_message = (char *)realloc(client_message, total_read_size + new_read_size);
    //   total_read_size += new_read_size;
    //   if (FirstNeedleInHaystack(client_message, total_read_size, END_OF_CLIENT_MESSAGE,
    //                             strlen(END_OF_CLIENT_MESSAGE)) != NULL) {
    //     break;
    //   } else {
    //     client_message = (char *)realloc(client_message, RECIEVE_BUFFER_SIZE + sizeof(client_message));
    //   }
    // }
    send(server_side_accepted_socket[server_socket_index], client_message, total_read_size, 0);
    server_socket_index++;
    total_read_size = 0;

    while ((new_read_size =
                recv(server_side_accepted_socket, server_message + total_read_size, RECIEVE_BUFFER_SIZE, 0)) > 0) {
      server_message = (char *)realloc(server_message, total_read_size + new_read_size);
      total_read_size += new_read_size;
      // need to add search for first occurance of \r\n\r\n
      if (FirstNeedleInHaystack(client_message, total_read_size, END_OF_CLIENT_MESSAGE,
                                strlen(END_OF_CLIENT_MESSAGE)) != NULL) {
        break;
      } else {
        client_message = (char *)realloc(client_message, RECIEVE_BUFFER_SIZE + sizeof(client_message));
      }
    }
  }
}

void WriteIntToFile(FILE *target, char *path, int int_content)
{
  target = fopen(path, "w");
  fprintf(target, "%d", int_content);
  fclose(target);
}
char *FirstNeedleInHaystack(char *haystack, int haystack_size, char *needle, int needle_size)
{
  int i = 0, j = 0;
  bool match = false;
  if (haystack_size >= needle_size) {
    for (i = 0; i < haystack_size; i++) {
      for (j = 0; j < needle_size; j++) {
        if (!(needle[j] == haystack[i + j])) {
          break;
        }
      }
      if (j == needle_size) {
        return haystack + i;
      }
    }
  }
  return NULL;
}