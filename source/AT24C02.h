#ifndef _AT24C02_H
#define _AT24C02_H

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

#endif