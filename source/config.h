#ifndef _CONFIG_H
#define _CONFIG_H
//�����ļ�

/****************************************************************
 ͨ��ͷ�ļ�
****************************************************************/
#include <reg52.h>
#include <intrins.h>

#define MAIN_Fosc		11059200UL	//�궨����ʱ��HZ
/*************************************************************
�Զ���ͷ�ļ�
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
���������������¶���
****************************************************************/
typedef signed char int8;  //8λ�з�����
typedef signed int  int16; //16λ�з�����
typedef unsigned char uint8;  //8λ�޷�����
typedef unsigned char u8;  //8λ�޷�����
typedef unsigned char uchar;  //8λ�޷�����
typedef unsigned int  uint16; //16λ�޷�����
typedef unsigned int  u16; //16λ�޷�����
typedef unsigned int  uint; //16λ�޷�����
typedef unsigned long uint32; //32λ�޷�����

typedef	unsigned char	BOOLEAN;
typedef	unsigned char	INT8U;
typedef	signed char		INT8S;
typedef	unsigned int	INT16U;
typedef	signed int		INT16S;
typedef	unsigned long  	INT32U;
typedef	signed long    	INT32S;

/****************************************************************
 Ӳ���ӿڶ���
****************************************************************/

//LCD1602�ӿڶ���
#define LCD1602_DB  P0
sbit LCD1602_RS = P2^5;		//LCD1602Һ������/����ѡ������
sbit LCD1602_RW = P2^6;		//LCD1602Һ����/дѡ������
sbit LCD1602_EN = P2^7;       //LCD1602Һ��ʹ������
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
