#include "unp.h"

int main(int argc, char **argv) {
	socklen_t len; // Used for obtaining the client addressing info
	int n, listenfd, connfd, char_in, count = 0; // Usual socket variables plus extra for reading from file
	struct sockaddr_in servaddr, cliaddr; // Declare address server for both the client and server
	// Usual read and write buffers, plus buffers to store the clients webpage request and a buffer to concat 
	// a full stop onto the start of the request so the page can be executed
	char sendbuff[MAXLINE], recvbuff[MAXLINE], cmd[16], path[64], filepath[64] = ".", vers[16];
	
	FILE * hFile; // The file pointer

	if (argc != 2) {
		err_quit("Usage: <Program Name><Port No.>\n");
	}

	listenfd = Socket(AF_INET, SOCK_STREAM, 0); // Create a listening socket

	bzero(&servaddr, sizeof(servaddr)); // Zero and fill in server address structure
	servaddr.sin_family = AF_INET; // Set ipv4
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); // Host to network the IP address
	servaddr.sin_port = htons(atoi(argv[1])); // Host to network the port number

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr)); // Connects the socket to an external (IP) interface
	
	Listen(listenfd, LISTENQ); // Changes the socket to a "Passive listening" socket

	// Infinite loop for dealing with client requests
	for ( ; ; ) {
		len = sizeof(cliaddr); // Len is the size of the clients address struct, to be used when we need to obtain client info
		
		connfd = Accept(listenfd, (SA *) &cliaddr, &len); // Accept next connection request and retrieve client addressing data

		// This statement processes and prints the clients IP address and port number. Note network to host function.
		printf("\nConnection from %s, port %d\n", Inet_ntop(AF_INET, &cliaddr.sin_addr, sendbuff, sizeof(sendbuff)), ntohs(cliaddr.sin_port));

		// Read loop
		while((n = read(connfd, recvbuff, MAXLINE)) > 0) {
        	recvbuff[n] = 0; // Null terminate

        	// Print contents of the read buffer
        	if(fputs(recvbuff, stdout) == EOF) {
                	err_sys("fputs error");
        	}

        	// Processing request happens here: If http request is found returns a pointer, else returns 0 
			if(strstr(recvbuff, "\r\n\r\n") > 0) {
				break;
			}
        }

        if (n < 0) {
        	err_sys("read error");
        }

		sscanf(recvbuff,"%s %s %s", cmd, path, vers); // Parse the incoming client request (sscanf looks for whitespace)
		strcat(filepath, path); // Concat the resource name to a full-stop to refer to 'this' directory

		// Default value that returns index, standard in all webservers
		if(strcmp(filepath, "./") == 0) {
			strcpy(filepath, "./index.html");
		}

		// Check if the requested file exists
		if((hFile = fopen (filepath, "r")) == NULL) {
				hFile = fopen("error.html", "r"); // If not: open the error file
		}

		strcpy(sendbuff, ""); // Empty the outgoing buffer

		// Read one char at-a-time from the file opened
		while((char_in = fgetc(hFile)) != EOF) {
			sendbuff[count] = char_in; // As with all servers, store each char in an outgoing buffer before sending
			count++; // Keep a count for future use
		}

		sendbuff[count] = 0; // null terminate outgoing buffer so it's ready for writing to socket

		Write(connfd, sendbuff, strlen(sendbuff)); // Write data to the client
		count = 0; // Reset the buffer index
		fclose(hFile); // Close the file
		strcpy(filepath, ".");

		Close(connfd); // Close TCP connection
	} // End infinite loop
}