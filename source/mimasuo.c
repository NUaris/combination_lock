#include"config.h"
#include"MAIN.h"
#include"stdio.h"
#include<string.h>
/*
键盘序号说明    键盘功能定义
   0  1  2  3    1  2  3   a:   
   4  5  6  7    4  5  6   b:
   8  9 10 11    7  8  9   c: 
  12 13 14 15    up 0 down d:
*/

INT8U states=0;//状态机

INT8U Card_type[2],	//卡片类型
      Card_SN[4]={0},//IC卡号
	  Card_SN_BUF[4]={0xff,0xff,0xff,0xff},//卡号登记缓冲区	   
	  KEY_BUF[8]={'0','0','0','0','0','0','0','0'},
      ID_ASC_buf[9]="0";

INT8U bPass = 0, bWarn = 0;	

void init_all(void)	//初始化
{
	EA = 0;	      //关总中断		 
    Timer0_Init();
    PX0=1;
	init_rc522(); //RC522初始化	
	Init_LCD1602();	  //LCD初始化   
	EEPROM_READ(0,Card_SN_BUF,4);//EEPROM初始化,取出记录的卡号
    delay(20);
	EEPROM_READ(7,KEY_BUF,8);//EEPROM初始化,取出记录的密码 
    UART1_Init();
	EA = 1;		  //开总中断
    SYN_FrameInfo(2, "[v5][t5]欢迎使用密码锁");
}      


void pass( void )
{
   beep();
   Delay_ms(500);//让卡片离开
   //LED_BLINK_1();
}

void warn(void)
{
  //LED_BLINK_2();
    beep();
    delay(100);
    beep();
}

uchar key_scan()	//带返回值的子函数
{
    uchar KeyValue=16;
    
	P1 = 0xf0;//判断哪一列被按下
	if(P1 != 0xf0)
	{
		delay(5);//软件消抖
		if( P1 != 0xf0)
		{
			switch(P1)
			{
				case 0xe0:	KeyValue = 0;	break;
				case 0xd0:	KeyValue = 1;	break;
				case 0xb0:	KeyValue = 2;	break;
				case 0x70:	KeyValue = 3;	break;
			}
			P1 = 0x0f;//判断哪一行被按下
			switch(P1)
			{
				case 0x0e:	KeyValue = KeyValue;	break;
				case 0x0d:	KeyValue += 4;	break;
				case 0x0b:	KeyValue += 8;	break;
				case 0x07:	KeyValue += 12;	break;
			}
			while(P1 != 0x0f);//松手检测
		}			
	}

        
    return KeyValue;
}

INT8U IC_READ( void )
{
    INT8U ID_ASC[8],i;
	if( PcdRequest( PICC_REQIDL, Card_type ) != MI_OK )//寻天线区内未进入休眠状态的卡，返回卡片类型 2字节	
	{
		if( PcdRequest( PICC_REQIDL, Card_type ) != MI_OK )//寻天线区内未进入休眠状态的卡，返回卡片类型 2字节	
		{
		   //LCD1602_Dis_Str(0, 1, "id:");
			return FALSE;
		}	
	}
	//LCD1602_Dis_Str(0, 1, "id:");
	if( PcdAnticoll( Card_SN ) != MI_OK ) //防冲撞，返回卡的序列号 4字节 
	{

		bWarn = 1;
		return FALSE;	
	}

	bPass = 1;
	//send_bytes(Card_type,2);
	//send_bytes(Card_SN,4);
	
	for(i=0;i<4;i++) //卡ID号转化成ASCII
	{
	  if(Card_SN[i]/16>9)	ID_ASC[i*2]=Card_SN[i]/16+'7';
	    else ID_ASC[i*2]=Card_SN[i]/16+'0';

      if(Card_SN[i]%16>9)	ID_ASC[i*2+1]=Card_SN[i]%16+'7';
	    else ID_ASC[i*2+1]=Card_SN[i]%16+'0';
	}
	//display2(3,0,"ID: ",4); //显示卡号
	//display2(3,2,ID_ASC,8);
    //strcpy(ID_ASC_buf,ID_ASC);
    LCD1602_Dis_Str(0, 1, "id:");
    LCD1602_Dis_Str(3, 1, ID_ASC );
    delay(1000);
    LCD1602_Dis_Str(0, 1, "                    " );
//    if( !strcmp(ID_ASC,ID_ASC_buf) )
//            LCD1602_Dis_Str(0, 0, "success");
    
	return TRUE;
}

