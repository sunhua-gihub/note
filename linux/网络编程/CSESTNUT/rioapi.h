#ifndef RIPAPI_H
#define RIPAPI_H

//#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define RIO_BUFSIZE 8192

typedef struct {
	int rio_fd;					//文件描述符
	int rio_cnt;				//
	char *rio_bufptr;			//
	char rio_buf[RIO_BUFSIZE];	//缓存
}rio_t;

/***************************************************
* 读文件
* fd: 文件描述符
* *usrbuf: 读取内容保存区
* n: 读取大小
***************************************************/
ssize_t rio_readn(int fd, void *usrbuf, size_t n);

/***************************************************
* 写文件
* fd: 文件描述符
* *usrbuf: 写入内容保存区
* n: 写入大小
***************************************************/
ssize_t rio_writen(int fd, void *usrbuf, size_t n);

/***************************************************
* 初始化rio_t结构体
* *rp: 结构体
* fd: 文件描述符
***************************************************/
void rio_readinitb(rio_t *rp, int fd);

/***************************************************
* 带缓冲区读文件
* fd: 文件描述符
* *usrbuf: 读取内容保存区
* n: 读取大小
***************************************************/
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n);

/***************************************************
* 带缓冲区读取一行内容
* rp: 结构体
* *usrbuf: 读取内容保存区
* maxlen: 读取大小
***************************************************/
ssize_t rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);

/***************************************************
* 带缓冲区读取n个字节内容
* rp: 结构体
* *usrbuf: 读取内容保存区
* maxlen: 读取大小
***************************************************/
ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n);

#endif
