#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./hexdump.c"
#include "./udpUtil.c"
#include "./rfc11.c"

#define MAXLINE 1024 /*max text line length as per the RFC 1.1*/
#define DEFAULT_SERV_PORT 10010 /*port as per the PDF*/
#define LISTENQ 8 /*maximum number of client connections*/


int compareByteArray(char *lhs, char *rhs, int length) {
	int i=0;
	for (i = 0; i < length; i++) {
		char l = *(lhs + i);
		char r = *(rhs + i);
		int comparison = l ^ r;
		if (comparison != 0)
			return -1;
	}
	return 0;
}

int main (int args, char **argv) {
	char sendline[MAXLINE];
	char receiveBuffer[MAXLINE + 14];
	char** sendBuffer = (char**) malloc(sizeof(char*));
	struct sockaddr_storage receiveAddr;
	socklen_t 	receiveAddrLen;
	//struct timeval tv,t0,t1;
	if (args < 2) {
		fprintf(stderr, "Please pass the server hostname as a commandline argument!\n");
		return 1;
	}
	char *serverHostname = argv[1];

	// port number is optional; if not passed, assume default
	int serverPort = 0;
	if (args > 2) {
		serverPort = (int) strtol(argv[1], NULL, 10);
	}
	if (serverPort == 0 ||  serverPort > 65535) // user passed an invalid number or no port num
		serverPort = DEFAULT_SERV_PORT;


	struct addrinfo * svrInfo = u_getServerInfo(serverHostname, serverPort);
	struct addrinfo ** remoteAddrPtr = (struct addrinfo **) malloc(sizeof(struct sockaddr*));
	int socket = u_clientSocketUdp(svrInfo, remoteAddrPtr);

	printf("Please enter a message to send (up to %d bytes):\n", MAXLINE - 1);
	// read one line from user
	fgets(sendline, MAXLINE, stdin);
	// build the message bytes with header to send
	// buildMessage is in rfc11.c
	int len = buildMessage(sendline, 15, sendBuffer);
	int bytesSent = sendto(socket, *sendBuffer, len, 0,
			(*remoteAddrPtr)->ai_addr, (*remoteAddrPtr)->ai_addrlen);
	//gettimeofday(&t0,NULL);
	long long int sendTime = now();
	int bytesReceived = recvfrom(socket, receiveBuffer, MAXLINE, 0,
			(struct sockaddr *)&receiveAddr, &receiveAddrLen);
	//gettimeofday(&t1,NULL);
	long long int recieveTime = now();
	int elapsed = recieveTime - sendTime;
	printf ("Round Trip Time : %lld milliseconds \n", &elapsed);
	// hexdump.c contains this.  I just got it from stackoverflow for debug purposes.
	hexDump("Sent Network msg", *sendBuffer, len);
	hexDump("Received from Server msg", receiveBuffer, bytesReceived);

	if (bytesSent != bytesReceived || (compareByteArray(*sendBuffer, receiveBuffer, bytesReceived) != 0)) {
		fprintf(stderr, "Data received from server is different from the data sent!");
	}
	return 0;
}
