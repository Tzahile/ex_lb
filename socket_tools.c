#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

#define UPPER_PORTS_RANGE 64000
#define LOWER_PORTS_RANGE 1024

int GetRandomPort();

int MakeSocket() { return socket(AF_INET, SOCK_STREAM, 0); }

int BindToPort(int socket, const char *ip) {
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

int ListenToConnections(int socket, int number_of_connections) {
  return listen(socket, number_of_connections);
}

int AcceptConnection(int socket, struct sockaddr_in connection) {
  int addr_len = sizeof(struct sockaddr_in);
  return accept(socket, (struct sockaddr *)&connection, (socklen_t *)&addr_len);
}

int SendMessage(int socket, char *message){
  return send(socket, message, strlen(message), 0);
}

int RecieveMessage(int socket, char *reply, int buffer){
  return recv(socket , reply , buffer , 0);
}

int GetRandomPort() {
  return rand() % (UPPER_PORTS_RANGE - LOWER_PORTS_RANGE) + LOWER_PORTS_RANGE;
}
