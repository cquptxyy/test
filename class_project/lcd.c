#include"lcd.h"
#include"delay.h"
void Lcd1602_Write_Cmd(unsigned char cmd)//д������
{
	Read_Busy();
	LCD1602_RS=0;      
	LCD1602_RW=0;
	LCD1602_DB=cmd;
	delay1ms(10);
	LCD1602_EN=1;
	delay1ms(10);
	LCD1602_EN=0;
}
void Lcd1602_Write_Data(unsigned char dat)//д������
{
	Read_Busy();
	LCD1602_RS=1;
	LCD1602_RW=0;
	LCD1602_DB=dat;
	delay1ms(10);
	LCD1602_EN=1;
	delay1ms(10);
	LCD1602_EN=0;
}
void InitLcd1602()//1602��ʼ��
{
	Lcd1602_Write_Cmd(0x38);//��5*8��8λ����
	Lcd1602_Write_Cmd(0x0c);//����ʾ������ʾ���
	Lcd1602_Write_Cmd(0x06);//��ַ��1��д������ʱ�������
	Lcd1602_Write_Cmd(0x01);//����
}

//æ��⺯�����ж�bit7��0������ִ�У�1��ֹ
void Read_Busy()
{
	uchar sta;
	LCD1602_DB=0xff;
	LCD1602_RS=0;
	LCD1602_RW=1;
	do
	{
		LCD1602_EN=1;
		sta=LCD1602_DB;//P0��
		LCD1602_EN=0;//ʹ�ܡ���������ͣ��ͷ�����
	}while(sta &0x80);
}
//��ʾ�ַ���
void ShowStr(uchar *str)
{
	while(*str!='\0')
	{
		Lcd1602_Write_Data(*str++);
	}
}
void display(uchar str[])
{
	Lcd1602_Write_Cmd(0x80);//ָ���׵�ַ
	ShowStr(str);//��ʾ�ַ���
}
void display_1(uchar str[])
{
	Lcd1602_Write_Cmd(0xc0);//ָ���׵�ַ
	ShowStr(str);//��ʾ�ַ���
}
