#ifndef _SERIAL_H
#define _SERIAL_H

#define Recv_Max_Len 8192//һ�ν���������󳤶�

extern uint16_t USART1_RecvBuffLen;//����1�������ݳ���
extern u8 USART1_RecvBuff[Recv_Max_Len];//����1���ջ�����
extern u8 USART1_Flag;//����1������ɱ�־λ
extern uint16_t USART3_RecvBuffLen;//����3�������ݳ���
extern u8 USART3_RecvBuff[Recv_Max_Len];//����1���ջ�����
extern u8 USART3_Flag;//����3������ɱ�־λ

void Serial_Init(void);
void Serial_USART2_SendByte(uint8_t Byte);
void Serial_DMA_Init(void);
void uart1DmaClear(void);
void uart3DmaClear(void);
#endif
