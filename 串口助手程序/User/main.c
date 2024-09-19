#include "stm32f10x.h"                  // Device header
#include "Serial.h"
#include "delay.h"
#include "DRIVER_SCREEN.h"
#include "PC13LED.h"

#include "sys.h"
#include "delay.h"
#include "gui.h"
#include "test.h"
#include "lcd.h"

#include "MY_KEY.h"
/*

串口1---1号数据线
串口3---2号数据线

*/

#define Recv_Max_Len 1024//一次接收数据最大长度

uint8_t Data;
uint16_t USART3_RecvBuffLen = 0;//串口3接收数据长度
u8 USART3_RecvBuff[Recv_Max_Len] = {0};//串口1接收缓存区
u8 USART3_Flag = 0;//串口3接收完成标志位
uint16_t USART1_RecvBuffLen = 0;//串口1接收数据长度
u8 USART1_RecvBuff[Recv_Max_Len] = {0};//串口1接收缓存区
u8 USART1_Flag = 0;//串口1接收完成标志位


int main(void)
{
	Serial_Init();
	DriverScreen_Init();
	PC13LED_Init();
	LCD_Control_On();
	MyKey_Init();
		
	SystemInit();//初始化RCC 设置系统主频为72MHZ
	delay_init(72);	     //延时初始化
	LCD_Init();	   //液晶屏初始化
	LCD_Clear(BLACK);
	//  Serial_USART2_SendByte(0x41);
	//LCD_ShowChar(0,10,WHITE,BLACK,'B',16,0);
	//LCD_ShowNum(0,30,520,4, 16);
	//LCD_ShowString(10,0,16,"Left Send:",0);
	LCD_Fill(0,0,lcddev.width,31,RED);
	MY_ShowString(X[10], Y[0], "Mode:String");
	MY_ShowString(X[10], Y[1], "Baud:115200");
	MY_ShowString(X[0], Y[2], "1 Send:");
	MY_ShowString(X[0], Y[11], "2 Send:");
	LCD_DrawLine(0, Y[11], 240, Y[11]);//分割线	
	//MY_ShowHexNum(X[1], Y[19],0xa5);
	//MY_ShowChar(100,100,'7');
	//MY_ShowNumber(50,50,1314,4);
	while(1)
	{
		if(USART1_Flag == 1)//查看串口是否接收到数据
		{
			USART1_Flag = 0;
			Assistant_1ShowBuff(USART1_RecvBuff, USART1_RecvBuffLen);
			USART1_RecvBuffLen = 0;
		} 
		if(USART3_Flag == 1)
		{
			USART3_Flag = 0;
			Assistant_2ShowBuff(USART3_RecvBuff, USART3_RecvBuffLen);
			USART3_RecvBuffLen = 0;
		} 
		MY_KeyScan();
	}
}



void USART2_IRQHandler(void)//usart2中断函数
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) == SET)
	{
		Data = USART_ReceiveData(USART2);
		Serial_USART2_SendByte(Data);
		//手动清除标志位
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
	}
}
void USART3_IRQHandler(void)//usart3中断函数
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		if(USART3_RecvBuffLen < Recv_Max_Len)
		{
			USART3_RecvBuff[USART3_RecvBuffLen++] = USART_ReceiveData(USART3);
		}
		else
		{
			USART3_RecvBuffLen = 0;
			USART3_RecvBuff[USART3_RecvBuffLen] = USART_ReceiveData(USART3);
		}
	}
	if(USART_GetITStatus(USART3, USART_IT_IDLE) == SET)
	{
		USART_ReceiveData(USART3);//读一次清空标志位
		//USART_ClearITPendingBit(USART2, USART_IT_IDLE);//清除空闲标志位
		if(USART3_RecvBuffLen > 0)
		{
			USART3_Flag = 1;
		}
	}
}
void USART1_IRQHandler(void)//usart1中断函数
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		if(USART1_RecvBuffLen < Recv_Max_Len)
		{
			USART1_RecvBuff[USART1_RecvBuffLen++] = USART_ReceiveData(USART1);
		}
		else
		{
			USART1_RecvBuffLen = 0;
			USART1_RecvBuff[USART1_RecvBuffLen] = USART_ReceiveData(USART1);
		}
	}
	if(USART_GetITStatus(USART1, USART_IT_IDLE) == SET)
	{
		USART_ReceiveData(USART1);//读一次清空标志位
		if(USART1_RecvBuffLen > 0)
		{
			USART1_Flag = 1;
		}
	}
}
