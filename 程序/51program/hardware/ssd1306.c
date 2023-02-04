#include <ssd1306.H>
#include <iic.H>
#include <f6x8.H>


/* Absolute value */
#define ABS(x)   ((x) > 0 ? (x) : -(x))

static uchar SSD1306_Buffer[SSD1306_WIDTH * SSD1306_HEIGHT / 8];
/* Private SSD1306 structure */
typedef struct {
	uint CurrentX;
	uint CurrentY;
	uchar Inverted;
	uchar Initialized;
} SSD1306_t;

/* Private variable */
 SSD1306_t SSD1306;




void SSD1306_WriteByte(uchar dat, uchar cmd)
{
	iicStart();
	iicSendData(SSD1306_I2C_ADDR);
	iicWaitAck();
	iicSendData(cmd);
	iicWaitAck();
	iicSendData(dat);
	iicWaitAck();
	iicStop();
}

void OLED_Clear(void)  
{  
	uchar i,n;		    
	for(i=0;i<8;i++)  
	{  
		SSD1306_WriteByte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		SSD1306_WriteByte (0x00,OLED_CMD);      //设置显示位置—列低地址
		SSD1306_WriteByte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)SSD1306_WriteByte(0,OLED_DATA); //write 0x00;
	} //更新显示
}

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	SSD1306_WriteByte(0xb0+y,OLED_CMD);						//page y
	SSD1306_WriteByte(((x&0xf0)>>4)|0x10,OLED_CMD);//high 4 address
	SSD1306_WriteByte((x&0x0f),OLED_CMD); 					//low 4 address
} 

void OLED_ShowChar(uchar x,uchar y,uchar chr,uchar Char_Size)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(Char_Size ==16)
			{
				OLED_Set_Pos(x,y);	
				for(i=0;i<8;i++)
				SSD1306_WriteByte(F8X16[c*16+i],OLED_DATA);
				OLED_Set_Pos(x,y+1);
				for(i=0;i<8;i++)
				SSD1306_WriteByte(F8X16[c*16+i+8],OLED_DATA);
			}
		else 
			{	
				OLED_Set_Pos(x,y);
				for(i=0;i<6;i++)
				SSD1306_WriteByte(F6x8[c][i],OLED_DATA);
			}
}




void SSD1306_init()
{	
  delay(100);
  SSD1306_WriteByte(0xAE,OLED_CMD); //display off
  SSD1306_WriteByte(0x20,OLED_CMD); //Set Memory Addressing Mode   
  SSD1306_WriteByte(0x10,OLED_CMD); //00,Horizontal Addressing Mode;01,Vertical Addressing Mode;10,Page Addressing Mode (RESET);11,Invalid
  SSD1306_WriteByte(0xB0,OLED_CMD); //Set Page Start Address for Page Addressing Mode,0-7
  SSD1306_WriteByte(0xC1,OLED_CMD); //Set COM Output Scan Direction
  SSD1306_WriteByte(0x00,OLED_CMD); //---set low column address
  SSD1306_WriteByte(0x10,OLED_CMD); //---set high column address
  SSD1306_WriteByte(0x40,OLED_CMD); //--set start line address
  SSD1306_WriteByte(0x81,OLED_CMD); //--set contrast control register
  SSD1306_WriteByte(0xFF,OLED_CMD);
  SSD1306_WriteByte(0xA1,OLED_CMD); //--set segment re-map 0 to 127 0xa0,if upside down
  SSD1306_WriteByte(0xC8,OLED_CMD); //--set segment re-map 0 to 127 0xc0,if upside down
  SSD1306_WriteByte(0xA6,OLED_CMD); //--set normal display
  SSD1306_WriteByte(0xA8,OLED_CMD); //--set multiplex ratio(1 to 64)
  SSD1306_WriteByte(0x3F,OLED_CMD); //
  SSD1306_WriteByte(0xA4,OLED_CMD); //0xa4,Output follows RAM content;0xa5,Output ignores RAM content
  SSD1306_WriteByte(0xD3,OLED_CMD); //-set display offset
  SSD1306_WriteByte(0x00,OLED_CMD); //-not offset
  SSD1306_WriteByte(0xD5,OLED_CMD); //--set display clock divide ratio/oscillator frequency
  SSD1306_WriteByte(0xF0,OLED_CMD); //--set divide ratio
  SSD1306_WriteByte(0xD9,OLED_CMD); //--set pre-charge period
  SSD1306_WriteByte(0x22,OLED_CMD); //
  SSD1306_WriteByte(0xDA,OLED_CMD); //--set com pins hardware configuration
  SSD1306_WriteByte(0x12,OLED_CMD);
  SSD1306_WriteByte(0xDB,OLED_CMD); //--set vcomh
  SSD1306_WriteByte(0x20,OLED_CMD); //0x20,0.77xVcc
  SSD1306_WriteByte(0x8D,OLED_CMD); //--set DC-DC enable
  SSD1306_WriteByte(0x14,OLED_CMD); //
  SSD1306_WriteByte(0xAF,OLED_CMD); //--turn on SSD1306 panel
}


