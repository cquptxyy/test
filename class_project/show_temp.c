#include<reg51.h>
#include"lcd.h"
#include"DS18B20.h"
#include"show_temp.h"
#include"delay.h"
sbit beep=P1^5;
sbit PWM=P1^0;
unsigned int num[4],temp;
float tmp,x;
unsigned char sign;
unsigned char receiveData[2];

void BEEP()//�¶ȱ���
{
	if(tmp-x<1e-7)
		PWM=0;//PWM=1ת��
	else
	{
		PWM=1;
		beep=1;
		delay1ms(1);
		beep=0;
		delay1ms(1);
	}
}
void DISPLAY()//��ֵ�¶���ʾ��LCD
{
	unsigned char i;
	Lcd1602_Write_Cmd(0x80+11);
	for(i=0;i<2;i++)
	  Lcd1602_Write_Data(receiveData[i]);
	/* ��ʾ��*/
    Lcd1602_Write_Cmd(0x80+13);
	Lcd1602_Write_Data(0xdf);
	Lcd1602_Write_Cmd(0x80+14); 
	Lcd1602_Write_Data('C');

}
float displayTemperature()//��LcD����ʾ�¶�
{
	unsigned int i;
	float f;
	if(temp<0)
		tmp=(~temp+1)*0.0625; 
	else
		tmp=temp*0.0625; 
	num[0]=(int)tmp/10;
	num[1]=(int)tmp%10;
	f=(float)(tmp-(num[0]*10+num[1]));
	i=(int)(f*100);
	num[2]=i/10;
	num[3]=i%10;	   
	/*��ʾʮλ*/
	Lcd1602_Write_Cmd(0x80+0x40);
	Lcd1602_Write_Data(num[0]+0x30);
	/*��ʾ��λ*/
	Lcd1602_Write_Cmd(0x80+0x40+1);
  Lcd1602_Write_Data(num[1]+0x30);
	/*��ʾС����*/
	Lcd1602_Write_Cmd(0x80+0x40+2);
	Lcd1602_Write_Data('.');
	/*��ʾС�����һλ*/
	Lcd1602_Write_Cmd(0x80+0x40+3);
	Lcd1602_Write_Data(num[2]+0x30);
	/*��ʾС�������λ*/
	Lcd1602_Write_Cmd(0x80+0x40+4);
	Lcd1602_Write_Data(num[3]+0x30);
	/* ��ʾ��*/
  Lcd1602_Write_Cmd(0x80+0x40+5);
	Lcd1602_Write_Data(0xdf);
	Lcd1602_Write_Cmd(0x80+0x40+6); 
	Lcd1602_Write_Data('C');
	return tmp;			  
}

void send()//��PC�˷����¶�
{
	unsigned int i;
  ES=0;
	for(i=0;i<2;i++)
	{
		SBUF=num[i]+0x30; //������
		while(!TI);
		TI=0;//�����־λ
	}
	SBUF='.'; //������
  while(!TI);
  TI=0;//�����־λ
	for(i=2;i<4;i++)
	{
		SBUF=num[i]+0x30; //������
  	while(!TI);
		TI=0;//�����־λ
	}
	SBUF=' '; //���Ϳո񣬽��м��
  while(!TI);
  TI=0;//�����־λ
	ES=1;
}
void UsartConfiguration()
{
	  SCON=0X50; //����Ϊ������ʽ1
    PCON=0X00; //�����ʲ�����
    TMOD=0X20; //���ü�����������ʽ2
    TL1=0xfA;//������4800
    TH1=0xfA;
    TR1=1; //����������
    EA=1; //���ж�
    ES=1;
}
void show_init()
{
	unsigned char code name[10]="0303/0304:";
	unsigned int i;
	InitLcd1602();
	UsartConfiguration();//����ͨ�ų�ʼ��
	Lcd1602_Write_Cmd (0x80);
	for(i=0;i<10;i++)
	Lcd1602_Write_Data (name[i]);
}
	
void show_temp()
{
	unsigned char i;
	temp=Ds18b20ReadTemp();//��ȡDS18B20���¶�
	tmp=displayTemperature();//�����¶�ת��
	send();
	(float)x=(receiveData[0]-0x30)*10+receiveData[1]-0x30;
	if(sign)
		{
			DISPLAY();
			for(i=0;i<50;i++)
			{
				BEEP();
			}
		}
}
/*void show()
{
	unsigned char i;
	UsartConfiguration();//����ͨ�ų�ʼ��
	temp=Ds18b20ReadTemp();//��ȡDS18B20���¶�
	tmp=displayTemperature();//�����¶�ת��
	send();
}*/