unsigned char val2func(unsigned char key_value)
{   
    uchar func_value=0;
    switch(key_value)
    {
        case 0:func_value=1;break;
        case 1:func_value=2;break;
        case 2:func_value=3;break;
        case 4:func_value=4;break;
        case 5:func_value=5;break;
        case 6:func_value=6;break;
        case 8:func_value=7;break;
        case 9:func_value=8;break;
        case 10:func_value=9;break;
        case 13:func_value=0;break;
    }
    return func_value;
}

void ctrl_process( void )
{		  
   INT8U i,key_count,key_value=16,table[8]="--------",statesbuf,table_buf[8]="--------";
   bit ok_flag=0,dj_flag=0;
	if(states!=statesbuf)  //状态改变，清屏
	{
        LCD1602_Clear_Screen();
		statesbuf=states;
	}
    switch(states)
	{
       case 0:	          //IC卡读卡输入
		  	  LCD1602_Dis_OneChar( 0, 0, '0');
              LCD1602_Dis_Str( 1, 0, " IDCARD       ");
		      if(IC_READ()) 
			  {
			   
                if ((Card_SN[0]==Card_SN_BUF[0])
			      &&(Card_SN[1]==Card_SN_BUF[1])
				  &&(Card_SN[2]==Card_SN_BUF[2])
				  &&(Card_SN[3]==Card_SN_BUF[3]))
				  {
                    bPass=1;
                    dj_flag=1;
                    states=2;
                    SYN_FrameInfo(2, "[v5][t5]卡片验证成功");
					//relay_ON();//灯开关
					//display(2,0,5);
                    LCD1602_Dis_Str( 0, 1, "success      ");
                    delay(1000);
                    //LCD1602_Dis_Str( 0, 1, "                  ");
			      }
				  else 	 //display(2,0,6);
                  {
                    bWarn=1;
                    LCD1602_Dis_Str( 0, 1, "error       ");
                    SYN_FrameInfo(2, "[v5][t5]卡片验证失败");  
                    delay(1000);
                    //LCD1602_Dis_Str( 0, 1, "                  ");
                  }
				    //relay_OFF();
                  
			   }
			
			  break;

        case 1:		
		       LCD1602_Dis_OneChar( 0, 0, '1');
			   //display2(3,0,table,8);
               LCD1602_Dis_Str( 1, 0, " passdword      ");
			   key_count=0;
               while(1)
			   {  
			      key_value=key_scan();	  
				  if(key_value==12) 
				   { 
				     states--;
	                 return;
					}

				  if(key_value==14)
				  {
				   states++;
				   return;
				  }				 
				  if((key_value>=0 && key_value <3)||(key_value>=4 && key_value <7)||(key_value>=8 && key_value <11)||key_value==13)//有按键输入
				  {	
                    if(key_count<=7)
                    {    
                        table[key_count]=val2func(key_value)+'0';
                        table_buf[key_count++]='*';
                    }   
					//display2(3,0,table,8);
                    LCD1602_Dis_Str( 0, 1, table_buf);
                    LCD1602_Dis_Str( 8, 1, "     ");    
				  }
				  
				  if(key_value==11)//退格
				  {
                    if(key_count>0)
                    {
                        --key_count;
                        table[key_count]='-';
                        table_buf[key_count]='-';
                    }   
					//display2(3,0,table,8);
                    LCD1602_Dis_Str( 0, 1, table_buf);
                    LCD1602_Dis_Str( 8, 1, "     ");
				  }
                  if(key_value==3)
                  {
                    ok_flag=1;
                  }
				  if(key_count==8&&ok_flag==1)
				  {
                      ok_flag=0;
				     if( table[0]==KEY_BUF[0] &&
					     table[1]==KEY_BUF[1] &&
					 	 table[2]==KEY_BUF[2] &&
						 table[3]==KEY_BUF[3] &&
						 table[4]==KEY_BUF[4] &&
						 table[5]==KEY_BUF[5] &&
						 table[6]==KEY_BUF[6] &&
						 table[7]==KEY_BUF[7] )	  //密吗正确 
				
				     { 
					   bPass=1;
                       dj_flag=1;
                       states=2;
                       SYN_FrameInfo(2, "[v5][t5]密码正确 ");  
					   //relay_ON();//灯开关
                       LCD1602_Dis_Str( 0, 1, "                  "); 
                       LCD1602_Dis_Str( 0, 1, "success           `");
                       delay(1000);
                       LCD1602_Dis_Str( 0, 1, "                  ");
					   //relay_OFF();
					   break;				  
					
					 }
					  else 	 //密码错误
					  {	
					    //relay_OFF();
						beep();
						bWarn=1;
					    //display(2,0,6);
                        SYN_FrameInfo(2, "[v5][t5]密码错误");  
                        LCD1602_Dis_Str( 0, 1, "                  "); 
                        LCD1602_Dis_Str( 0, 1, "error");
                        delay(1000);
                        LCD1602_Dis_Str( 0, 1, "                  ");
						break;
					 }	  				
				  }
                  else
                  {
                    ok_flag=0;
                  }

			   }
		       break ;
		  
		 case 2:		
		       //display(1,0,3);      //IC卡登记
		       LCD1602_Dis_OneChar( 0, 0, '2');
               LCD1602_Dis_Str( 1, 0, " set idcard    ");
	           if(IC_READ())  
			   {
					 
			    for(i=0;i<4;i++)
			    Card_SN_BUF[i] = Card_SN[i];
                LCD1602_Dis_Str( 0, 1, "loading.....    ");
				EEPROM_WRITE(0,Card_SN_BUF,4);//写入EEPROM
                delay(200);   
                LCD1602_Dis_Str( 0, 1, "ok              ");
                SYN_FrameInfo(2, "[v5][t5]卡片已更新");   
                delay(2000);
                LCD1602_Dis_Str( 0, 1, "             ");
			   }
		       break ;

		 case 3:		
//		       display(1,0,4);      //密码设置
//			   display2(3,0,table,8);
               LCD1602_Dis_OneChar( 0, 0, '3');
               LCD1602_Dis_Str( 1, 0, " set passdword    ");
			   key_count=0;
			   while(1)
			   {  
			      key_value=key_scan();	  
				  if(key_value==12) 
				   { 
				     states--;
	                 return;
					}
			 
			      if((key_value>=0 && key_value <3)||(key_value>=4 && key_value <7)||(key_value>=8 && key_value <11)||key_value==13)//有按键输入
				  {		  				    				    
					table[key_count++]=val2func(key_value)+'0';
					//display2(3,0,table,8);
                    LCD1602_Dis_Str( 0, 1, table);
                    LCD1602_Dis_Str( 8, 1, "        "); 
				  }
				  
				  if(key_value==11)//退格
				  {								    
					table[--key_count]='-';
					//display2(3,0,table,8);
                    LCD1602_Dis_Str( 0, 1, table);
                    LCD1602_Dis_Str( 8, 1, "        ");     
				  }
                  if(key_value==3)
                  {
                    ok_flag=1;
                  }
				   if(key_count==8 && ok_flag==1) //按下确定键
				  
				  { 
                    ok_flag=0;
				    for(i=0;i<8;i++)
				     KEY_BUF[i]=table[i];
                    LCD1602_Dis_Str( 0, 1, "loading         ");  
				   	EEPROM_WRITE(7,KEY_BUF,8);//写入EEPROM
                    SYN_FrameInfo(2, "[v5][t5]密码更改成功");  
                    LCD1602_Dis_Str( 0, 1, "finish         ");
                    delay(1000);
                    LCD1602_Dis_Str( 0, 1, "               ");
				   break;
				   }
                  else ok_flag=0;
                } 			 
				
		       break ;

		default : break;

	 }	  		

	if( bPass )		 //处理成功
	{
		bPass = 0;
		pass();
        LCD1602_Dis_Str( 0, 1, "                  ");        
	
	}

	if( bWarn )	     //处理失败
	{
		bWarn = 0;
		warn();
        LCD1602_Dis_Str( 0, 1, "                  ");
	}
    if(dj_flag==1)
    {

        dj_flag=0;
        LCD1602_Dis_Str( 0, 1, "                  ");
        duoji_openDoor();
        SYN_FrameInfo(2, "[v5][t5]门已开");
        LCD1602_Dis_Str( 0, 1, "opening door");
//        duoji_start(15);
//        duoji_on();
//        delay1s();
//        duoji_off();
        delay(200);
        LCD1602_Dis_Str( 0, 1, "finish        ");
        delay(200);

        LCD1602_Dis_Str( 0, 1, "                  ");
    }
    
}


void main()
{
    INT8U key;
    
   
   
    Delay_ms(50); //让硬件稳定
	init_all();	   //执行初始化函数
    //duoji_Init();
    //duoji_openDoor();
	beep();	   //beep test 	
    LCD1602_Dis_Str( 0, 0, "welcome mimasuo" );	 //显示初始化
    delay(1000);
    LCD1602_Clear_Screen();

    delay(100);
    while(1)
    {
        key=key_scan(); //按键操作
	   if(key==12)  if(states>0) states--;
	                else states=0;	//上一功能
	   
	   if(key==14) if(++states>3) states=3;	//下一功能
	   ctrl_process(); //进入RC522操作

        
    }
}
    
        

