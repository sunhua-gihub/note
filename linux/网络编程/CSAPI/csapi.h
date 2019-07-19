/*************************************************
* Author: 	AutoCatFuuuu
* Date:		Fri Jul 19 14:44:04 PDT 2019
*************************************************/
#ifndef CSAPI_H
#define CSAPI_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define LISTENQ 5
/*************************************************
* 创建客户端
* hostname:	主机
* port:		端口号
*************************************************/
int CreateClient(char *hostname,char *port);

/*************************************************
* 创建服务端
* port:		端口号
*************************************************/
int CreateListen(char *port);

#endif
