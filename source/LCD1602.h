#ifndef _LCD1602_H
#define _LCD1602_H

#include"config.h"

void Read_Busy();
void LCD1602_Write_Cmd(unsigned char cmd);//写命令  
void LCD1602_Write_Dat(unsigned char dat);//写数据
void LCD1602_Clear_Screen();//清屏
void LCD1602_Open_Cursor();//显示光标
void LCD1602_Close_Cursor();//关闭光标
void LCD1602_Set_Cursor(unsigned char x, unsigned char y);//设置坐标
void LCD1602_Dis_OneChar(unsigned char x, unsigned char y, unsigned char dat);//输入字符
void LCD1602_Dis_Str(unsigned char x, unsigned char y, unsigned char *str);//输入字符串
void Init_LCD1602();//初始化


#endif