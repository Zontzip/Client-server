#include "unp.h"

int main(int argc, char **argv) {
	socklen_t len;
	int n, listenfd, connfd, char_in, count = 0; // Usual socket variables plus extra for reading from file
	struct sockaddr_in servaddr, cliaddr; // Declare address server for both the client and server
	char sendbuff[MAXLINE], recvbuff[MAXLINE], cmd[16], path[64], filepath[64] = ".", vers[16];
	
	FILE * hFile;

	if (argc != 2) {
		err_quit("Usage: <Program Name><Port No.>\n");
	}


	// Create a listening socket
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr)); // Zero and fill in server address structure
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Connect to any local ip address
	servaddr.sin_port = htons(atoi(argv[1])); // Daytime server port number

	// Connects the socket to an external interface
	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	// Changes the socket to a " Passive listening" socket
	Listen(listenfd, LISTENQ);


	for ( ; ; ) {
		len = sizeof(cliaddr);
		
		connfd = Accept(listenfd, (SA *) NULL, NULL); // Accept next connection request

		printf("\nConnection from %s, port %d\n", Inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));

		while((n = read(connfd, recvbuff, MAXLINE)) > 0) {
        	recvbuff[n] = 0; // Null terminate

        	if(fputs(recvbuff, stdout) == EOF) {
                	err_sys("fputs error");
        	}

			if(strstr(recvbuff, "\r\n\r\n") > 0) {
				break;
			}
        }

        if (n < 0) {
        	err_sys("read error");
        }

		sscanf(recvbuff,"%s %s %s", cmd, path, vers); // Parse the incoming client request
		strcat(filepath, path); // Concat the resource name to a full-stop to refer to 'this' directory
		printf("Filepath: %s\n", filepath);

		if(strcmp(filepath, "./") == 0) {
			strcpy(filepath, "./index.html");
		}

		// Check if the requested file exists
		if((fd = fopen (filepath, "r")) == NULL) {
				printf("error 404\n"); // Open the error file
		}

		strcpy(sendbuff, ""); // Empty the outgoing buffer

		while((char_in = fgetc(hFile)) != EOF) {
			sendbuff[count] = char_in;
			count++;
		}

		sendbuff[count] = 0; // null terminate outgoing buffer ready for writing to socket

		Write(connfd, sendbuff, strlen(sendbuff)); // Write data to the client
		count = 0; // Reset the buffer index
		fclose(hFile); // Close the file
		strcpy(filepath, ".");

		Close(connfd); // Close TCP connection
	} // End infinite loop
}