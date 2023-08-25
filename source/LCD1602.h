#ifndef _LCD1602_H
#define _LCD1602_H

#include"config.h"

void Read_Busy();
void LCD1602_Write_Cmd(unsigned char cmd);//д����  
void LCD1602_Write_Dat(unsigned char dat);//д����
void LCD1602_Clear_Screen();//����
void LCD1602_Open_Cursor();//��ʾ���
void LCD1602_Close_Cursor();//�رչ��
void LCD1602_Set_Cursor(unsigned char x, unsigned char y);//��������
void LCD1602_Dis_OneChar(unsigned char x, unsigned char y, unsigned char dat);//�����ַ�
void LCD1602_Dis_Str(unsigned char x, unsigned char y, unsigned char *str);//�����ַ���
void Init_LCD1602();//��ʼ��


#endif