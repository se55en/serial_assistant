#include "stm32f10x.h"                  // Device header

void DriverScreen_Init(void)
{	//初始化外部时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//创建结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//初始化GPIO口
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void LCD_Control_Off(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}

void LCD_Control_On(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_0);
}

