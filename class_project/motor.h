#ifndef _MOTOR_H_
#define _MOTOR_H_
#include<reg51.h>
sbit PWM=P1^0; //����ʹ�õ�IO��//
sbit K3=P3^2;
extern unsigned int count_motor;
extern unsigned int timer;
void Motor_Init();
void zhuandong();

#endif