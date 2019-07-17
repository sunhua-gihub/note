#include "gpio_manage.h"

int s32AlarmTime = 3;
int s32GPIOflag = 0;
pthread_mutex_t s32GPIOflag_mutex;
GPIOCallBackFuncHandle g_CallBackHandle = NULL;

int SetReg(unsigned long phyaddr, unsigned int val)
{
	int fd;
	unsigned long virtaddr;

	fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd < 0)
	{
		printf("mem dev Open Failed\n");
		return -1;
	}
	
	virtaddr = (unsigned long)mmap((void *)0, REG_SPACE_SIZE,
				PROT_READ | PROT_WRITE, MAP_SHARED, fd, (phyaddr&0xfffff000));
	if(virtaddr == 0xffffffff)
	{
		printf("Memory Map Failed For Address\n");
		close(fd);
		return -1;
	}
	
	*((volatile unsigned int *)(virtaddr + (phyaddr&0x00000fff))) = val;
	
	munmap((void *)virtaddr,REG_SPACE_SIZE);
	
	close(fd);
	return 0;	
}

//Get Reg function
int GetReg(unsigned long phyaddr, unsigned int *val)
{
	int fd;
	unsigned long virtaddr;

	fd = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd < 0)
	{
		printf("mem dev Open Failed\n");
		return -1;
	}
	virtaddr = (unsigned long)mmap((void *)0, REG_SPACE_SIZE,
				PROT_READ | PROT_WRITE, MAP_SHARED, fd, (phyaddr&0xfffff000));

	if(virtaddr == 0xffffffff)
	{
		printf("Memory Map Failed For Address\n");
		close(fd);
		return -1;
	}
	*val = *((volatile unsigned int *)(virtaddr + (phyaddr&0x00000fff)));

	munmap((void *)virtaddr,REG_SPACE_SIZE);
	close(fd);
	return 0;
}

//Set GPIO Direction
void HisiGpioDirCfg(int group,int pin,bool isOutput)
{
    unsigned int val = 0;
    unsigned int dirAddr = 0;
    dirAddr = GPIO_GROUP_DIR_ADDR(HiGpioBaseAddrTab[group]);
 
    if( 0 == GetReg(dirAddr,&val) )
    {
		if(isOutput)
		{
			val |= 1 << pin;
		}
		else
		{
			val &= ~(1 << pin);
		}
		if( 0 != SetReg(dirAddr,val) )
		{
			printf("[HisiGpioDirCfg] SetReg Error! val: %d\n",val);
		}
		printf("[HisiGpioDirCfg]addr: 0x%08lX =[0x%08lX] \n", dirAddr, val);
    }
	else
	{
		printf("[HisiGpioDirCfg] GetReg Error! val: %d\n",val);
	}
}
 
//Set Value
void HisiGpioSetValue(int group,int pin,bool isHeight)
{
    unsigned int val = 0;
    unsigned int dataAddr = 0;
    dataAddr = GPIO_DATA_ADDR(HiGpioBaseAddrTab[group], pin);
    if(isHeight)
    {
    	val = 1 << pin;
    }
    else
    {
    	val = 0;
    }
    if( 0 != SetReg(dataAddr,val) )
    {
        printf("HisiGpioSetValue Error! val: %d\n",val);
    }
}
 
//Get Value
int HisiGpioGetValue(int group,int pin)
{
    unsigned int val = 0;
    unsigned int dataAddr = 0;
    dataAddr = GPIO_DATA_ADDR(HiGpioBaseAddrTab[group], pin);
    if( 0 == GetReg(dataAddr,&val) )
    {
        if(val & (1 << pin))
        {
            val = 1;
        }
        else
        {
            val = 0;
        }
        return val;
    }
    else
    {
        printf("HisiGpioGetValue Error! val: %d\n",val);
        return -1;
    }
}

void s32GPIOflagMutexInit()
{
	pthread_mutex_init(&s32GPIOflag_mutex,NULL);
}

