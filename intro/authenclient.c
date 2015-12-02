#include "unp.h"

int main(int argc, char **argv)
{
	int sockfd, n;
	char sendbuff[MAXLINE + 1], recvbuff[MAXLINE + 1];
	struct sockaddr_in servaddr;

	if (argc != 5)
		err_quit("usage: ./authenclient <IPaddress> <Port Number> <Username> <Password>");

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	snprintf(sendbuff, sizeof(sendbuff), "username: %s password: %s\r\n\r\n", argv[3], argv[4]);

	Write(sockfd, sendbuff, strlen(sendbuff)); //write contents of sendbuff to socket

	while ( (n = read(sockfd, recvbuff, MAXLINE)) > 0)
	{
		recvbuff[n] = 0;

		if (fputs(recvbuff, stdout) == EOF)
			err_sys("fputs error");

		if(strstr(recvbuff, "\r\n") > 0)
			break;
	}
	
	if (n < 0)
		err_sys("read error");

	exit(0);
}