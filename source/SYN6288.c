#include "reg51.h"
#include <string.h>
#include"SYN6288.h"

#define uchar unsigned char
#define uint unsigned int

/**************芯片设置命令*********************/
uchar SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //停止合成
uchar SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //暂停合成
uchar SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //恢复合成
uchar SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //状态查询
uchar SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //进入POWER DOWN 状态命令

//串口1初始化
void UART1_Init(void)
{
	SCON = 0x50;		//8位数据,可变波特率
	TMOD |= 0x20;		//设定定时器1为8位自动重装方式
	TL1 = 0xFD;		//设定定时初值
	TH1 = 0xFD;		//设定定时器重装值
	ET1 = 0;		//禁止定时器1中断
	TR1 = 1;		//启动定时器1
	ES = 1;
	EA = 1;
}

//串口2发送一字节数据
void UART1_SendByte(uchar DAT)
{
	SBUF = DAT;
	while(TI == 0);
	TI=0;
}

//串口2发送字符串数据
void UART1_SendString(uchar *DAT, uchar len)
{
	uchar i;
	for(i = 0; i < len; i++)
	{
		UART1_SendByte(*DAT++);
	}
}

//串口1中断
void Uart1_Isr() interrupt 4
{
	if(RI)
	{
		RI=0;
	}

}


//Music:  0:无背景音乐  1~15:选择背景音乐
void SYN_FrameInfo(uchar Music, uchar *HZdata)
{
	/****************需要发送的文本**********************************/
	uchar  Frame_Info[50];
	uchar  HZ_Length;
	uchar  ecc  = 0;  			//定义校验字节
	uint i = 0;
	HZ_Length = strlen((char*)HZdata); 			//需要发送文本的长度

	/*****************帧固定配置信息**************************************/
	Frame_Info[0] = 0xFD ; 			//构造帧头FD
	Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
	Frame_Info[2] = HZ_Length + 3; 		//构造数据区长度的低字节
	Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令
	Frame_Info[4] = 0x01 | Music << 4 ; //构造命令参数：背景音乐设定

	/*******************校验码计算***************************************/
	for(i = 0; i < 5; i++)   				//依次发送构造好的5个帧头字节
	{
		ecc = ecc ^ (Frame_Info[i]);		//对发送的字节进行异或校验
	}

	for(i = 0; i < HZ_Length; i++)   		//依次发送待合成的文本数据
	{
		ecc = ecc ^ (HZdata[i]); 				//对发送的字节进行异或校验
	}
	/*******************发送帧信息***************************************/
	memcpy(&Frame_Info[5], HZdata, HZ_Length);
	Frame_Info[5 + HZ_Length] = ecc;
	UART1_SendString(Frame_Info, 5 + HZ_Length + 1);
}


/***********************************************************
* 名    称： YS_SYN_Set(uchar *Info_data)
* 功    能： 主函数	程序入口
* 入口参数： *Info_data:固定的配置信息变量
* 出口参数：
* 说    明：本函数用于配置，停止合成、暂停合成等设置 ，默认波特率9600bps。
* 调用方法：通过调用已经定义的相关数组进行配置。
**********************************************************/
void YS_SYN_Set(uchar *Info_data)
{
	uchar Com_Len;
	Com_Len = strlen((char*)Info_data);
	UART1_SendString(Info_data, Com_Len);
}
		//选择背景音乐2。(0：无背景音乐  1-15：背景音乐可选)
		//m[0~16]:0背景音乐为静音，16背景音乐音量最大
		//v[0~16]:0朗读音量为静音，16朗读音量最大
		//t[0~5]:0朗读语速最慢，5朗读语速最快
		//其他不常用功能请参考数据手册
		//SYN_FrameInfo(2, "[v5][t5]欢迎使用绿深旗舰店SYN6288语音合成模块");
		//暂停合成，此时没有用到，用于展示函数用法
		//YS_SYN_Set(SYN_SuspendCom);