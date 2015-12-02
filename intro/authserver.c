/**
 * Name: Alex Kiernan
 *
 * Student No: C13451458
*/

#include "unp.h"
#include <string.h>
#include <stdio.h>

int main(int argc, char **argv) {
	socklen_t len;
	int listenfd, connfd, n;
	struct sockaddr_in servaddr, cliaddr;
	char uname[50];
	char pswrd[50];
	char temp1[50];
	char temp2[50];
	char recvbuff[MAXLINE];
	char sendbuff[MAXLINE];
	char ipadr[32];
	char portno[16];

	if (argc != 2)
		err_quit("usage: ./authenclient <IPaddress> <Port Number>");

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	for( ; ; )
	{
		len = sizeof(cliaddr);
		
		connfd = Accept(listenfd, (SA *) NULL, NULL);

		printf("\nConnection from %s, port %d\n", Inet_ntop(AF_INET, &cliaddr.sin_addr, sendbuff, sizeof(sendbuff)), ntohs(cliaddr.sin_port));

		strcpy(ipadr, Inet_ntop(AF_INET, &cliaddr.sin_addr, sendbuff, sizeof(sendbuff)));
		strcpy(portno, ntohs(cliaddr.sin_port));

		if (connfd > 0){    
	         printf("The Client is connected...\n");
    	}	

		while((n = read(connfd, recvbuff, MAXLINE)) > 0) {
        	recvbuff[n] = 0; // Null terminate

        	if(fputs(recvbuff, stdout) == EOF) {
                	err_sys("fputs error");
        	}

        	sscanf(recvbuff,"%s %s %s %s", temp1, uname, temp2, pswrd);

        	if (strstr(recvbuff, "\r\n\r\n") > 0) {
				break;
			}	
        }

    	strcpy(sendbuff, "");

    	if (strcmp(uname, "admin") == 0 && strcmp(pswrd, "pwd") == 0) {
    		strcpy(sendbuff, "PROCEED");
    		strcat(sendbuff, ipadr);
    		strcat(sendbuff, portno);
    		printf("Success");
    	} else {
    		strcpy(sendbuff, "DENIED" );
    		strcat(sendbuff, ipadr);
    		strcat(sendbuff, portno);
    		printf("Failure");
    	}

    	strcpy(ipadr, argv[1]);
    	printf("\nIp address: %s", ipadr);

		Write(connfd, sendbuff, sizeof(sendbuff));	

		Close(connfd);
	}
}
