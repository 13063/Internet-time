#ifndef _SSD1306_H
#define _SSD1306_H
#include <stdlib.h>
#include <string.h>
#include "public.h"

#define OLED_CMD  0X00 //Ð´ÃüÁî
#define OLED_DATA 0X40 //Ð´Êý¾Ý

#define SSD1306_I2C_ADDR         0x78
#define SSD1306_WIDTH            128
#define SSD1306_HEIGHT           6

#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	

void OLED_Init(void);
void OLED_ShowChar(uchar x,uchar y,uchar chr,uchar Char_Size);
void OLED_Clear(void);
void OLED_ShowString(uchar x,uchar y,uchar *chr,uchar Char_Size);
void OLED_ShowNum(uchar x,uchar y,uint num,uchar len,uchar size2);
#endif