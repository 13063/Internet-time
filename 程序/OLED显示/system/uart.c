#include <STC89C5xRC.H>
#include "public.h"

void UART_Init(void)		//9600bps@11.0592MHz
{
	PCON &= 0x7F;		//波特率不倍速
	SCON = 0x50;		//8位数据,可变波特率
	TMOD &= 0x0F;		//设置定时器模式
	TMOD |= 0x20;		//设置定时器模式
	TL1 = 0xFD;			//设置定时初始值
	TH1 = 0xFD;			//设置定时重载值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
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