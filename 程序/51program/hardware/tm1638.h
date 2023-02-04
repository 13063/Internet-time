#ifndef TM1638_H
#define TM1638_H
#include "public.h"

sbit STB=P1^7;
sbit CLK=P1^6;
sbit DIO=P1^5;


void TM1638_ShowNum(uchar pos,uchar num);				//ָ��λ��(1~8)д����
void TM1638_ShowTime(uchar hour,uchar min,uchar sec);	
void TM1638_Init();
void TM1638_SetLight(uchar light);						//0~7������
void TM1638_ShowNumber(unsigned long number);


#endif