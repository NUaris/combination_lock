#ifndef _SYN6288_H
#define _SYN6288_H

#define uchar unsigned char
#define uint unsigned int

void UART1_Init(void);
void UART1_SendByte(uchar DAT);
void UART1_SendString(uchar *DAT, uchar len);
void SYN_FrameInfo(uchar Music, uchar *HZdata);
//Music:  0:Œﬁ±≥æ∞“Ù¿÷  1~15:—°‘Ò±≥æ∞“Ù¿÷
void YS_SYN_Set(uchar *Info_data);

#endif