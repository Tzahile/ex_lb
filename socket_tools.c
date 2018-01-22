#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#define UPPER_PORTS_RANGE 64000
#define LOWER_PORTS_RANGE 1024
#define MESSAGE_TERMINATION "\r\n\r\n"
#define RECIEVE_BUFFER_SIZE 1024

int BindToPort(int socket, const char *ip);
int MakeSocket();
int GetRandomPort();
char *NeedleInHaystack_ByOccurrence(char *haystack, int haystack_size, char const *needle, int needle_size,
                                    int required_occurrence);
void WriteIntToFile(FILE *target_file, char *path, int int_content);

int MakeSocket() { return socket(AF_INET, SOCK_STREAM, 0); }

int BindToPort(int socket, const char *ip)
{
  struct sockaddr_in server;
  int port_number = GetRandomPort();

  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(ip);
  server.sin_port = htons(80);

  while (bind(socket, (struct sockaddr *)&server, sizeof(server)) < 0) {
    port_number = GetRandomPort();
    server.sin_port = htons(port_number);
  }
  return port_number;
}

int ListenToConnections(int socket, int number_of_connections) { return listen(socket, number_of_connections); }

int AcceptConnection(int socket, struct sockaddr_in connection)
{
  int addr_len = sizeof(struct sockaddr_in);
  return accept(socket, (struct sockaddr *)&connection, (socklen_t *)&addr_len);
}

int SendMessage(int socket, char *message, int message_length) { return send(socket, message, message_length, 0); }

char *RecieveMessage(int socket, int *buffer_final_size, bool isClient)
{
  char *message = NULL, *message_last_termination = NULL;
  message = (char *)calloc(RECIEVE_BUFFER_SIZE, sizeof(char));
  int total_read_size = 0, new_read_size = 0;
  while ((new_read_size = recv(socket, message + total_read_size, RECIEVE_BUFFER_SIZE, 0)) > 0) {
    message = (char *)realloc(message, total_read_size + new_read_size);
    total_read_size += new_read_size;
    if (isClient) {
      message_last_termination =
          NeedleInHaystack_ByOccurrence(message, total_read_size, MESSAGE_TERMINATION, strlen(MESSAGE_TERMINATION), 1);
      if (message_last_termination != NULL) {
        break;
      }
    } else {
      message_last_termination =
          NeedleInHaystack_ByOccurrence(message, total_read_size, MESSAGE_TERMINATION, strlen(MESSAGE_TERMINATION), 2);
      if (message_last_termination != NULL) {
        break;
      }
    }
    message = (char *)realloc(message, RECIEVE_BUFFER_SIZE + sizeof(message));
  }
  *buffer_final_size = total_read_size;
  return message;
}

void FreeRecievedMessage(char *message) { free(message); }

int GetRandomPort() { return rand() % (UPPER_PORTS_RANGE - LOWER_PORTS_RANGE) + LOWER_PORTS_RANGE; }

void MakeSocketAndBind(int *socket, FILE *port_documentation_file, char *path, const char *ip)
{
  int port_number = 0;
  *socket = MakeSocket();
  port_number = BindToPort(*socket, ip);
  WriteIntToFile(port_documentation_file, path, port_number);
}

int CloseSocket(int socket)
{
  shutdown(socket, SHUT_RDWR);
  return close(socket);
}

void WriteIntToFile(FILE *target_file, char *path, int int_content)
{
  target_file = fopen(path, "w");
  fprintf(target_file, "%d", int_content);
  fclose(target_file);
}
char *NeedleInHaystack_ByOccurrence(char *haystack, int haystack_size, const char *needle, int needle_size,
                                    int required_occurrence)
{
  int i = 0, j = 0;
  int match_occurrence = 0;
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