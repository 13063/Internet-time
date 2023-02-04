#include "iic.h"

void iicStart()
{
	SCL=1;
	SDA=1;
	//Delay5us();
	SDA=0;
	//Delay5us();
}

void iicStop()
{
	SCL=1;
	SDA=0;
	//Delay5us();
	SDA=1;
	//Delay5us();
}

void iicSendData(uchar dat)
{
	uint i;
	SCL=0;
	for(i=0;i<8;i++)
	{
		SDA=dat&0x80?1:0;
		SCL=1;
		_nop_();
		SCL=0;
		dat=dat<<1;
	}
}
void iicWaitAck()
{
	SCL=1;
	SCL=0;
}

