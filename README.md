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
   
    ./server {HOST_PORT} {SECRET_WORD}
    
Client format
   
    ./client {HOST_ADDRESS} {HOST_PORT} 


### Example Game

The player will guess a letter each round. The player's remaining guess will be decreased if the letter does not appear in the secret wor, or if the letter has already been guessed. The game is over if the player has guessed all letters, or there are no reamining guesses.

The game can support multiple games at once. The server will continue to listen for connections until it is manually terminated.

This example game assumes that you are hosting the server and clients on the same machine.

Start the server

    ./server 3987 binary
    
Start the client

    ./client localhost 3987
    
The client output is on the left, while the server output is on the right.
<img width="1335" alt="Screen Shot 2022-05-21 at 9 31 11 PM" src="https://user-images.githubusercontent.com/55816533/169678749-43c0abcb-90a0-4d37-9964-80171afface7.png">
<img width="1337" alt="Screen Shot 2022-05-21 at 9 31 31 PM" src="https://user-images.githubusercontent.com/55816533/169678752-1d07a746-a261-4db8-8f44-415dc4d2f31f.png">
<img width="1327" alt="Screen Shot 2022-05-21 at 9 32 17 PM" src="https://user-images.githubusercontent.com/55816533/169678756-c933e1af-6bf3-4257-84b4-6da78031bd55.png">

