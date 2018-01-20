#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define LOCALHOST "127.0.0.1"
#define UPPER_PORTS_RANGE 64000
#define LOWER_PORTS_RANGE 1024
#define MESSAGE_TERMINATION "\r\n\r\n"
#define RECIEVE_BUFFER_SIZE 1024

int GetRandomPort();
char *FirstNeedleInHaystack_ByOccurrence(char *haystack, int haystack_size, char *needle, int needle_size);
void WriteIntToFile(FILE *target, char *path, int int_content);

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

char *RecieveMessage(int socket, bool isClient)
{
  char *message = NULL, first_message_termination;
  message = (char *)calloc(RECIEVE_BUFFER_SIZE, sizeof(char));
  int total_read_size = 0, new_read_size = 0;
  while ((new_read_size = recv(socket, message + total_read_size, RECIEVE_BUFFER_SIZE, 0)) > 0) {
    message = (char *)realloc(message, total_read_size + new_read_size);
    total_read_size += new_read_size;
    if (isClient) {
      /*if (FirstNeedleInHaystack_ByOccurrence(message, total_read_size, MESSAGE_TERMINATION, strlen(MESSAGE_TERMINATION,1)) != NULL) {
        break;
      } else {
        first_message_termination =
            FirstNeedleInHaystack_ByOccurrence(message, total_read_size, MESSAGE_TERMINATION, strlen(MESSAGE_TERMINATION),1);
        if (FirstNeedleInHaystack_ByOccurrence(message + strlen(MESSAGE_TERMINATION), total_read_size, MESSAGE_TERMINATION,
                                  strlen(MESSAGE_TERMINATION)) != NULL) {
          break;
        }
      }

    */} else {
        message = (char *)realloc(message, RECIEVE_BUFFER_SIZE + sizeof(message));
      }
  }
  return message;
}

int GetRandomPort() { return rand() % (UPPER_PORTS_RANGE - LOWER_PORTS_RANGE) + LOWER_PORTS_RANGE; }

int makeSocket_Bind_Listen(int *socket, FILE *port_documentation_file, char *path, int number_of_connections)
{
  int port_number = 0;
  struct sockaddr_in communication_side;
  *socket = MakeSocket();
  port_number = BindToPort(*socket, LOCALHOST);
  WriteIntToFile(port_documentation_file, path, port_number);
  ListenToConnections(*socket, number_of_connections);
}

void WriteIntToFile(FILE *target, char *path, int int_content)
{
  target = fopen(path, "w");
  fprintf(target, "%d", int_content);
  fclose(target);
}
char *NeedleInHaystack_ByOccurrence(char *haystack, int haystack_size, char *needle, int needle_size,
                                    int required_occurrence)
{
  int i = 0, j = 0;
  int match_occurrence = 0;
  bool match = false;
  if (haystack_size >= needle_size) {
    for (i = 0; i < haystack_size; i++) {
      for (j = 0; j < needle_size; j++) {
        if (!(needle[j] == haystack[i + j])) {
          break;
        }
      }
      if (j == needle_size) {
        match_occurrence++;
      }
      if (match_occurrence == required_occurrence) {
        return haystack + i;
      }
    }
  }
  return NULL;
}