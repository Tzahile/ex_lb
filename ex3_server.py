#!/usr/bin/python2.7 -tt

from socket import *
server = socket()

server_port_file = open("server_port", "r")
server_port = int(server_port_file.read())

server.connect(('localhost', server_port))
server.send("I'm first  \r\n\r\n")
server.recv(1024)