#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define SENDLINE 16 /*max text line length*/
#define RECVLINE 10 /*max text line length*/
#define SERV_PORT 10010 /*port*/
#define LISTENQ 8 /*maximum number of client connections */

int main (int argc, char **argv)
{
    int listenfd, connfd;
    int n, counter;
    socklen_t clilen;
    char buf[RECVLINE], *end;
    struct sockaddr_in cliaddr, servaddr;
    char *arg1, *arg2, *op, result[7], final_message[SENDLINE];
    unsigned long a, b, c;
	
 /*creation of the socket
 if((listenfd = socket (AF_INET, SOCK_STREAM, 0) == -1))
 {
     perror("Problem in creating the server socket: ");
     exit(1);

 }*/
    
    
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    
 //preparation of the socket address
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(SERV_PORT);


    bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	
 /*if(bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1)
 {
     perror("Bind: ");
     exit(2);
 }
  
*/
	
    listen(listenfd, LISTENQ);
 /*{
     perror("Listen: ");
     exit(3);
 }*/
	
 printf("%s\n","Server running...waiting for connections.");
	
    for ( ; ; ) {
		
        clilen = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen);
  /*{
      perror("Accept: ");
      exit(4);
  }
   */
        printf("%s\n","Received request...");
        n = recv(connfd, buf, RECVLINE,0);
     /*if (n < 0) {
         perror("Recv: Server: ");
         exit(5)    ;
     }*/
        printf("%s","String received from the client:");
        puts(buf);
     
     // Preserve the message for the Send
     
        strcpy(final_message, buf);
       // printf("Final Message: %s\n", final_message);
     
        arg1 = strtok(buf, " ");
     //printf("1. %s \n", arg1);
        op = strtok(NULL, " ");
     //printf("2. %s \n", op);
        arg2 = strtok(NULL, " ");
     //printf("3. %s \n", arg2);
     
        a = strtoul(arg1, &end, 10);
        printf("%lu \n", a);
     
        b = strtoul(arg2, &end, 10);
        printf("%ld \n", b);
     
        //printf("%c \n", arg2[0]);
     
        switch(op[0])
            {
                case '+': c = a + b;
                            break;
                case '-': c = a - b;
                        break;
                case '*': c = a * b;
                        break;
                case '/': c = a / b;
                        break;
            }
    
        printf("4 . The result is : %lu . \n", c);
     
        sprintf(result, "%lu", c);
     
     //printf("The result is : %s . \n", result);
     
        strcat(final_message, result);
     
        n = strlen(final_message);

        printf("%s  - %d. \n", final_message, n);
     
        if(n > 16)
        {
            printf(" Message could not be sent. More than 16 bytes in length. \n");
            exit(10);
        }
     
        send(connfd, result, SENDLINE, 0);
     /*{
         perror("Server Send: ");
         exit(11);
     }*/
     
        printf("%s", "String sent to the client: ");
        fputs(buf, stdout);

        close(connfd);
	
    }
    
    //close listening socket
    close(listenfd);
    
    
}