void SSD1306_ON(void) {
	SSD1306_WriteByte(0x8D,OLED_CMD);  
	SSD1306_WriteByte(0x14,OLED_CMD);  
	SSD1306_WriteByte(0xAF,OLED_CMD);  
}
void SSD1306_OFF(void) {
	SSD1306_WriteByte(0x8D,OLED_CMD);  
	SSD1306_WriteByte(0x10,OLED_CMD);
	SSD1306_WriteByte(0xAE,OLED_CMD);  
}


uint oled_pow(uchar m,uchar n)
{
	uint result=1;	 
	while(n--)result*=m;    
	return result;
}			

//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(uchar x,uchar y,uint num,uchar len,uchar size2)
{         	
	uchar t,temp;
	uchar enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 

//显示一个字符号串
void OLED_ShowString(uchar x,uchar y,uchar *chr,uchar Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

//显示汉字
void OLED_ShowCHinese(uchar x,uchar y,uchar no)
{      			    
	uchar t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				SSD1306_WriteByte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				SSD1306_WriteByte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	SSD1306_WriteByte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 








void OLED_Init(void)
{ 	
 
	SSD1306_WriteByte(0xAE,OLED_CMD);//--display off
	
	SSD1306_WriteByte(0x00,OLED_CMD);//---set low column address
	SSD1306_WriteByte(0x10,OLED_CMD);//---set high column address
	SSD1306_WriteByte(0x40,OLED_CMD);//--set start line address  
	SSD1306_WriteByte(0xB0,OLED_CMD);//--set page address
	
	SSD1306_WriteByte(0x81,OLED_CMD); // contract control对比度
	SSD1306_WriteByte(0xFF,OLED_CMD);//--128 
  
	SSD1306_WriteByte(0xA1,OLED_CMD);//set segment remap 
	SSD1306_WriteByte(0xA6,OLED_CMD);//--normal / reverse正常或逆向
	
	SSD1306_WriteByte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	SSD1306_WriteByte(0x3F,OLED_CMD);//--1/32 duty
	
	SSD1306_WriteByte(0xC8,OLED_CMD);//Com scan direction
	
	SSD1306_WriteByte(0xD3,OLED_CMD);//-set display offset
	SSD1306_WriteByte(0x00,OLED_CMD);//no offset
	
	SSD1306_WriteByte(0xD5,OLED_CMD);//set osc division
	SSD1306_WriteByte(0x80,OLED_CMD);//default
	
	SSD1306_WriteByte(0xD8,OLED_CMD);//set area color mode off
	SSD1306_WriteByte(0x05,OLED_CMD);//
	
	SSD1306_WriteByte(0xD9,OLED_CMD);//Set Pre-Charge Period
	SSD1306_WriteByte(0xF1,OLED_CMD);//
	
	SSD1306_WriteByte(0xDA,OLED_CMD);//set com pin configuartion
	SSD1306_WriteByte(0x12,OLED_CMD);//
	
	SSD1306_WriteByte(0xDB,OLED_CMD);//set Vcomh可调亮度
	SSD1306_WriteByte(0x40,OLED_CMD);//default 
	
	SSD1306_WriteByte(0x8D,OLED_CMD);//set charge pump enable电荷泵
	SSD1306_WriteByte(0x14,OLED_CMD);//display on
	
	SSD1306_WriteByte(0xAF,OLED_CMD);//--turn on oled panel
} 


