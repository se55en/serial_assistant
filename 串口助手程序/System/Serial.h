#ifndef _SERIAL_H
#define _SERIAL_H
#include "stm32f10x.h"                  // Device header
#include "string.h"
#define Recv_Max_Len 2048//һ�ν���������󳤶�

extern char print_buf[512];

typedef struct
{
	u32 Baud;
	u8 ShowMode;//0Ϊ�ַ�����1ΪHex
	u8 ProcessMode;//0Ϊ��Ļ��ʾ��1Ϊת����pc��2Ϊsd���洢
	
	u16 USART1_RecvBuffLen;//����1�������ݳ���
	u16 USART1_DisplayLen;	//�Ѿ���ʾ�����ݳ���
	u16 USART1_NoDisplayLen;//δ��ʾ���ݳ���
	u8 USART1_RecvBuff[Recv_Max_Len];//����1���ջ�����

	u16 USART3_RecvBuffLen;//����3�������ݳ���
	u16 USART3_DisplayLen;	//�Ѿ���ʾ�����ݳ���
	u16 USART3_NoDisplayLen;//δ��ʾ���ݳ���
	u8 USART3_RecvBuff[Recv_Max_Len];//����3���ջ�����
	
	u32 toPCBaud;//���͸����ԵĲ�����
	u16 USART2_TxBuffLen;//����2�������ݳ���
	u8 USART2_TxBuff[Recv_Max_Len];//����2���ͽ����ݻ�����
	u8 USART2_TxFlag;//������ɱ�־λ
}Serial_TypeDef;

extern Serial_TypeDef serial;

void Serial_Init(void);
void Serial_USART2_SendByte(uint8_t Byte);
void Serial_DMA_Init(void);
void uart1DmaClear(void);
void uart3DmaClear(void);
void serial_Change13Baud(u32 Baud);//�޸Ĵ���13������
void serial_Change2Baud(u32 Baud);
void uart2SendArray(uint8_t *arr, uint16_t len);
#endif
