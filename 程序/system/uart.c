#include <STC89C5xRC.H>
#include "public.h"

void UART_Init(void)		//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//�����ʲ�����
	SCON = 0x50;		//8λ����,�ɱ䲨����
	TMOD &= 0x0F;		//���ö�ʱ��ģʽ
	TMOD |= 0x20;		//���ö�ʱ��ģʽ
	TL1 = 0xFD;			//���ö�ʱ��ʼֵ
	TH1 = 0xFD;			//���ö�ʱ����ֵ
	ET1 = 0;			//��ֹ��ʱ���ж�
	TR1 = 1;			//��ʱ��1��ʼ��ʱ
	EA=1;
	ES=1;
}


void UART_Send(uchar byte)
{
	SBUF=byte;
	while(TI==0);
	TI=0;
}

/*
void UART_IT() interrupt 4
{
	if(RI==1)
	{
		P0=~SBUF;
		UART_Send(SBUF);
		RI=0;
	}
	
}
*/