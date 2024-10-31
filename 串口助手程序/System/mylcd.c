
#include "mylcd.h"
const u16 X[30]={
0, 8*1, 8*2, 8*3, 8*4, 8*5, 8*6, 8*7, 8*8, 8*9, 
8*10, 8*11, 8*12, 8*13, 8*14, 8*15, 8*16, 8*17, 8*18, 8*19, 
8*20, 8*21, 8*22, 8*23, 8*24, 8*25, 8*26, 8*27, 8*28, 8*29
};
const u16 Y[20]={
0, 16*1, 16*2, 16*3, 16*4, 16*5, 16*6, 16*7, 16*8, 16*9, 
16*10, 16*11, 16*12, 16*13, 16*14, 16*15, 16*16, 16*17, 16*18, 16*19
};
void myLCD_Init(void)
{
	SystemInit();//初始化RCC 设置系统主频为72MHZ
	delay_init(72);	     //延时初始化
	LCD_Init();	   //液晶屏初始化
}

void myLCD_Power_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
void myLCD_Power_Off(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

void myLCD_Power_On(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
}
void myLCD_ShowString(u16 x, u16 y, u8 *str)
{
//	Show_Str(x,y,BLACK,WHITE, str,16,1);
	LCD_ShowString(x,y,16,str,0);
}
void myLCD_ShowChar(u16 x,u16 y,u8 num)
{
	LCD_ShowChar(x,y,POINT_COLOR,BACK_COLOR, num,16,0);
}
//显示十六进制
void myLCD_ShowHexNum(u16 x,u16 y, u8 num)
{
	u8 hex = num/16%16;
	if(hex < 10)
	{
		LCD_ShowChar(x,y,POINT_COLOR,BACK_COLOR,hex+'0',16,0);
	}
	else
	{
		LCD_ShowChar(x,y,POINT_COLOR,BACK_COLOR,hex-10+'A',16,0);
	}
	x+=8;
	if(x>=240)
	{
		x = x%240;
		y += 16;
		if(y>=304)
		{
			y = y%304;
		}
	}

	hex = num/1%16;
	if(hex < 10)
	{
		LCD_ShowChar(x,y,POINT_COLOR,BACK_COLOR,hex+'0',16,0);
	}
	else
	{
		LCD_ShowChar(x,y,POINT_COLOR,BACK_COLOR,hex-10+'A',16,0);
	}
}
