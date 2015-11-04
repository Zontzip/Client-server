#include "unp.h"
#include <time.h>

int main(int argc, char **argv) {
	int 		listenfd, connfd;
	struct sockaddr_in servaddr;
	char		recvline[MAXLINE];
	char 		sendline[MAXLINE];

	if (argc != 2) 
		err_quit("usage: <Program Name> <Port No.>");

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);
	
	
	for( ; ; )
	{
		connfd = Accept(listenfd, (SA *) NULL, NULL);
		
		while ((n = read(connfd, recvline, MAXLINE)) > 0) {
			recvline = 0;
			if(fputs(recvline, stdout) {
				
		

		snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
		Write(connfd, buff, strlen(sendline));
		
		getchar();

		Close(connfd);
	}
}
