#include "unp.h"
#include <string.h>
#include <stdio.h>
#define HOME_PAGE "<!DOCTYPE html> <html> <body> <h1> Hello Alex! </h1> </body> </html>"
#define ERROR_PAGE "HTTP/1.1 404 Not Found <br><br>"

int main(int argc, char **argv) {
	int listenfd, connfd, n, count = 0;
	struct sockaddr_in servaddr;
	char recvbuff[MAXLINE];
	char sendbuff[MAXLINE];

	char cmd[16];
	char path[64];
	char vers[16];

	if (argc != 2) 
		err_quit("usage: <Program Name> <Port No.>");

	/* Creation of socket is identical to client */
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	/* The servers know port :wqis bound to the socket by filling in an Internet socket address structure 
	and then calling bind */
	/* The IP address is specified as INADDR_ANY which allows to server to accept a client connection on 
	any interface. htonl is same as htons but converts hostlong */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	/* By calling listen, the socket is converted to a listening socket that incoming connections 
	from clients are accepted by the  kernel. socket, bind and listen are the three steps for setting up a 
	listening descriptor for any TCP server */
	Listen(listenfd, LISTENQ);

	/* When a handshake is complete, accept returns a value that is used as the connection descriptor. 
	This new descriptor is used for communication with the new client. A new deriptor is returned by accept 
	for each client that connects to the server. */
	for( ; ; )
	{
		connfd = Accept(listenfd, (SA *) NULL, NULL);

		if (connfd > 0){    
	         	printf("The Client is connected...\n");
      		}

		while((n = read(connfd, recvbuff, MAXLINE)) > 0) {
			recvbuff[n] = 0;

			if(fputs(recvbuff, stdout) == EOF) {
				err_sys("fputs error");
			}

			sscanf(recvbuff, "%s %s %s", cmd, path, vers); 
			
			if (strstr(recvbuff, "\r\n\r\n") > 0) {
				break;
			}			 		
		}

		strcpy(sendbuff, "");

		if (strstr(path, "/index.html")) {
			strcpy(sendbuff, HOME_PAGE);
			Write(connfd, sendbuff, sizeof(sendbuff));			
		} 
		
		getchar();
		Close(connfd);
	}
}
