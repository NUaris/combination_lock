#ifndef _AT24C02_H
#define _AT24C02_H

void init();  //24c02初始化子程序
void start();        //启动I2C总线
void stop();         //停止I2C总线
void writebyte(unsigned char j);  //写一个字节
unsigned char readbyte();   //读一个字节
void clock();         //I2C总线时钟
////////从24c02的地址address中读取一个字节数据/////
unsigned char read24c02(unsigned char address);
//////向24c02的address地址中写入一字节数据info/////
void write24c02(unsigned char address,unsigned char info);

#endif