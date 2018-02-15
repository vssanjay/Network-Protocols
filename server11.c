#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./udpUtil.c"
#include "./hexdump.c"
#include "./rfc11.c"

#define MAXLINE 1024 /*max text line length as per the RFC 1.1*/
#define DEFAULT_SERV_PORT 10010 /*port as per the PDF*/
#define LISTENQ 8 /*maximum number of client connections*/


int main (int args, char **argv) {

	char sendline[MAXLINE];
	char** sendBuffer = (char**) malloc(sizeof(char*));

	// convert the first argument to an integer.  argv[0] is the path of the binary run
	// a port number is just 16-bits, unsigned
	int listenPort = 0;
	if (args > 1)
		listenPort = (int) strtol(argv[1], NULL, 10);
	if (listenPort < 1024 || listenPort > 65535) {
		// using fprintf to write to stderr so that any piping of stdout doesn't mess up the flow
		fprintf(stderr, "A valid listen port specified as command-line argument must be 1024-65535!  Using default value of %i .\n", listenPort = DEFAULT_SERV_PORT);
	}

	int listenSocket = u_bindUdp(u_getServerInfo(NULL, listenPort));
	struct sockaddr_storage remoteAddr;
	socklen_t addrLen; // ip v4 or v6 have different lengths
	int clientSocket;

	while (1) {
		// receive ping
		int rcv_bytes = recvfrom(listenSocket, sendline, MAXLINE + 14, 0,
				(struct sockaddr *)&remoteAddr, &addrLen);
		hexDump("Received", sendline, rcv_bytes);

		// pong it back
		sendto(listenSocket, sendline, rcv_bytes, 0,
			(struct sockaddr *)&remoteAddr, addrLen);
	}

	close(listenSocket);

	return 0;
}

