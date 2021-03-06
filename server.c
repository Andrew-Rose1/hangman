// prog1_server.c
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define QLEN 6 /* size of request queue */


int main(int argc, char **argv) {
	struct protoent *ptrp;	/* pointer to a protocol table entry */
	struct sockaddr_in sad; /* structure to hold server's address */
	struct sockaddr_in cad; /* structure to hold client's address */
	int sd, sd2;			/* socket descriptors */
	int port;				/* protocol port number */
	int alen;				/* length of address */
	int optval = 1;			/* boolean value when we set socket option */
	char buf[1024];			/* buffer for string the server sends */
	uint8_t bufN;
	char buf2[10];				/* buffer for string the server receives */
	uint8_t zero = 0;
	pid_t pid;

	char *secretWord = argv[2];
	uint8_t secretSize = strlen(secretWord);

	if (argc != 3)
	{
		fprintf(stderr, "Error: Wrong number of arguments\n");
		fprintf(stderr, "usage:\n");
		fprintf(stderr, "./prog1_server server_port secret_word\n");
		exit(EXIT_FAILURE);
	}

	memset((char *)&sad, 0, sizeof(sad)); /* clear sockaddr structure */

	// Set socket family to AF_INET
	sad.sin_family = AF_INET;

	// Set local IP address to listen to all IP addresses this server can assume. You can do it by using INADDR_ANY
	sad.sin_addr.s_addr = INADDR_ANY;

	port = atoi(argv[1]); /* convert argument to binary */
	if (port > 0) {  /* test for illegal value */
		// set port number. The data type is u_short
		sad.sin_port = htons(port);
	}
	else { /* print error message and exit */
		fprintf(stderr, "Error: Bad port number %s\n", argv[1]);
		exit(EXIT_FAILURE);
	}

	/* Map TCP transport protocol name to protocol number */
	if (((long int)(ptrp = getprotobyname("tcp"))) == 0) {
		fprintf(stderr, "Error: Cannot map \"tcp\" to protocol number");
		exit(EXIT_FAILURE);
	}

	/* Create a socket with AF_INET as domain, protocol type as SOCK_STREAM, and protocol as ptrp->p_proto. This call returns a socket descriptor named sd. */
	sd = socket(AF_INET, SOCK_STREAM, ptrp->p_proto);
	if (sd < 0) {
		fprintf(stderr, "Error: Socket creation failed\n");
		exit(EXIT_FAILURE);
	}

	/* Allow reuse of port - avoid "Bind failed" issues */
	if (setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) < 0) {
		fprintf(stderr, "Error Setting socket option failed\n");
		exit(EXIT_FAILURE);
	}

	/* Bind a local address to the socket. For this, you need to pass correct parameters to the bind function. */
	if (bind(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
		fprintf(stderr, "Error: Bind failed\n");
		exit(EXIT_FAILURE);
	}

	/* Specify size of request queue. Listen take 2 parameters -- socket descriptor and QLEN, which has been set at the top of this code. */
	if (listen(sd, QLEN) < 0) {
		fprintf(stderr, "Error: Listen failed\n");
		exit(EXIT_FAILURE);
	}

	alen = sizeof(cad);
	while(1) {
		if ((sd2 = accept(sd, (struct sockaddr *)&cad, &alen)) < 0) {
			exit(EXIT_FAILURE);
		}
		printf("Connection Successful... Starting Game %d....\n", sd2);
		
		if ((pid = fork()) == 0) {
			close(sd);
			uint8_t guesses = secretSize;
			char blankWord[secretSize];
			for (int j = 0; j < secretSize; j++) {
				blankWord[j] = '_';
			}

			blankWord[secretSize] = '\0';
			char tmpBlank[secretSize];
			strncpy(tmpBlank, blankWord, sizeof(tmpBlank));
			while (1) {				
				if (guesses <= zero) {
					bufN = 0;
					send(sd2, &bufN, 1, 0);
					strncpy(buf, blankWord, sizeof(buf));
					send(sd2, buf, strlen(buf), 0);
					break;
				}
				
				// The server sends the client N as a uint8_t (Guesses)
				bufN = guesses;
				send(sd2, &bufN, 1, 0);

				// The server sends the client the board (a length-K string)
				strcpy(buf, blankWord);
				send(sd2, buf, strlen(buf), 0);
				
				// Server receives guess from client
				recv(sd2, buf2, sizeof(buf2), 0);

				// The server checks the character against the secret word
				for (int j = 0; secretWord[j] != '\0'; j++) {
					// If the character is in the secret word and hasn???t been guessed already, all in-
					// stances of the character should be added to the board in the proper location(s)
					// (replacing underscores).
					if (secretWord[j] == buf2[0]) {
						blankWord[j] = buf2[0];
					}
				}

				// If the character is not in secret word or has it already been guessed, N is
				// decremented.
				if (strcmp(tmpBlank, blankWord) == 0) {
					guesses--;
				}
				else {
					strcpy(tmpBlank, blankWord);
				}

				// If N > 0, the server sends 255 (as a uint8_t) and the board one last time, and then
				// closes the socket.
				if (strcmp(blankWord, secretWord) == 0) {
					bufN = 255;
					send(sd2, &bufN, 1, 0);
					strncpy(buf, blankWord, sizeof(buf));
					send(sd2, buf, strlen(buf), 0);
					break;
				}
			}
		}
	}
}
