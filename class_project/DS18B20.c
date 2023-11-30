#include"DS18B20.h"
sbit DSIO=P3^7;
void Delayms1(unsigned int n)
{
	unsigned int i,j;
  for(j=n;j>0;j--)
  for(i=112;i>0;i--);
}
unsigned char Ds18b20Init()
{
	unsigned int i;
  DSIO=0; 
  i=80;
	while(i--);//�����͵�ƽ
	DSIO=1;//����
	i=0;
	while(DSIO) //����Ƿ����DS18B20Ӧ��͵�ƽ�ź�
{
	i++;
	if(i>50000)//�ȴ�>50MS
		return 0;
}
return 1;
}

void Ds18b20WriteByte(unsigned char dat)
{
	unsigned int i,j;
  for(j=0;j<8;j++)
  {
		DSIO=0; //����������1us
    i++;
    DSIO=dat&0x01; //�����λ��ʼ����
    i=6;
    while(i--); //��ʱ60us���ϣ���DS18B20��ȡ���������Ϣ
    DSIO=1; //�ͷ�����
    dat>>=1;//��λ������д��
		}
}

unsigned char Ds18b20ReadByte()
{
	unsigned char byte,bi;
  unsigned int i,j;
  for(j=8;j>0;j--)
	{
		DSIO=0;//����1us
    i++;
    DSIO=1;//��DS18B20�����ݴ���������
    i++;
    i++;//��ʱ6us�ȴ������ȶ�
    bi=DSIO; //�����λ��ʼ��ȡ����
    byte=(byte>>1)|(bi<<7); //byte����1λ��bi������λ����������
    i=4; 
    while(i--);//��ȡ���ȴ�48us�ٶ�ȡ��һλ
		}
	return byte;
}

void Ds18b20ChangTemp()//д���¶�ת��ָ��
{
	Ds18b20Init();//DS18B20��ʼ��
  Delayms1(1);
  Ds18b20WriteByte(0xcc); //����ROM
  Ds18b20WriteByte(0x44); //�¶�ת������
  Delayms1(100);
}

void Ds18b20ReadTempCom()//д���ȡ�¶�ָ��
{
	Ds18b20Init();//DS18B20��ʼ��
  Delayms1(1);
  Ds18b20WriteByte(0xcc); //����ROM
  Ds18b20WriteByte(0xbe); //���Ͷ�ȡ�¶�����
}

int Ds18b20ReadTemp()//��ȡ�¶�
{
	unsigned int temp;
  unsigned char tmh,tml;
  Ds18b20ChangTemp(); //д��ת��ָ��
  Ds18b20ReadTempCom();//д���ȡ�¶�ָ��
  tml=Ds18b20ReadByte(); //��ȡ�¶ȣ�16λ���Ͱ�λ
  tmh=Ds18b20ReadByte(); //�߰�λ
  temp=tmh;
  temp<<=8; //
  temp|=tml; //
  return temp;
}
