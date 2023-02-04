#include "tm1638.h"

//共18个，其中第17个为-，第后位不显示
unsigned char code tab[]=
	{0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,
    0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x40,0x00};


void TM1638_WriteByte(uchar	DATA)			//写数据函数
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		
		if(DATA&0X01)
			DIO=1;
		else
			DIO=0;
		DATA>>=1;
		CLK=0;
		CLK=1;								//产生一个上升沿
	}
}

void TM1638_COM(unsigned char cmd)			//发送命令字
{
	STB=0;
	TM1638_WriteByte(cmd);
	STB=1;
}


void TM1638_Init()
{
	uchar i;
	TM1638_COM(0x8b);       				//默认中间挡
	TM1638_COM(0x40);       				//采用地址自动加1
	STB=0;		           
	TM1638_WriteByte(0xc0);    				//设置起始地址

	for(i=0;i<16;i++)	   					//传送16个字节的数据
		TM1638_WriteByte(0x00);
	STB=1;
}

void TM1638_SetLight(uchar light)			//亮度 (0x88-0x8f)8级亮度可调
{
	TM1638_COM(0x88+light);
}




void TM1638_ShowNum(unsigned char pos,unsigned char DATA)	//指定地址写入数据
{
	
	pos-=1;
	TM1638_COM(0x44);
	STB=0;
	TM1638_WriteByte(0xc0+pos*2);
	TM1638_WriteByte(tab[DATA]);
	STB=1;
}

void TM1638_ShowTime(uchar hour,uchar min,uchar sec) 		//显示时间
{
	TM1638_ShowNum(1,hour/10);
	TM1638_ShowNum(2,hour%10);
	
	TM1638_ShowNum(3,16);
	
	TM1638_ShowNum(4,min/10);
	TM1638_ShowNum(5,min%10);
	
	TM1638_ShowNum(6,16);
	
	TM1638_ShowNum(7,sec/10);
	TM1638_ShowNum(8,sec%10);
}

void TM1638_ShowNumber(unsigned long number)				//右对齐显示一个8位的十进制数
{

	unsigned long int f=1;
	unsigned long int i=1;
	uchar len=0;

	if(number==0)
		len=1;
	while((number/f)!=0)
	{
		f=f*10;
		len++;
	}
	for(f=1;f<=8;f++)
	{
		if(f<=len)
		{
			TM1638_ShowNum(9-f,(number/i)%10);
			i=i*10;
		}
		else
			TM1638_ShowNum(9-f,17);
		
	}
	
}



