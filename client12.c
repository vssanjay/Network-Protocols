#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>


#define SENDLINE 10 /*max text line length*/
#define RECVLINE 16 /*max text line length*/
#define SERV_PORT 10010 /*port*/

int main(int argc, char **argv)
{

 int sockfd;
 struct sockaddr_in servaddr;
 char recvline[RECVLINE], message[SENDLINE];
    char *end;
    unsigned long message_length, b;
	
 //basic check of the arguments
  if (argc !=5) {
  printf("Usage: Exec arg1 arg2 op server");
  exit(1);
 }
    if(strlen(argv[1]) > 8)
    {
        printf("Usage: Length of argument 1 cannot be greater than 5 bytes. \n");
        exit(2);
    }
    
    if(strlen(argv[2]) > 8)
    {
        printf("Usage: Length of argument 2 cannot be greater than 5 bytes. \n");
        exit(3);
    }
    
    if(strlen(argv[3]) > 1)
    {
        printf("Usage: Length of argument 2 cannot be greater than 1 bytes. \n");
        exit(4);
    }
    
    
    if((argv[3][0] != '+') && (argv[3][0] != '-') && (argv[3][0] != '*') && (argv[3][0] != '/'))
       {
           printf("Usage: Wrong Operator. \n");
           exit(5);
       }
       
       if((argv[4][0] != '/') && ( (b = strtoul(argv[2], &end, 10) )== 0 ))
       {
           printf("Usage: Divisor cannot be 0. \n");
           exit(6);
       }
    
    strcat(message, argv[1]);
    strcat(message, " ");
    strcat(message, argv[3]);
    strcat(message, " ");
    strcat(message, argv[2]);
    message_length = strlen(message);
    
    if(message_length > 10)
    {
        printf("Usage: Length of entire message cannot be greater than 10 bytes. \n");
        exit(7);
    }
    
    printf("%lu \n",message_length);

    
 //Create a socket for the client
 if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) == -1) {
  perror("Problem in creating the client socket; ");
  exit(8);
 }
  
 //Creation of the socket
 memset(&servaddr, 0, sizeof(servaddr));
 servaddr.sin_family = AF_INET;
 servaddr.sin_addr.s_addr= inet_addr(argv[4]);
 servaddr.sin_port =  htons(SERV_PORT); //convert to big-endian order
	
 //Connection of the client to the socket 
 if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
  perror("Problem in connecting to the server.");
  exit(9);
 }
	 
  if(send(sockfd, message, SENDLINE, 0) < 0)
       {
           perror("Client Send: ");
           exit(10);
   
       }
       
       printf("%s", "String sent to the server: ");
       fputs(message, stdout);
    printf("\n");
		
  if (recv(sockfd, recvline, RECVLINE, 0) < 0){
     perror("Client Receive: ");
   exit(11);
  }

       printf("%s", "String received from the server: ");
       fputs(recvline, stdout);
    printf("\n");

 exit(0);
}
