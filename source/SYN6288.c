#include "reg51.h"
#include <string.h>
#include"SYN6288.h"

#define uchar unsigned char
#define uint unsigned int

/**************оƬ��������*********************/
uchar SYN_StopCom[] = {0xFD, 0X00, 0X02, 0X02, 0XFD}; //ֹͣ�ϳ�
uchar SYN_SuspendCom[] = {0XFD, 0X00, 0X02, 0X03, 0XFC}; //��ͣ�ϳ�
uchar SYN_RecoverCom[] = {0XFD, 0X00, 0X02, 0X04, 0XFB}; //�ָ��ϳ�
uchar SYN_ChackCom[] = {0XFD, 0X00, 0X02, 0X21, 0XDE}; //״̬��ѯ
uchar SYN_PowerDownCom[] = {0XFD, 0X00, 0X02, 0X88, 0X77}; //����POWER DOWN ״̬����

//����1��ʼ��
void UART1_Init(void)
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	TMOD |= 0x20;		//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xFD;		//�趨��ʱ��ֵ
	TH1 = 0xFD;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1
	ES = 1;
	EA = 1;
}

//����2����һ�ֽ�����
void UART1_SendByte(uchar DAT)
{
	SBUF = DAT;
	while(TI == 0);
	TI=0;
}

//����2�����ַ�������
void UART1_SendString(uchar *DAT, uchar len)
{
	uchar i;
	for(i = 0; i < len; i++)
	{
		UART1_SendByte(*DAT++);
	}
}

//����1�ж�
void Uart1_Isr() interrupt 4
{
	if(RI)
	{
		RI=0;
	}

}


//Music:  0:�ޱ�������  1~15:ѡ�񱳾�����
void SYN_FrameInfo(uchar Music, uchar *HZdata)
{
	/****************��Ҫ���͵��ı�**********************************/
	uchar  Frame_Info[50];
	uchar  HZ_Length;
	uchar  ecc  = 0;  			//����У���ֽ�
	uint i = 0;
	HZ_Length = strlen((char*)HZdata); 			//��Ҫ�����ı��ĳ���

	/*****************֡�̶�������Ϣ**************************************/
	Frame_Info[0] = 0xFD ; 			//����֡ͷFD
	Frame_Info[1] = 0x00 ; 			//�������������ȵĸ��ֽ�
	Frame_Info[2] = HZ_Length + 3; 		//�������������ȵĵ��ֽ�
	Frame_Info[3] = 0x01 ; 			//���������֣��ϳɲ�������
	Frame_Info[4] = 0x01 | Music << 4 ; //����������������������趨

	/*******************У�������***************************************/
	for(i = 0; i < 5; i++)   				//���η��͹���õ�5��֡ͷ�ֽ�
	{
		ecc = ecc ^ (Frame_Info[i]);		//�Է��͵��ֽڽ������У��
	}

	for(i = 0; i < HZ_Length; i++)   		//���η��ʹ��ϳɵ��ı�����
	{
		ecc = ecc ^ (HZdata[i]); 				//�Է��͵��ֽڽ������У��
	}
	/*******************����֡��Ϣ***************************************/
	memcpy(&Frame_Info[5], HZdata, HZ_Length);
	Frame_Info[5 + HZ_Length] = ecc;
	UART1_SendString(Frame_Info, 5 + HZ_Length + 1);
}


/***********************************************************
* ��    �ƣ� YS_SYN_Set(uchar *Info_data)
* ��    �ܣ� ������	�������
* ��ڲ����� *Info_data:�̶���������Ϣ����
* ���ڲ�����
* ˵    �����������������ã�ֹͣ�ϳɡ���ͣ�ϳɵ����� ��Ĭ�ϲ�����9600bps��
* ���÷�����ͨ�������Ѿ�������������������á�
**********************************************************/
void YS_SYN_Set(uchar *Info_data)
{
	uchar Com_Len;
	Com_Len = strlen((char*)Info_data);
	UART1_SendString(Info_data, Com_Len);
}
		//ѡ�񱳾�����2��(0���ޱ�������  1-15���������ֿ�ѡ)
		//m[0~16]:0��������Ϊ������16���������������
		//v[0~16]:0�ʶ�����Ϊ������16�ʶ��������
		//t[0~5]:0�ʶ�����������5�ʶ��������
		//���������ù�����ο������ֲ�
		//SYN_FrameInfo(2, "[v5][t5]��ӭʹ�������콢��SYN6288�����ϳ�ģ��");
		//��ͣ�ϳɣ���ʱû���õ�������չʾ�����÷�
		//YS_SYN_Set(SYN_SuspendCom);