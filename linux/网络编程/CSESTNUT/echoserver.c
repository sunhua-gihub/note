#include <stdio.h>
#include "rioapi.h"
#include "csapi.h"

#define MAXLINE 512

void echo(int fd)
{
	size_t n;
	char buf[MAXLINE];
	rio_t rio;
	
	rio_readinitb(&rio,fd);
	while((n = rio_readlineb(&rio,buf,MAXLINE)) != 0) {
		printf("\nserver received %d bytes: %s",(int)n,buf);
		rio_writen(fd,buf,n);
	}
}

int main(int argc,char **argv)
{
	int listenfd,connfd;
	socklen_t clientlen;
	struct sockaddr clientaddr;
	char client_hostname[MAXLINE],client_port[MAXLINE];
	
	if(argc != 2) {
		fprintf(stderr,"usage: %s <port>\n",argv[0]);
		exit(0);
	}
	
	listenfd = CreateListen(argv[1]);
	printf("server is start!!\n!");
	while(1) {
		clientlen = sizeof(struct sockaddr);
		connfd = accept(listenfd,&clientaddr,&clientlen);
		getnameinfo(&clientaddr,clientlen,client_hostname,MAXLINE,
				client_port,MAXLINE,0);
		printf("Connected to (%s, %s)\n",client_hostname,client_port);
		echo(connfd);
		close(connfd);
	}
	exit(0);
}
