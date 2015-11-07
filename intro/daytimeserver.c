#include "unp.h"
#include <time.h>

int main(int argc, char **argv) {
	int listenfd, connfd;
	struct sockaddr_in servaddr;
	char buff[MAXLINE];
	time_t ticks;

	if (argc != 2) 
		err_quit("usage: <Program Name> <Port No.>");

	/* Creation of socket is identical to client */
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	/* The servers know port is bound to the socket by filling in an Internet socket address structure 
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
	/* time returns the current date and time (okeanos is 2 hours ahread) in the form of seconds since jan 1, 1970, 
	ctime converts this int to human-readable string. A carriage return and linefeed are then appended to the string 
	by snprintf and result is written to the client by write. snprintf also takes the buffer size to prevent overflow */
	for( ; ; )
	{
		connfd = Accept(listenfd, (SA *) NULL, NULL);

		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		Write(connfd, buff, strlen(buff));
		
		getchar();

		/* Server closes connection with client. Initiates normal TCP connection sequence where a 
		FIN is sent in each direction */
		Close(connfd);
	}
}
