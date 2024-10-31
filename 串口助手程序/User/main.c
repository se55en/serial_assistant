#include "stm32f10x.h"                  // Device header

#include "mylcd.h"	//屏幕头文件
#include "PC13LED.h" 

#include "menu.h"
#include "myKey.h"
#include "process.h"
#include "serial.h"
#include "mmc_sd.h"
#include "ff.h"
#include "mf.h"
#include "stdio.h"

u8 menu2_flag = 0;//子菜单标志位 
int main(void)
{
	Serial_Init();//串口初始化
	
	myKey_Init();//按键初始化
	myLCD_Power_Init();
	myLCD_Power_On();//LCD-VCC供电开关打开
	myLCD_Init();
	Menu_ShowHead();
//	mf_test();
	while(1)
	{
		process(serial.ProcessMode);//处理接收数据
		while(1)
		{
			menu2_flag = Menu();
			if(menu2_flag == 1)menu2_baud();
			if(menu2_flag == 2)menu2_showmode();
			if(menu2_flag == 3)menu2_processmode();
			if(menu2_flag == 0)break;
		}
	}
}
void USART1_IRQHandler(void)    // 串口1 的中断处理函数
{
    uint8_t clear;
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)   // 空闲中断
    {
        clear = USART1->SR; // 清除空闲中断
        clear = USART1->DR; // 清除空闲中断

//        serial.USART1_Flag = 1;  // 置接收标志位
        serial.USART1_RecvBuffLen = Recv_Max_Len - DMA_GetCurrDataCounter(DMA1_Channel5);// 总的buf长度减去剩余buf长度，得到接收到数据的长度
		serial.USART1_NoDisplayLen = serial.USART1_RecvBuffLen - serial.USART1_DisplayLen;
    }   
}
void USART3_IRQHandler(void)//usart3中断函数
{
    uint8_t clear;
    if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)   // 空闲中断
    {
        clear = USART3->SR; // 清除空闲中断
        clear = USART3->DR; // 清除空闲中断

        serial.USART3_RecvBuffLen = Recv_Max_Len - DMA_GetCurrDataCounter(DMA1_Channel3);// 总的buf长度减去剩余buf长度，得到接收到数据的长度
		serial.USART3_NoDisplayLen = serial.USART3_RecvBuffLen - serial.USART3_DisplayLen;
    }
}
void USART2_IRQHandler(void)    // 串口1 的中断处理函数
{
    if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)     // 发送完成
    {
        USART_ClearITPendingBit(USART2, USART_IT_TC);       // 清除完成标记
        DMA_Cmd(DMA1_Channel7, DISABLE);                    // 关闭DMA
        serial.USART2_TxFlag = 1;                              // 设置发送完成标志位
    }	
}
