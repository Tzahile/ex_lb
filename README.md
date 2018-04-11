# ex_lb

Project in C Lab, dealing with Sockets and Makefile compiling method.

A computer (written in C) gets counting-requests from the client (website) through a dedicated socket.
The computer passes the request through another socket to 3 servers (round robin method), written in Python, to deal with the requests.
Each server responds with the number of times it got a request.
