#ifndef _CONFIG_H
#define _CONFIG_H
//配置文件

/****************************************************************
 通用头文件
****************************************************************/
#include <reg52.h>
#include <intrins.h>

#define MAIN_Fosc		11059200UL	//宏定义主时钟HZ
/*************************************************************
自定义头文件
**************************************************************/
/*#include"config.h"
#include"AT24C02.h"
#include"delay.h"
#include"JR6001.h"
#include"LCD1602.h"
#include"RFID.h"
#include"SPI.h"
#include"mimasuo.h"
*/
//#include".h"
/****************************************************************
已有数据类型重新定义
****************************************************************/
typedef signed char int8;  //8位有符号型
typedef signed int  int16; //16位有符号型
typedef unsigned char uint8;  //8位无符号型
typedef unsigned char u8;  //8位无符号型
typedef unsigned char uchar;  //8位无符号型
typedef unsigned int  uint16; //16位无符号型
typedef unsigned int  u16; //16位无符号型
typedef unsigned int  uint; //16位无符号型
typedef unsigned long uint32; //32位无符号型

typedef	unsigned char	BOOLEAN;
typedef	unsigned char	INT8U;
typedef	signed char		INT8S;
typedef	unsigned int	INT16U;
typedef	signed int		INT16S;
typedef	unsigned long  	INT32U;
typedef	signed long    	INT32S;

/****************************************************************
 硬件接口定义
****************************************************************/

//LCD1602接口定义
#define LCD1602_DB  P0
sbit LCD1602_RS = P2^5;		//LCD1602液晶命令/数据选择引脚
sbit LCD1602_RW = P2^6;		//LCD1602液晶读/写选择引脚
sbit LCD1602_EN = P2^7;       //LCD1602液晶使能引脚
//**************************************************************

//iic
//sbit sda=P1^1;                          
//sbit scl=P1^0;
sbit I2C_SCL = P3^3;
sbit I2C_SDA = P3^4;
//**************************************************************

sbit  spi_cs = P2^4;
sbit  spi_ck = P2^3;
sbit  spi_mosi = P2^2;
sbit  spi_miso = P2^1;
sbit  spi_rst = P2^0;

//
//**************************************************************

sbit PWM = P3^6;
sbit alarm = P3^5;
#endif
