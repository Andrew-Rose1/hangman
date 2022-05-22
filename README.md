# Hangman Game
A hangman game between the player and a computer via a client and server connection. This project utilizes the c socket library and can support multiple games at once.

## Getting Started

By following these steps you should be able to get a server and client connected to play a hangman game.


### Prerequisites
- GNU Compiler Collection


### Installing

Compile both server and client c files

    gcc -o server server.c
    gcc -o client client.c


## Playing the game

To run each program you need to pass in the correct arguements to avoid errors.

Server format
   
    ./server {HOST_ADDRESS} {HOST_PORT} {SECRET_WORD}
    
Client format
   
    ./client {HOST_ADDRESS} {HOST_PORT} 


### Example Game

This example game assumes that you are hosting the server and clients on the same machine.

Start the server

    ./server localhost 3987 8 30 dict.txt
    
Start the client

    ./client localhost 3987
