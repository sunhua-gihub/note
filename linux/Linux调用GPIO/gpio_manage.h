/************************************************************************
*	Author 	: AutoCatFuuuu
*	Date	: Mon Jul 15 16:58:00 UTC 2019
*	Chestnut.1: 
*		1. HisiGpioDirCfg(17,5,1);		设置方向
*		2. HisiGpioSetValue(17,5,1);	打开
*		3. HisiGpioSetValue(17,5,0);	关闭
*	Chestnut.2: 
*		1. s32GPIOflagMutexInit();		初始化标记锁
*		2. RegisterCallBackFunc();		注册回调函数 *可以不注册会调用默认回调函数
*		3. Sets32AlarmTimeValue(3);		设置报警时长 *可以不设置，默认3秒
*		3. CreateGPIODealThread();		创建GPIO线程
*		4. Sets32GPIOflagValue(1);		设置报警  
************************************************************************/
#ifndef GPIO_MANAGE_H
#define GPIO_MANAGE_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

//#include "comm_struct.h"

#define REG_SPACE_SIZE 0x1000
#define GPIO_DATA_ADDR(gpioGrpBaseAddr, pin)        ((gpioGrpBaseAddr) + 0x000) + (1 << ((pin) + 2))
#define GPIO_GROUP_DIR_ADDR(gpioGrpBaseAddr)        (gpioGrpBaseAddr) + 0x400

typedef void* (*GPIOCallBackFuncHandle)(void *argv);

/***************************************************
* GPIO地址
****************************************************/
#if defined(_USE_3559)
static int HiGpioBaseAddrTab[19]={
					0x12140000,0x12141000,0x12142000,0x12143000,0x12144000,/**GPIO0-4*/
					0x12145000,0x12146000,0x12147000,0x12148000,0x12149000,/**GPIO5-9*/
					0x1214A000,0x1214B000,0x1214C000,0x1214D000,0x1214E000,/**GPIO10-14*/
					0x1214F000,0x12150000,0x12151000,0x12152000/**GPIO15-18*/
					};
#else
static int HiGpioBaseAddrTab[19]={
					0x12150000,0x12160000,0x12170000,0x12180000,0x12190000,/**GPIO0-4*/
					0x121A0000,0x121B0000,0x121C0000,0x121D0000,0x121E0000,/**GPIO5-9*/
					0x121F0000,0x12200000,0x12210000,0x12220000,0x12240000,/**GPIO10-14*/
					0x12250000,0x1226000,0x12270000,0x12280000/**GPIO15-18*/
					};
#endif
/***************************************************
* 设置某地址的值
* phyaddr: 地址
* val: 需要设置的值
* Return: -1 失败 0 成功
****************************************************/
int SetReg(unsigned long phyaddr, unsigned int val);

/***************************************************
* 获取某地址的值
* phyaddr: 地址
* *val: 返回的值
* Return: -1 失败 0 成功
****************************************************/
int GetReg(unsigned long phyaddr, unsigned int *val);

/***************************************************
* 设置GPIO输入或者输出模式 (读出再写入)
* group: HiGpioBaseAddrTab数组的下标
* pin: 引脚
* isOutput:  0 输入 1 输出
* 举个栗子: 要设置GPIO17_5 为输出模式 -->  HisiGpioDirCfg(17,5,1);
****************************************************/	
void HisiGpioDirCfg(int group,int pin,bool isOutput);

/***************************************************
* 设置GPIO的值为高电平或低电平 (没有读出直接写)
* group: HiGpioBaseAddrTab数组的下标
* pin: 引脚
* isOutput:  0 低电平 1 高电平
* 举个栗子: 要设置GPIO17_5 为高点平 -->  HisiGpioDirCfg(17,5,1);
****************************************************/
void HisiGpioSetValue(int group,int pin,bool isHeight);

/***************************************************
* 获取GPIO的值
* group: HiGpioBaseAddrTab数组的下标
* pin: 引脚
* Return : GPIO的值
****************************************************/
int HisiGpioGetValue(int group,int pin);

/***************************************************
* 初始化s32GPIOflagMutex锁
****************************************************/
void s32GPIOflagMutexInit();

/***************************************************
* 设置报警时长 单位: second 默认3秒
****************************************************/
void Sets32AlarmTimeValue(int value);

/***************************************************
* 设置s32GPIOflag值
****************************************************/
void Sets32GPIOflagValue(int value);

/***************************************************
* 自定义回调函数
****************************************************/
void RegisterCallBackFunc(GPIOCallBackFuncHandle func);

/***************************************************
* 默认回调函数
****************************************************/
void* DefaultCallBackFunc(void *argv);

/***************************************************
* 创建线程 
* Return : 
*		0	成功
* 		-1  创建线程失败
****************************************************/
int CreateGPIODealThread();
#endif
