# Networking

### chatroom:
    
The program has a client-Server Architecture. All the instances of the client connects to the server, when a client sends something, it is received by the server and forwards it to other clients, thus creating a chat-room. This program does this  with the help of select system call.

### HttpServer:
This program is a standrad forking server with support of CGI Scripting. A server is created which waits for a GET verb. The request is read by the server and corresponding reply is send back to the client (A Web browser).

### py_tcp:
Is a program written in Python which creates a simple client Server. Client sends some data to the Server and the Server echoes it back to the Client.

### tcp:
This directory contains the Client Server implemenation of the above program using c. 

### udp:
This directory also conatins a Client Server implementation. socktype used is SOCK_DGRAM.
