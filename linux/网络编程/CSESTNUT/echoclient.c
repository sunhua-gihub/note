#include <stdio.h>
#include "rioapi.h"
#include "csapi.h"

#define MAXLINE 512
int main(int argc,char **argv)
{
	int fd;
	char *host, *port, buf[MAXLINE];
	rio_t rio;
	
	if(argc != 3) {
		fprintf(stderr, "usage: %s<host><port> \n",argv[0]);
		exit(0);
	}
	
	host = argv[1];
	port = argv[2];
	
	fd = CreateClient(host,port);
	rio_readinitb(&rio,fd);
	
	while(fgets(buf, MAXLINE,stdin) != NULL) {
		rio_writen(fd,buf,strlen(buf));
		rio_readlineb(&rio,buf,MAXLINE);
		fputs(buf,stdout);
	}
	close(fd);
	exit(0);
}
