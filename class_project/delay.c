#include"delay.h"
void delay1ms(unsigned int t)//���0us
{
	unsigned int i,j;
	for(i=0;i<t;i++)
	{
		for(j=0;j<120;j++);
	}
}