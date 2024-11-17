#include "delay.h"

void myKey_Init(void)
{	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//使用PB3和PB4要关闭JTAG功能
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	//初始化外部时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//创建结构体
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//初始化GPIO口
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t myKey_GetNum(void)
{
	uint8_t KeyNum = 0;
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0)
	{
		delay_ms(10);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3) == 0);
		delay_ms(10);
		KeyNum = 1;
	}
	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0)
	{
		delay_ms(10);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4) == 0);
		delay_ms(10);
		KeyNum = 2;
	}
	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0)
	{
		delay_ms(10);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5) == 0);
		delay_ms(10);
		KeyNum = 3;
	}
	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0)
	{
		delay_ms(10);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6) == 0);
		delay_ms(10);
		KeyNum = 4;
	}
	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 0)
	{
		delay_ms(10);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7) == 0);
		delay_ms(10);
		KeyNum = 5;
	}
	
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0)
	{
		delay_ms(10);
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_8) == 0);
		delay_ms(10);
		KeyNum = 6;
	}
	return KeyNum;
}

