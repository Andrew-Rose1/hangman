//prog1_client.c
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>


int main(int argc, char **argv) {
	struct hostent *ptrh;	/* pointer to a host table entry */
	struct protoent *ptrp;	/* pointer to a protocol table entry */
	struct sockaddr_in sad; /* structure to hold an IP address */
	int sd;					/* socket descriptor */
	int port;				/* protocol port number */
	char *host;				/* pointer to host name */
	char buf[1024];			/* buffer for board from the server */
	uint8_t bufN;			/* buffer for N from the server */
	char buf2[10];				/* buffer for data from to server */
	

	memset((char *)&sad, 0, sizeof(sad)); /* clear sockaddr structure */
	sad.sin_family = AF_INET;			  /* set family to Internet */

	if (argc != 3) {
		fprintf(stderr, "Error: Wrong number of arguments\n");
		fprintf(stderr, "usage:\n");
		fprintf(stderr, "./client server_address server_port\n");
		exit(EXIT_FAILURE);
	}

	port = atoi(argv[2]); /* convert to binary */
	if (port > 0) {	  /* test for legal value */
		sad.sin_port = htons((u_short)port);
	} else {
		fprintf(stderr, "Error: bad port number %s\n", argv[2]);
		exit(EXIT_FAILURE);
	}

	host = argv[1]; /* if host argument specified */

	/* Convert host name to equivalent IP address and copy to sad. */
	ptrh = gethostbyname(host);
	if (ptrh == NULL) {
		fprintf(stderr, "Error: Invalid host: %s\n", host);
		exit(EXIT_FAILURE);
	}

	memcpy(&sad.sin_addr, ptrh->h_addr, ptrh->h_length);

	/* Map TCP transport protocol name to protocol number. */
	if (((long int)(ptrp = getprotobyname("tcp"))) == 0) {
		fprintf(stderr, "Error: Cannot map \"tcp\" to protocol number");
		exit(EXIT_FAILURE);
	}

	/* Create a socket. */
	sd = socket(PF_INET, SOCK_STREAM, ptrp->p_proto);
	if (sd < 0) {
		fprintf(stderr, "Error: Socket creation failed\n");
		exit(EXIT_FAILURE);
	}

	/* TODO: Connect the socket to the specified server. You have to pass correct parameters to the connect function.*/
	if (connect(sd, (struct sockaddr *)&sad, sizeof(sad)) < 0) {
		fprintf(stderr, "connect failed\n");
		exit(EXIT_FAILURE);
	}

	/* Repeatedly read data from socket and write to user's screen. */
	while (1) {
		// Receive N
		recv(sd, &bufN, sizeof(bufN), 0);
		recv(sd, buf, sizeof(buf), 0);

		if (bufN == 255) {
			printf("Board: %s\n", buf);
			printf("You Win!\n");
			break;
		}
		if (bufN == 0) {
			printf("Board: %s\n", buf);
			printf("You Lost!\n");
			break;
		}

		/* The client displays the board and number of guesses remaining */
		printf("Board: %s (%d guesses left)\n", buf, bufN);
		
		/* The client prompts the user for a guess */
		printf("Enter a guess: \n");
		fgets(buf2, sizeof(buf2), stdin);

		/* The client sends the guess as a char to the server */
		send(sd, buf2, strlen(buf2), 0);
		
	}
	close(sd);

	exit(EXIT_SUCCESS);
}
