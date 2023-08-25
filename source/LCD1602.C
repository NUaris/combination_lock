#include"LCD1602.h"
//#include"config.h"
#include"intrins.h"
#include"reg52.h"


void Read_Busy();
void LCD1602_Write_Cmd(uchar cmd);//写命令  
void LCD1602_Write_Dat(uchar dat);//写数据
void LCD1602_Clear_Screen();//清屏
void LCD1602_Open_Cursor();//显示光标
void LCD1602_Close_Cursor();//关闭光标
void LCD1602_Set_Cursor(uchar x, uchar y);//设置坐标
void LCD1602_Dis_OneChar(uchar x, uchar y, uchar dat);//输入字符
void LCD1602_Dis_Str(uchar x, uchar y, uchar *str);//输入字符串
void Init_LCD1602();//初始化




void Read_Busy()
{
	uchar busy;
	LCD1602_DB = 0xff;//复位数据总线
	LCD1602_RS = 0;	  //拉低RS
	LCD1602_RW = 1;	  //拉高RW读
	do
	{
		LCD1602_EN = 1;//使能EN
		busy = LCD1602_DB;//读回数据
		LCD1602_EN = 0;	 //拉低使能以便于下一次产生上升沿
	}while(busy & 0x80); //判断状态字BIT7位是否为1，为1则表示忙，程序等待
}
//写命令
void LCD1602_Write_Cmd(uchar cmd)
{
	Read_Busy();	 //判断忙，忙则等待
	LCD1602_RS = 0;	 //选择为写命令
	LCD1602_RW = 0;	//模式为写
	LCD1602_DB = cmd;//写入命令
	LCD1602_EN = 1;	 //拉高使能端 数据被传输到LCD1602内
	LCD1602_EN = 0;	 //拉低使能以便于下一次产生上升沿
}
//写数据
void LCD1602_Write_Dat(uchar dat)
{
	Read_Busy();  //判断忙，忙则等待
	LCD1602_RS = 1;	//选择为写数据
	LCD1602_RW = 0;	//模式为写
	LCD1602_DB = dat; //写入8位数据
	LCD1602_EN = 1;	  //拉高使能端 数据被传输到LCD1602内
	LCD1602_EN = 0;	  //拉低使能以便于下一次产生上升沿
}

//清屏

void LCD1602_Clear_Screen()
{
	LCD1602_Write_Cmd(0x01);
}
//显示光标
void LCD1602_Open_Cursor()
{
	LCD1602_Write_Cmd(0x0F);
}
//关闭光标
void LCD1602_Close_Cursor()
{
	LCD1602_Write_Cmd(0x0C);
}
//设置坐标
void LCD1602_Set_Cursor(uchar x, uchar y)
{
	if(y) x |= 0x40;
	x |= 0x80;
	LCD1602_Write_Cmd(x);
}
//输入字符
void LCD1602_Dis_OneChar(uchar x, uchar y, uchar dat)
{
	LCD1602_Set_Cursor(x, y);//设置RAM起始位置
	LCD1602_Write_Dat(dat);	 //写入一个字节的数据	
}
//输入字符串
void LCD1602_Dis_Str(uchar x, uchar y, uchar *str)
{
	LCD1602_Set_Cursor(x, y);//设置RAM起始位置
	while(*str != '\0')	     //写入字符串数据
	{
		LCD1602_Write_Dat(*str++);
	}
}
//初始化
void Init_LCD1602()
{
	LCD1602_Write_Cmd(0x38); //	设置16*2显示，5*7点阵，8位数据接口
	LCD1602_Write_Cmd(0x0c); //开显示,关闭光标
	LCD1602_Write_Cmd(0x06); //读写一字节后地址指针加1
	LCD1602_Write_Cmd(0x01); // 清屏
}