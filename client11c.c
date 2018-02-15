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
	int stringGen = 0;
	int temp=0, counter=0;
	int sendInteger =0, recieveInteger=0;
	char buffer[33];
	char sendline[MAXLINE];
	char receiveBuffer[MAXLINE + 14];
	char** sendBuffer = (char**) malloc(sizeof(char*));
	struct sockaddr_storage receiveAddr;
	socklen_t 	receiveAddrLen;
	long long int fastElapsed=0;
	long long int newElapsed =0;
	long double elapsed;
	long long int avgElapsed;
	long long int sendTime;
	long double recieveTime;
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

	printf("\n Sending 1-10,000 strings to the server\n");

		struct addrinfo * svrInfo = u_getServerInfo(serverHostname, serverPort);
		struct addrinfo ** remoteAddrPtr = (struct addrinfo **) malloc(sizeof(struct sockaddr*));
		int socket = u_clientSocketUdp(svrInfo, remoteAddrPtr);

	for (stringGen=0;stringGen<10000;stringGen++)
	{
		
		temp=temp+1;
		snprintf(buffer,33,"%d",temp);
		int len = buildMessage(buffer, 15, sendBuffer);
		sendto(socket, buffer, len, 0,(*remoteAddrPtr)->ai_addr, (*remoteAddrPtr)->ai_addrlen);
		sendTime = now();

		hexDump("Sent Network msg", *sendBuffer, len);
		int bytesReceived=recvfrom(socket, receiveBuffer, MAXLINE, 0,(struct sockaddr *)&receiveAddr, &receiveAddrLen);
		recieveTime = now();
		hexDump("Received from Server msg", receiveBuffer, bytesReceived);
		elapsed = recieveTime;
		sendInteger = atoi(buffer);
		recieveInteger =atoi(receiveBuffer);
		if (sendInteger!=recieveInteger)
		{	
			counter = counter +1;
		}
	}
	avgElapsed = (elapsed + newElapsed)/1000000000000;
	printf("The number of missing Echoes are %d \n",counter);
	printf("\n ------Statistics------\n");
	printf("\n The fastest Round Trip Time is : %lld ms ", fastElapsed );
	printf("\n The Round Trip Time is : %ld ms ", recieveTime);
	printf("\n The average elapsed Round Trip Time is : %lld\n", avgElapsed);

	return 0;
}