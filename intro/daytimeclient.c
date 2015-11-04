#include "unp.h"

// TCP requires address and port
// pass in stuff from command line, here we pass in 3 args: "./daytimeclient", "127.0.0.1" and "1234"
int main (int argc, char **argv) {
	int sockfd, n, counter = 0;
	char recvline(MAXLINE + 1);
	struct sockaddr_in servaddr;  

	if (argc != 3) 
		err_quit("usage: a.out <IPaddress>");

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr)); 
	servaddr.sin_family = AF_INET; 
	servaddr.sin_port = htons(atoi(argv[2])); 
	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) 
		err_quit("inet_pton error for %s", argv[1]);

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0) 
		err_sys("connect error");

	while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
		counter++; /* count the number of loops completed */
		recvline[n] = 0; /* null terminate, sets line 27 to null so it can be printed as string */
		if (fputs(recvline, stdout) == EOF) /* print data (time and date) returned from server */
			err_sys("fputs error");
	}

	if (n < 0)
		err_sys("read error")

	printf("counter = %d\n", counter);

	exit(0);
}
