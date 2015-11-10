#include "unp.h"

int main(int argc, char **argv) {

	int n, listenfd, connfd, char_in, count = 0; // Socket Ids; one for listening and one for the connected socket.
	struct sockaddr_in servaddr, cliaddr; // Address structure to hold this server's address.
	char sendbuff[MAXLINE], recvbuff[MAXLINE], cmd[16], path[64], filepath[64] = ".", vers[16];
	
	FILE *fd;

	if (argc != 2) {
		err_quit("Usage: <Program Name><Port No.>\n");
	}


	// Create a socket.
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr)); // Zero and fill in server address structure.
	servaddr.sin_family 		= AF_INET;
	servaddr.sin_addr.s_addr 	= htonl(INADDR_ANY); // Connect to any local ip address.
	servaddr.sin_port 		= htons(atoi(argv[1])); // Daytime server port number;

	// Connects the socket to an external interface.
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	// Changes the socket to a " Passive listening" socket
	Listen(listenfd, LISTENQ);


	for ( ; ; ) {
		// Accept next connection request.
		connfd = Accept(listenfd, (SA *) NULL, NULL);

		while((n = read(connfd, recvbuff, MAXLINE)) > 0) {
        	recvbuff[n] = 0; // Null terminate.

        	if(fputs(recvbuff, stdout) == EOF) {
                	err_sys("fputs error");
        	}

			if(strstr(recvbuff, "\r\n\r\n") > 0) {
				break;
			}
        }

	// GET /index.html HTTP/1.1
	sscanf(recvbuff,"%s %s %s", cmd, path, vers);
	strcat(filepath, path);
	printf("Filepath: %s\n", filepath);

	if(strcmp(filepath, "./") == 0) 
		strcpy(filepath, "./index.html");

	if((fd = fopen (filepath, "r")) == NULL) {
			// fopen(error.html file ...);
			printf("error 404\n");
	}
	else {
		int c;

		while ((c = fgetc(fd)) != EOF) {
			sendbuff[count] = c;
			count++;
		}
		sendbuff[count] = 0;
	}


	// Write data to the client.
	Write(connfd, sendbuff, strlen(sendbuff));

	Close(connfd);
	fclose(fd);
            count = 0;
	strcpy(filepath, ".");
	}
}
// Note the use of the upper-case letters at the start of the socket primitives names.
// These are wrapper function that include error-checking functionality for each call.
// The actual socket primitives use lower case names.