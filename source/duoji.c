#include"config.h"
#include"delay.h"
#include"duoji.h"
#include"MAIN.h"

//sbit PWM = P3^6;  //设定PWM输出的I/O端口
unsigned char count = 0, count1 = 0;
unsigned char timer1 ;
u8 overDoor = 0, overDoor_Init = 0;
//对于180°舵机
//t = 0.5ms——————-舵机会转动 0 °
//t = 1.0ms——————-舵机会转动 45°
//t = 1.5ms——————-舵机会转动 90°
//t = 2.0ms——————-舵机会转动 135°
//t = 2.5ms——————-舵机会转动180




/*延时程序*/
void delay1s(void)   //误差 0us
{
    unsigned char a, b, c;

    for(c = 167; c > 0; c--)
        for(b = 171; b > 0; b--)
            for(a = 16; a > 0; a--);

    _nop_();  //if Keil,require use intrins.h
}

/*定时器T0初始化*/
void Timer0_Init()
{
    TMOD &= 0x00;
    TMOD = 0x01; //定时器T0设置成方式1

    TH0 = 0xff;   //定时常数 0.1ms 晶振为11.0592MHz
    TL0 = 0xa4;

    ET0 = 1;
    TR0 = 0;
    EA = 1;
}

/*T0中断初始化*/
//void Time0_Init() interrupt 1
//{
//    if(overDoor != 1)
//    {
//        TR0 = 0;
//        TH0 = 0xff; // 0.1ms
//        TL0 = 0xa4;

//        if(count <= timer1) //5==0° 15==90°
//        {
//            PWM = 1;
//        }
//        else
//        {
//            PWM = 0;
//        }

//        count++;

//        if (count >= 200) //T = 20ms清零
//        {
//            count = 0;
//        }

//        TR0 = 1; //开启T0
//    }
//    else
//    {
//        TR0 = 0;
//        TH0 = 0xff; // 0.1ms
//        TL0 = 0xa4;

//        if(count < 100000) //10s°
//        {
//            overDoor_Init = 1;
//        }

//        count1++;

//        if(count == 100000) //10s°
//        {
//            count1 = 0;
//        }

//        TR0 = 1;
//    }
//}
void Time0_Init() interrupt 1
{
        TR0 = 0;
        TH0 = 0xff; // 0.1ms
        TL0 = 0xa4;

        if(count <= timer1) //5==0° 15==90°
        {
            PWM = 1;
        }
        else
        {
            PWM = 0;
        }

        count++;

        if (count >= 200) //T = 20ms清零
        {
            count = 0;
        }

        TR0 = 1; //开启T0
    
}
void duoji_on()
{
    TH0 = 0xff; // 0.1ms
    TL0 = 0xa4;
//    ET0 = 1;
    TR0 = 1;
}

void duoji_off()
{
    count = 0;
    //ET0 = 0;
    TR0 = 0;
    TH0 = 0xff; // 0.1ms
    TL0 = 0xa4;
//   count1 = 0;
}

//t = 0.5ms——————-舵机会转动 0 ° angle=5
//t = 1.0ms——————-舵机会转动 45° angle=10
//t = 1.5ms——————-舵机会转动 90° angle=15
//t = 2.0ms——————-舵机会转动 135°angle=20
//t = 2.5ms——————-舵机会转动180   angle=25
void duoji_start(unsigned char angle)//旋转角度
{
    timer1 = angle; //舵机恢复到0°的位置
//	count=0;//让定时器重新计数
   delay1s();
}
void duoji_Init()
{
    count = 0;
    duoji_start(10);
    duoji_on();
    delay1s();
    duoji_off();
    count = 0;
 //   count1 = 0;
    //PWM = 0;
}

void duoji_overDoor()
{
    duoji_start(5);
    duoji_on();
    delay1s();
    duoji_off();

}

void duoji_openDoor()
{
    duoji_start(15);
    duoji_on();
    delay1s();
    duoji_off();
    //overDoor = 1;
    //duoji_overDoor();
}
//void duoji_test()
//{
//    if(overDoor_Init == 1)
//    {
//        overDoor_Init = 0;
//        overDoor = 0;
//        TH0 = 0xff; // 0.1ms
//        TL0 = 0xa4;
//        count1 = 0;
//        ET0 = 0;
//        TR0 = 0;
//        beep();
//        delay(200);
//        beep();
//        delay(200);
//        beep();
//        duoji_overDoor();
//    }
//}
