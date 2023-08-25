#include"LCD1602.h"
//#include"config.h"
#include"intrins.h"
#include"reg52.h"


void Read_Busy();
void LCD1602_Write_Cmd(uchar cmd);//д����  
void LCD1602_Write_Dat(uchar dat);//д����
void LCD1602_Clear_Screen();//����
void LCD1602_Open_Cursor();//��ʾ���
void LCD1602_Close_Cursor();//�رչ��
void LCD1602_Set_Cursor(uchar x, uchar y);//��������
void LCD1602_Dis_OneChar(uchar x, uchar y, uchar dat);//�����ַ�
void LCD1602_Dis_Str(uchar x, uchar y, uchar *str);//�����ַ���
void Init_LCD1602();//��ʼ��




void Read_Busy()
{
	uchar busy;
	LCD1602_DB = 0xff;//��λ��������
	LCD1602_RS = 0;	  //����RS
	LCD1602_RW = 1;	  //����RW��
	do
	{
		LCD1602_EN = 1;//ʹ��EN
		busy = LCD1602_DB;//��������
		LCD1602_EN = 0;	 //����ʹ���Ա�����һ�β���������
	}while(busy & 0x80); //�ж�״̬��BIT7λ�Ƿ�Ϊ1��Ϊ1���ʾæ������ȴ�
}
//д����
void LCD1602_Write_Cmd(uchar cmd)
{
	Read_Busy();	 //�ж�æ��æ��ȴ�
	LCD1602_RS = 0;	 //ѡ��Ϊд����
	LCD1602_RW = 0;	//ģʽΪд
	LCD1602_DB = cmd;//д������
	LCD1602_EN = 1;	 //����ʹ�ܶ� ���ݱ����䵽LCD1602��
	LCD1602_EN = 0;	 //����ʹ���Ա�����һ�β���������
}
//д����
void LCD1602_Write_Dat(uchar dat)
{
	Read_Busy();  //�ж�æ��æ��ȴ�
	LCD1602_RS = 1;	//ѡ��Ϊд����
	LCD1602_RW = 0;	//ģʽΪд
	LCD1602_DB = dat; //д��8λ����
	LCD1602_EN = 1;	  //����ʹ�ܶ� ���ݱ����䵽LCD1602��
	LCD1602_EN = 0;	  //����ʹ���Ա�����һ�β���������
}

//����

void LCD1602_Clear_Screen()
{
	LCD1602_Write_Cmd(0x01);
}
//��ʾ���
void LCD1602_Open_Cursor()
{
	LCD1602_Write_Cmd(0x0F);
}
//�رչ��
void LCD1602_Close_Cursor()
{
	LCD1602_Write_Cmd(0x0C);
}
//��������
void LCD1602_Set_Cursor(uchar x, uchar y)
{
	if(y) x |= 0x40;
	x |= 0x80;
	LCD1602_Write_Cmd(x);
}
//�����ַ�
void LCD1602_Dis_OneChar(uchar x, uchar y, uchar dat)
{
	LCD1602_Set_Cursor(x, y);//����RAM��ʼλ��
	LCD1602_Write_Dat(dat);	 //д��һ���ֽڵ�����	
}
//�����ַ���
void LCD1602_Dis_Str(uchar x, uchar y, uchar *str)
{
	LCD1602_Set_Cursor(x, y);//����RAM��ʼλ��
	while(*str != '\0')	     //д���ַ�������
	{
		LCD1602_Write_Dat(*str++);
	}
}
//��ʼ��
void Init_LCD1602()
{
	LCD1602_Write_Cmd(0x38); //	����16*2��ʾ��5*7����8λ���ݽӿ�
	LCD1602_Write_Cmd(0x0c); //����ʾ,�رչ��
	LCD1602_Write_Cmd(0x06); //��дһ�ֽں��ַָ���1
	LCD1602_Write_Cmd(0x01); // ����
}