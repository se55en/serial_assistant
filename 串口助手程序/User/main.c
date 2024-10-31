#include "stm32f10x.h"                  // Device header

#include "mylcd.h"	//��Ļͷ�ļ�
#include "PC13LED.h" 

#include "menu.h"
#include "myKey.h"
#include "process.h"
#include "serial.h"
#include "mmc_sd.h"
#include "ff.h"
#include "mf.h"
#include "stdio.h"

u8 menu2_flag = 0;//�Ӳ˵���־λ 
int main(void)
{
	Serial_Init();//���ڳ�ʼ��
	
	myKey_Init();//������ʼ��
	myLCD_Power_Init();
	myLCD_Power_On();//LCD-VCC���翪�ش�
	myLCD_Init();
	Menu_ShowHead();
//	mf_test();
	while(1)
	{
		process(serial.ProcessMode);//�����������
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
void USART1_IRQHandler(void)    // ����1 ���жϴ�����
{
    uint8_t clear;
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)   // �����ж�
    {
        clear = USART1->SR; // ��������ж�
        clear = USART1->DR; // ��������ж�

//        serial.USART1_Flag = 1;  // �ý��ձ�־λ
        serial.USART1_RecvBuffLen = Recv_Max_Len - DMA_GetCurrDataCounter(DMA1_Channel5);// �ܵ�buf���ȼ�ȥʣ��buf���ȣ��õ����յ����ݵĳ���
		serial.USART1_NoDisplayLen = serial.USART1_RecvBuffLen - serial.USART1_DisplayLen;
    }   
}
void USART3_IRQHandler(void)//usart3�жϺ���
{
    uint8_t clear;
    if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)   // �����ж�
    {
        clear = USART3->SR; // ��������ж�
        clear = USART3->DR; // ��������ж�

        serial.USART3_RecvBuffLen = Recv_Max_Len - DMA_GetCurrDataCounter(DMA1_Channel3);// �ܵ�buf���ȼ�ȥʣ��buf���ȣ��õ����յ����ݵĳ���
		serial.USART3_NoDisplayLen = serial.USART3_RecvBuffLen - serial.USART3_DisplayLen;
    }
}
void USART2_IRQHandler(void)    // ����1 ���жϴ�����
{
    if(USART_GetITStatus(USART2, USART_IT_TC) != RESET)     // �������
    {
        USART_ClearITPendingBit(USART2, USART_IT_TC);       // �����ɱ��
        DMA_Cmd(DMA1_Channel7, DISABLE);                    // �ر�DMA
        serial.USART2_TxFlag = 1;                              // ���÷�����ɱ�־λ
    }	
}
