#include"config.h"
#include"delay.h"
void nop();
void delay(uint z);
void Delay_ms(int ms);
void delay_ns(unsigned int ns);

  void delay(uint z)
{
	uint x,y;
	for(x = z; x > 0; x--)
		for(y = 114; y > 0 ; y--); 		
} 

  void nop()
{
	_nop_();
	_nop_();
}

void Delay_ms(int ms)	 //ÑÓÊ±xms
{
    unsigned int i,j;
	for(i=ms;i>0;i--)
	   for(j=220;j>0;j--);
}

void delay_ns(unsigned int ns)
{
	unsigned int i;
	for(i=0;i<ns;i++)
	{
		nop();
		nop();
		nop();
	}
}

void beep(void)
{
    alarm=0;
    delay(150);
    alarm=1;
}
