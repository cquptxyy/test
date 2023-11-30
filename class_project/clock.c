#include"lcd.h"
#include"clock.h"
#include"delay.h"
#include"show_temp.h"
#include"DS18B20.h"
uchar time_str[]="The time is";
uchar lcd_clock[]={0x30,0x30,0x3a,0x30,0x30,0x3a,0x30,0x30};
sbit K1=P3^1;
sbit K2=P3^0;
sbit K3=P3^2;

static uint second,min,hour;
//��ʱ���жϳ�ʼ��
void ExitInit()
{
	TMOD=0x01;//TMOD���ã�0x01Ϊ���ܸ��Ŷ�ʱ��
	EA=1;//���ж�����
	ET0=1;//��ʱ���ж�����
	TR0=1;//�жϿ��ƿ��أ���ʱ��0��ʼ����
	TH0=0xFC;//���ó�ֵ64535/256 
	TL0=0x18;//64535%256
	P0=0x00;
}
//�ⲿ�жϳ�ʼ��
void ExitInit_isr()
{
	EA=1;
	EX0=1;
	IT0=1;//���ش���
	EX1=1;
	IT1=0;//��ƽ����
}

void show_clock()
{
	unsigned char i;
	//unsigned char code name[]="0303/0304:";
	InitLcd1602();
	ExitInit();
	ExitInit_isr();
	Lcd1602_Write_Cmd(0x80);//ָ���׵�ַ
	ShowStr(time_str);//��ʾ�ַ���
	while(1)
	{
		if(EA==1)
		{
		for(i=0;i<8;i++)
		{
			Lcd1602_Write_Cmd(0xc4+i);
			Lcd1602_Write_Data(lcd_clock[i]);
		}
		
    }
		else
		{
			InitLcd1602();
			K4=1;
			show_init();
			break;
		}
	}
	return;
}


//�ⲿ�жϷ������
void int0_isr(void) interrupt 0
{
	
	while(!TR0)
	{
		if(K2==0)
		{
			delay1ms(50);
			if(K2==0)
			{
				hour++;
				if(hour==24)
					{
						hour=0;
					}
					Lcd1602_Write_Cmd(0xc4+0);
			    lcd_clock[0]=hour/10+0x30;
			    Lcd1602_Write_Data(lcd_clock[0]);
			    Lcd1602_Write_Cmd(0xc4+1);
	        lcd_clock[1]=hour%10+0x30;
			    Lcd1602_Write_Data(lcd_clock[1]);
			}
		}
		if(K3==0)
			{
				delay1ms(50);
			  if(K3==0)
					{
						min++;
			      if(min==60)
							{
					 			min=0;
							}
							Lcd1602_Write_Cmd(0xc4+3);
			        lcd_clock[3]=min/10+0x30;
			        Lcd1602_Write_Data(lcd_clock[3]);
			        Lcd1602_Write_Cmd(0xc4+4);
	            lcd_clock[4]=min%10+0x30;
		         	Lcd1602_Write_Data(lcd_clock[4]);
					}
			}
			if(K4==0)
		{
			TR0=1;
		}
		
	}
	if(K3==0)
	{
		EA=0;
	}
}

void int1_isr(void) interrupt 2
{
	if(K4==0)
	{
		TR0=1;
	}
}

//��ʱ���жϷ������
void Timer0() interrupt 1
{
	static uint timer;
	TH0=0xFC;//����ʱ������ֵ
	TL0=0x18;
	
	ES=1;
	
	if(K1==0)
	{
		TR0=0;
		K3=1;
		K3=0;
		K3=1;
	}

	timer++;
	if(timer==1000)//�ж��Ƿ񵽴�1s
	{
		timer=0;
		second++;
		if(second==60)
	  {
		second=0;
		min++;
			if(min==60)
	    {
		  min=0;
		  hour++;
				if(hour==24)
	      {
		     second=0;
		     min=0;
		     hour=0;
	      } 
	   }
	  }
	}
	lcd_clock[0]=hour/10+0x30;
	lcd_clock[1]=hour%10+0x30;
	lcd_clock[3]=min/10+0x30;
	lcd_clock[4]=min%10+0x30;
	lcd_clock[6]=second/10+0x30;
	lcd_clock[7]=second%10+0x30;
}