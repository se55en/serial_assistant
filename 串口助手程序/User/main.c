#include "stm32f10x.h"                  // Device header
#include "string.h"
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
uint8_t Data;

int main(void)
{
	PC13LED_Init();
	Serial_Init();
	MyKey_Init();
//屏幕相关初始化
	DriverScreen_Init();
	LCD_Control_On();	
	SystemInit();//初始化RCC 设置系统主频为72MHZ
	delay_init(72);	     //延时初始化
	LCD_Init();	   //液晶屏初始化
	LCD_Clear(BLACK);

	LCD_Fill(0,0,lcddev.width,31,RED);
	MY_ShowString(X[10], Y[0], "Mode:String");
	MY_ShowString(X[10], Y[1], "Baud: 115200");
	MY_ShowString(X[0], Y[2], "1 Send:");
	MY_ShowString(X[0], Y[11], "2 Send:");
	LCD_DrawLine(0, Y[11], 240, Y[11]);//分割线	
	while(1)
	{
		if(USART1_Flag == 1)//查看串口是否接收到数据
		{
			USART1_Flag = 0;
			Assistant_1ShowBuff(USART1_RecvBuff, USART1_RecvBuffLen, 0);
			uart1DmaClear();    // 清空DMA接收通道
//			memset(USART1_RecvBuff, '\0', sizeof(USART1_RecvBuff)); // 清空接收缓冲区
		} 
		if(USART3_Flag == 1)
		{
			USART3_Flag = 0;
			Assistant_2ShowBuff(USART3_RecvBuff, USART3_RecvBuffLen, 0);
			uart3DmaClear();    // 清空DMA接收通道
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
    uint8_t clear;
    if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)   // 空闲中断
    {
        clear = USART3->SR; // 清除空闲中断
        clear = USART3->DR; // 清除空闲中断

        USART3_Flag = 1;  // 置接收标志位
        USART3_RecvBuffLen = Recv_Max_Len - DMA_GetCurrDataCounter(DMA1_Channel3);// 总的buf长度减去剩余buf长度，得到接收到数据的长度
    }   
}

void USART1_IRQHandler(void)    // 串口1 的中断处理函数
{
    uint8_t clear;
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)   // 空闲中断
    {
        clear = USART1->SR; // 清除空闲中断
        clear = USART1->DR; // 清除空闲中断

        USART1_Flag = 1;  // 置接收标志位
        USART1_RecvBuffLen = Recv_Max_Len - DMA_GetCurrDataCounter(DMA1_Channel5);// 总的buf长度减去剩余buf长度，得到接收到数据的长度
    }   
}

