#include "csapi.h"

int CreateClient(char *hostname,char *port)
{
	int fd;
	struct addrinfo hints, *listp, *p;
	
	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_NUMERICSERV;
	hints.ai_flags |= AI_ADDRCONFIG;
	
	getaddrinfo(hostname,port,&hints, &listp);
	
	for(p=listp;p;p=p->ai_next)
	{
		if((fd = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) <0)
			continue;
		if(connect(fd,p->ai_addr,p->ai_addrlen) != -1)
			break;
		close(fd);
	}
	
	freeaddrinfo(listp);
	if(!p)
		return -1;
	else
		return fd;
}

int CreateListen(char *port)
{
	struct addrinfo hints,*listp,*p;
	int fd,optval=1;
	
	memset(&hints,0,sizeof(struct addrinfo));
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE | AI_ADDRCONFIG;	//服务器监听和相同协议族
	hints.ai_flags |= AI_NUMERICSERV;				//设置为端口号
	getaddrinfo(NULL,port,&hints,&listp);
	
	for(p=listp;p;p=p->ai_next)
	{
		if((fd = socket(p->ai_family,p->ai_socktype,p->ai_protocol)) < 0)
			continue;
		
		setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,(const void *)&optval,sizeof(int));
		
		if(bind(fd,p->ai_addr,p->ai_addrlen) == 0)
			break;
		close(fd);
	}
	
	freeaddrinfo(listp);
	
	if(!p)
		return -1;
	
	if(listen(fd,LISTENQ) < 0)
	{
		close(fd);
		return -1;
	}
	
	return fd;
}


