void Sets32AlarmTimeValue(int value)
{
	s32AlarmTime = value;
}

void Sets32GPIOflagValue(int value)
{
	pthread_mutex_lock(&s32GPIOflag_mutex);
	s32GPIOflag = value;
	pthread_mutex_unlock(&s32GPIOflag_mutex);
}

void RegisterCallBackFunc(GPIOCallBackFuncHandle func)
{
	g_CallBackHandle = func;
}

void* DefaultCallBackFunc(void *argv)
{
//	pthread_mutex_init(&s32GPIOflag_mutex, NULL);
	int s32flag = 0;	//为了减少s32GPIOflag_mutex上锁时间

	//四路输出
#if defined(_USE_3559)
	HisiGpioDirCfg(17,5,1);
	HisiGpioDirCfg(17,4,1);
	HisiGpioDirCfg(17,3,1);
	HisiGpioDirCfg(14,0,1);
#else
	HisiGpioDirCfg(11,0,1);
	HisiGpioDirCfg(11,1,1);
	HisiGpioDirCfg(11,2,1);
	HisiGpioDirCfg(11,3,1);
	SetReg(0x120f0000 + 0x1d0, 0x00);
	SetReg(0x120f0000 + 0x1d4, 0x00);
	SetReg(0x120f0000 + 0x1d8, 0x00);
	SetReg(0x120f0000 + 0x1dc, 0x00);
#endif
	while(1)
	{
		pthread_mutex_lock(&s32GPIOflag_mutex);
		if(s32GPIOflag == 1)
		{
			s32flag = 1;
			s32GPIOflag = 0;
		}
		pthread_mutex_unlock(&s32GPIOflag_mutex);
		if(s32flag == 1)
		{
#if defined(_USE_3559)
			HisiGpioSetValue(17,5,1);
			HisiGpioSetValue(17,4,1);
			HisiGpioSetValue(17,3,1);
			HisiGpioSetValue(14,0,1);
			sleep(s32AlarmTime);
			HisiGpioSetValue(17,5,0);
			HisiGpioSetValue(17,4,0);
			HisiGpioSetValue(17,3,0);
			HisiGpioSetValue(14,0,0);
#else
			HisiGpioSetValue(11,0,1);
			HisiGpioSetValue(11,1,1);
			HisiGpioSetValue(11,2,1);
			HisiGpioSetValue(11,3,1);
			sleep(s32AlarmTime);
			HisiGpioSetValue(11,0,0);
			HisiGpioSetValue(11,1,0);
			HisiGpioSetValue(11,2,0);
			HisiGpioSetValue(11,3,0);
			printf("i am hisi3536\n");
#endif
			s32flag = 0;
		}
		sleep(2);
	}
//	pthread_mutex_destory(&s32GPIOflag_mutex);
}

int CreateGPIODealThread()
{
	if(g_CallBackHandle == NULL) {
		printf("%s:<%s>[%d]Set the DefaultCallBackFunc!\n",__FILE__,__FUNCTION__,__LINE__);
		g_CallBackHandle = DefaultCallBackFunc;
	}
	printf("g_CallBackHandle %p\n",g_CallBackHandle);
	
	pthread_t tid;

	pthread_attr_t attr_record;
	pthread_attr_init(&attr_record);
	pthread_attr_setscope(&attr_record, PTHREAD_SCOPE_SYSTEM);
	pthread_attr_setdetachstate(&attr_record, PTHREAD_CREATE_JOINABLE);

	if (pthread_create(&tid, &attr_record, g_CallBackHandle, NULL))
	{
		printf("%s:<%s>[%d]Create Thread fail!\n",__FILE__,__FUNCTION__,__LINE__);
		pthread_attr_destroy(&attr_record);
		return -1;
	}
	printf("Thread is start!\n");
	pthread_attr_destroy(&attr_record);
	
	return 0;
}
