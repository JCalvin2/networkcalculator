# networkcalculator

This code acts as a calculator over a network. 
The client connects to the server, sends the simple math equation it wants the server to computer (+,-,*,/), and the server sends the answer.

Client
gcc client.c -o client
./client IP address --> ie. ./client 127.0.0.1

Server
gcc server.c -o server
./server
