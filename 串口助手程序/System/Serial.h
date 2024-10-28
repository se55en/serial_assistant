#ifndef _SERIAL_H
#define _SERIAL_H
#include "stm32f10x.h"                  // Device header
#include "string.h"
#define Recv_Max_Len 2048//一次接收数据最大长度

extern char print_buf[512];

typedef struct
{
	u32 Baud;
	u8 ShowMode;//0为字符串，1为Hex
	u8 ProcessMode;//0为屏幕显示，1为转发给pc，2为sd卡存储
	
	u16 USART1_RecvBuffLen;//串口1接收数据长度
	u16 USART1_DisplayLen;	//已经显示的数据长度
	u16 USART1_NoDisplayLen;//未显示数据长度
	u8 USART1_RecvBuff[Recv_Max_Len];//串口1接收缓存区

	u16 USART3_RecvBuffLen;//串口3接收数据长度
	u16 USART3_DisplayLen;	//已经显示的数据长度
	u16 USART3_NoDisplayLen;//未显示数据长度
	u8 USART3_RecvBuff[Recv_Max_Len];//串口3接收缓存区
	
	u32 toPCBaud;//发送给电脑的波特率
	u16 USART2_TxBuffLen;//串口2发送数据长度
	u8 USART2_TxBuff[Recv_Max_Len];//串口2发送接数据缓存区
	u8 USART2_TxFlag;//发送完成标志位
}Serial_TypeDef;

extern Serial_TypeDef serial;

void Serial_Init(void);
void Serial_USART2_SendByte(uint8_t Byte);
void Serial_DMA_Init(void);
void uart1DmaClear(void);
void uart3DmaClear(void);
void serial_Change13Baud(u32 Baud);//修改串口13波特率
void serial_Change2Baud(u32 Baud);
void uart2SendArray(uint8_t *arr, uint16_t len);
#endif
