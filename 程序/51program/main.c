#include <STC89C5xRC.H>
#include <tm1638.h>
#include <ssd1306.h>
#include <delay.h>
#include "uart.h"

			
uchar state;					//�������ݰ�״̬��
uchar len;						//�������ݰ�����
uchar NetTimeBuffer[15];		//�������ݰ�����

struct systime					//��������ʱ��ṹ��
{
	uchar hour;
	uchar min;
	uchar sec;
	uchar year;
	uchar mon;
	uchar date;
} netTime;



void main()
{
	UART_Init();
	TM1638_Init();												//��ʼ�������ģ��
	TM1638_SetLight(0);
	
	OLED_Init();
	OLED_Clear();
	
	while(1)
	{
		netTime.hour=(NetTimeBuffer[9]-'0')*10+(NetTimeBuffer[10]-'0');
		netTime.min=(NetTimeBuffer[11]-'0')*10+(NetTimeBuffer[12]-'0');
		netTime.sec=(NetTimeBuffer[13]-'0')*10+(NetTimeBuffer[14]-'0');
		
		netTime.year=(NetTimeBuffer[3]-'0')*10+(NetTimeBuffer[4]-'0');
		netTime.mon=(NetTimeBuffer[5]-'0')*10+(NetTimeBuffer[6]-'0');
		netTime.date=(NetTimeBuffer[7]-'0')*10+(NetTimeBuffer[8]-'0');
		
		OLED_ShowChar(1,1,'0'+netTime.hour/10,16);
		OLED_ShowChar(9,1,'0'+netTime.hour%10,16);
		OLED_ShowChar(17,1,':',16);
		OLED_ShowChar(25,1,'0'+netTime.min/10,16);
		OLED_ShowChar(33,1,'0'+netTime.min%10,16);
		OLED_ShowChar(41,1,':',16);
		OLED_ShowChar(49,1,'0'+netTime.sec/10,16);
		OLED_ShowChar(57,1,'0'+netTime.sec%10,16);

		TM1638_ShowTime(netTime.hour,netTime.min,netTime.sec);
	}
}

void UART_IT() interrupt 4
{
	uchar buf;
	if(RI==1)
	{
		buf=SBUF;
		if(buf==0x40)
		{
			state=1;					//���յ���ͷ,��״̬1,��¼14��ʱ������0D
			len=15;
			P0=0xfe;
		}
		if(state==1&&len>0)
		{
			NetTimeBuffer[15-len]=buf;	//�����ܵ������ݴ���NetTime����
			len--;
		}
		if(buf==0x0A)
		{
			state=2;					//���ܵ���β��״̬2
			len=0;
			P0=0xff;
		}
	}
	RI=0;
}