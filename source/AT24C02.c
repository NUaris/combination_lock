#include"AT24C02.h"
#include"config.h"
#include"delay.h"


void init();  //24c02��ʼ���ӳ���
void start();        //����I2C����
void stop();         //ֹͣI2C����
void writebyte(unsigned char j);  //дһ���ֽ�
unsigned char readbyte();   //��һ���ֽ�
void clock();         //I2C����ʱ��
////////��24c02�ĵ�ַaddress�ж�ȡһ���ֽ�����/////
unsigned char read24c02(unsigned char address);
//////��24c02��address��ַ��д��һ�ֽ�����info/////
void write24c02(unsigned char address,unsigned char info);




void init()  //24c02��ʼ���ӳ���
{
	scl=1;
	nop();
	sda=1;
	nop();
}

void start()        //����I2C����
{
	sda=1;
	nop();
	scl=1;
	nop();
	sda=0;
	nop();
	scl=0;
	nop();
}

void stop()         //ֹͣI2C����
{
	sda=0;
	nop();
	scl=1;
	nop();
	sda=1;
	nop();
}

void writebyte(unsigned char j)  //дһ���ֽ�
{
	unsigned char i,temp;
   	temp=j;
   	for (i=0;i<8;i++)
   {
	   temp=temp<<1;
	   scl=0;
	   nop();
	   sda=CY;		//temp����ʱ���Ƴ���ֵ������CY��
	   nop();
	   scl=1;
	   nop();
   }
   scl=0;
   nop();
   sda=1;
   nop();
}

unsigned char readbyte()   //��һ���ֽ�
{
   unsigned char i,j,k=0;
   scl=0; nop(); sda=1;
   for (i=0;i<8;i++)
   {  
		nop(); scl=1; nop();
      	if(sda==1) 
		j=1;
      	else
		j=0;
      	k=(k<<1)|j;
	  	scl=0;
	}
   	nop();
	return(k);
}

void clock()         //I2C����ʱ��
{
   unsigned char i=0;
   scl=1;
   nop();
   while((sda==1)&&(i<255))
   	  i++;
   scl=0;
   nop();
}

////////��24c02�ĵ�ַaddress�ж�ȡһ���ֽ�����/////
unsigned char read24c02(unsigned char address)
{
   unsigned char i;
   start();
   writebyte(0xa0);
   clock();
   writebyte(address);
   clock();
   start();
   writebyte(0xa1);
   clock();
   i=readbyte();
   stop();
   delay(100);
   return(i);
}

//////��24c02��address��ַ��д��һ�ֽ�����info/////
void write24c02(unsigned char address,unsigned char info)
{
   start();
   writebyte(0xa0);
   clock();
   writebyte(address);
   clock();
   writebyte(info);
   clock();
   stop();
   delay(5000); 
}