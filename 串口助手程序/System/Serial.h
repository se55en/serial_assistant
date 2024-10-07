#ifndef _SERIAL_H
#define _SERIAL_H

#define Recv_Max_Len 8192//一次接收数据最大长度

extern uint16_t USART1_RecvBuffLen;//串口1接收数据长度
extern u8 USART1_RecvBuff[Recv_Max_Len];//串口1接收缓存区
extern u8 USART1_Flag;//串口1接收完成标志位
extern uint16_t USART3_RecvBuffLen;//串口3接收数据长度
extern u8 USART3_RecvBuff[Recv_Max_Len];//串口1接收缓存区
extern u8 USART3_Flag;//串口3接收完成标志位

void Serial_Init(void);
void Serial_USART2_SendByte(uint8_t Byte);
void Serial_DMA_Init(void);
void uart1DmaClear(void);
void uart3DmaClear(void);
#endif
