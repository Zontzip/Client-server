#include "unp.h"

int main (int argc, char **argv) {

	int sockfd, n; /* Define our socket file descriptor */
	char recvline[MAXLINE + 1];
	struct sockaddr_in servaddr;

	if (argc != 3)
		err_quit("usage: a.out <IPaddress>");

	/* Socket function creates an Internet stream and a TCP socket, the function returns a small int 
	file descriptor that can be used to identify  the socket in all future function calls, such as connect. */
	/* The IF state contains a call to the socket function, 
	then assigns the return value to sockfd and then tests the value */
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		err_sys("socket error");

	/* In the next few lines, we fill in an Internet socket address structure (sockaddr_in structure named seraddr) 
	with the servers IP address and port number. We set the entire structure to 0 using bzero, then set the address family to AF_INET, 
	set the port number to the second argument passed in and finally set the IP address to the first argument passed. 
	The IP address and port number must be in specific formats. htons ("host to network short") converts the binary port number and
	inet_pton ("presentation to numerid") converts the ASCII IP address into the proper format */
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2])); /* daytime server port taken from the command line */
	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		/* wrapper function checks for error */
		err_quit("inet_pton error for %s", argv[1]);

	/* The connect function, when applied to a TCP socket, establishes a TCP connection with the server specified by the socket address structure 
	as our second argument. We must also specify the length of the socket address structure as our third argument. 
	In unp.h, SA is defined as a generic socket address structure that must be cast as a pointer because socket functions predate ANSI C standard. 
	 */
	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	/* We read the servers reply and display the result using fputs. Read needs to be code in a loop as not all the data may be sent in a single segment, 
	there might be 1 single segment for 26 bytes or 26 segments of 1 byte. We then terminate read when it returns a 0 signalling there is nothing more to read. */
	while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
		recvline[n] = 0; /* null terminate */
		if (fputs(recvline, stdout) == EOF) /* print data (time and date in this case) returned from server */
			err_sys("fputs error");
	}

	if (n < 0)
		err_sys("read error");

	printf("counter = %d\n", counter);

	/* Exit the program and close all open descriptors */ 
	exit(0);
}
