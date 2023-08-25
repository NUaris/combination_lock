#include"AT24C02PZ.h"
#include<reg52.h>
#include"config.h"
#include"delay.h"
  /*********************IIC.h*************************************/
//--����ʹ�õ�IO��--//
//sbit I2C_SCL = P3^3;
//sbit I2C_SDA = P3^4;
//--����ȫ�ֱ���--//
void I2C_Delay10us();
void I2C_Start();           //��ʼ�źţ���I2C_SCLʱ���ź��ڸߵ�ƽ�ڼ�I2C_SDA�źŲ���һ���½���
void I2C_Stop();            //��ֹ�źţ���I2C_SCLʱ���źŸߵ�ƽ�ڼ�I2C_SDA�źŲ���һ��������
uchar I2C_SendByte(uchar dat, uchar ack);//ʹ��I2c��ȡһ���ֽ�
uchar I2C_ReadByte();       //ͨ��I2C����һ���ֽڡ���I2C_SCLʱ���źŸߵ�ƽ�ڼ䣬���ַ����ź�I2C_SDA�����ȶ�
void At24c02Write(unsigned char addr,unsigned char dat);
unsigned char At24c02Read(unsigned char addr);
/*********************IIC.c*************************************/

//��ʱ:1us
void I2C_Delay10us()
{
	uchar a, b;
	for(b=1; b>0; b--)
	{
		for(a=2; a>0; a--);
	}
}

// ��ʼ�źţ���I2C_SCLʱ���ź��ڸߵ�ƽ�ڼ�I2C_SDA�źŲ���һ���½���
void I2C_Start()
{
	I2C_SDA = 1;
	I2C_Delay10us();
	I2C_SCL = 1;
	I2C_Delay10us();//����ʱ����I2C_SDA����ʱ��>4.7us
	I2C_SDA = 0;
	I2C_Delay10us();//����ʱ����>4us
	I2C_SCL = 0;			
	I2C_Delay10us();		
}

//��ֹ�źţ���I2C_SCLʱ���źŸߵ�ƽ�ڼ�I2C_SDA�źŲ���һ��������
void I2C_Stop()
{
	I2C_SDA = 0;
	I2C_Delay10us();
	I2C_SCL = 1;
	I2C_Delay10us();//����ʱ�����4.7us
	I2C_SDA = 1;
	I2C_Delay10us();		
}

//ͨ��I2C����һ���ֽڡ���I2C_SCLʱ���źŸߵ�ƽ�ڼ䣬 ���ַ����ź�I2C_SDA�����ȶ�
uchar I2C_SendByte(uchar dat, uchar ack)
{
	uchar a = 0,b = 0;//���255��һ����������Ϊ1us�������ʱ255us��			
	for(a=0; a<8; a++)//Ҫ����8λ�������λ��ʼ
	{
		I2C_SDA = dat >> 7;	 //��ʼ�ź�֮��I2C_SCL=0�����Կ���ֱ�Ӹı�I2C_SDA�ź�
		dat = dat << 1;
		I2C_Delay10us();
		I2C_SCL = 1;
		I2C_Delay10us();//����ʱ��>4.7us
		I2C_SCL = 0;
		I2C_Delay10us();//ʱ�����4us		
	}
	I2C_SDA = 1;
	I2C_Delay10us();
	I2C_SCL = 1;
	while(I2C_SDA && (ack == 1))//�ȴ�Ӧ��Ҳ���ǵȴ����豸��I2C_SDA����
	{
		b++;
		if(b > 200)	 //�������200usû��Ӧ����ʧ�ܣ�����Ϊ��Ӧ�𣬱�ʾ���ս���
		{
			I2C_SCL = 0;
			I2C_Delay10us();
			return 0;
		}
	}
	I2C_SCL = 0;
	I2C_Delay10us();
 	return 1;		
}

// ʹ��I2c��ȡһ���ֽ�
uchar I2C_ReadByte()
{
	uchar a = 0,dat = 0;
	I2C_SDA = 1;			//��ʼ�ͷ���һ���ֽ�֮��I2C_SCL����0
	I2C_Delay10us();
	for(a=0; a<8; a++)//����8���ֽ�
	{
		I2C_SCL = 1;
		I2C_Delay10us();
		dat <<= 1;
		dat |= I2C_SDA;
		I2C_Delay10us();
		I2C_SCL = 0;
		I2C_Delay10us();
	}
	return dat;		
}
//��������		   : ��24c02��һ����ַд��һ������

void At24c02Write(unsigned char addr,unsigned char dat)
{
	I2C_Start();
	I2C_SendByte(0xa0, 1);//����д������ַ
	I2C_SendByte(addr, 1);//����Ҫд���ڴ��ַ
	I2C_SendByte(dat, 0);	//��������
	I2C_Stop();
}
// ��ȡ24c02��һ����ַ��һ������

unsigned char At24c02Read(unsigned char addr)
{
	unsigned char num;
	I2C_Start();
	I2C_SendByte(0xa0, 1); //����д������ַ
	I2C_SendByte(addr, 1); //����Ҫ��ȡ�ĵ�ַ
	I2C_Start();
	I2C_SendByte(0xa1, 1); //���Ͷ�������ַ
	num=I2C_ReadByte(); //��ȡ����
	I2C_Stop();
	return num;	
}

void EEPROM_READ(unsigned char addr_start,unsigned char * dat,unsigned char addr_len)//��ȡһ�����ݣ�addr_startΪ��ʼ��ַ��dataΪд������ݣ�addr_lenΪ���ݳ���
{
    unsigned char a;
    
    for(a=addr_start;a<addr_start+addr_len;a++)
    {
        *dat=At24c02Read(a);
        dat++;
        delay(20);
    }
}

void EEPROM_WRITE(unsigned char addr_start,unsigned char * dat,unsigned char addr_len)//д��һ�����ݣ�addr_startΪ��ʼ��ַ��dataΪд������ݣ�addr_lenΪ���ݳ���
{
    unsigned char a;
    
    for(a=addr_start;a<addr_start+addr_len;a++)
    {
        At24c02Write(a,*dat);
        dat++;
        delay(20);
    }
}
