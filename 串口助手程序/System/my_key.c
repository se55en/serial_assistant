
#include "delay.h"
#include "gui.h"
#include "position.h"
#define BaudModeNum 4
u32 Baud[4] = {4800, 9600, 38400, 115200};
u8 ShowMode = 0;

u8 x1 = 0, y1 = 3;//2���յ�λ������
u8 x2 = 0, y2 = 12;//2���յ�λ������

extern u8 USART3_Flag;
extern u8 USART1_Flag;
extern uint16_t USART3_RecvBuffLen;
extern uint16_t USART1_RecvBuffLen;

void MyKey_Init(void)
{	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//ʹ��PB3��PB4Ҫ�ر�JTAG����
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	//��ʼ���ⲿʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//�����ṹ��
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//��ʼ��GPIO��
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint8_t MyKey_GetNum(void)
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

void ChangeBaud(u32 Baud)
{
	USART_InitTypeDef USART_InitStructure;
	//����USART1
	USART_InitStructure.USART_BaudRate = Baud;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	//����USART3
	USART_InitStructure.USART_BaudRate = Baud;
	USART_Init(USART3, &USART_InitStructure);
}
void ClearShow2(void)//���2���ߵĽ�����ʾ����
{
	u16 cnt = 8*30;
	x2 = 0;
	y2 = 12;
	while(cnt--)//���������ӵ�һ�����¿�ʼ��ʾ
	{
		MY_ShowChar(X0[x2],Y0[y2],' ');
		x2++;
		if(x2 > 29)
		{
			x2 = 0;
			y2++;
		}
	}
	x2 = 0;
	y2 = 12;
}
void ClearShow1(void)//���2���ߵĽ�����ʾ����
{
	u16 cnt = 8*30;
	x1 = 0;
	y1 = 3;
	while(cnt--)//���������ӵ�һ�����¿�ʼ��ʾ
	{
		MY_ShowChar(X0[x1],Y0[y1],' ');
		x1++;
		if(x1 > 29)
		{
			x1 = 0;
			y1++;
		}
	}
	x1 = 0;//����Ƶ���һ�е�һ��
	y1 = 3;
}
		
void MY_KeyScan(void)
{
	static u8 BaudMode = 3;
	u8 KeyNum = MyKey_GetNum();
	if(KeyNum != 0)
	{
		switch(KeyNum)
		{
			case 2:
			{
				BaudMode++;
				if(BaudMode >= BaudModeNum)
				{
					BaudMode = 0;
				}
				ChangeBaud(Baud[BaudMode]);
				LCD_ShowNumRed(X0[15], Y0[1],Baud[BaudMode], 6, 16);
				break;
			}
			case 3:
			{
				//0���ı�ģʽ��1��HEXģʽ
				ShowMode++;
				if(ShowMode >= 2)
				{
					ShowMode = 0;
				}
				if(ShowMode)
				{
					LCD_ShowStringRed(X0[15], Y0[0],16,"HEX   ", 0);
				}
				else
				{
					LCD_ShowStringRed(X0[15], Y0[0],16,"String", 0);
				}
				break;
			}
			case 4:
			{
				ClearShow1();
				ClearShow2();
				break;
			}
			default:
				break;
		}
	}
}

void Assistant_2ShowBuff(u8* buff, u16 len)
{
	static u8 ClearFlag = 0;
	u8* p_temp = buff;
	while(len--)
	{
		if(USART3_Flag == 1)
		{
			USART3_Flag = 0;
			x2 = 0;
			Assistant_2ShowBuff(p_temp, USART3_RecvBuffLen);
			break;
		}
		if(ClearFlag == 1)
		{
			ClearShow2();
			ClearFlag = 0;
		}
		if(*buff == 0x0D)//-------/r
		{
			buff++;
			x2 = 0;
			continue;
		}
		if(*buff == 0x0A)//---------/n
		{
			buff++;
			y2++;
			if(y2 > 19)
			{
				ClearFlag = 1;
				y2 = 12;
			}
			continue;
		}
		if(ShowMode == 0)
		{
			MY_ShowChar(X0[x2],Y0[y2],*buff++);
			x2++;
			if(x2>29)
			{
				x2 = 0;
				y2++;
				if(y2 > 19)
				{
					ClearFlag = 1;
					y2 = 12;
				}
			}
		}
		else
		{
			MY_ShowHexNum(X0[x2],Y0[y2],*buff++);
			x2 += 2;
			if(x2>29)
			{
				x2 = 0;
				y2++;
				if(y2 > 19)
				{
					ClearFlag = 1;
					y2 = 12;
				}
			}
			MY_ShowChar(X0[x2],Y0[y2],' ');
			x2++;
			if(x2>29)
			{
				x2 = 0;
				y2++;
				if(y2 > 19)
				{
					ClearFlag = 1;
					y2 = 12;
				}
			}
		}
	}
}
void Assistant_1ShowBuff(u8* buff, u16 len)
{
	static u8 ClearFlag = 0;
	u8* p_temp = buff;
	while(len--)
	{
		if(USART1_Flag == 1)
		{
			USART1_Flag = 0;
			x1 = 0;
			Assistant_1ShowBuff(p_temp, USART1_RecvBuffLen);
			break;
		}
		if(ClearFlag == 1)
		{
			ClearShow1();
			ClearFlag = 0;
		}
		if(*buff == 0x0D)
		{
			buff++;
			x1 = 0;
			continue;
		}
		if(*buff == 0x0A)
		{
			buff++;
			y1++;
			if(y1 > 10)
			{
				ClearFlag = 1;
				y1 = 3;
			}
			continue;
		}
		if(ShowMode == 0)//�ı�ģʽ��ʾ
		{
			MY_ShowChar(X0[x1],Y0[y1],*buff++);
			x1++;
			if(x1>29)
			{
				x1 = 0;
				y1++;
				if(y1 > 10)
				{
					ClearFlag = 1;
					y1 = 3;
				}
			}
		}
		else//HEXģʽ��ʾ
		{
			MY_ShowHexNum(X0[x1],Y0[y1],*buff++);
			x1 += 2;
			if(x1>29)
			{
				x1 = 0;
				y1++;
				if(y1 > 10)
				{
					ClearFlag = 1;
					y1 = 3;
				}
			}
			MY_ShowChar(X0[x1],Y0[y1],' ');
			x1++;
			if(x1>29)
			{
				x1 = 0;
				y1++;
				if(y1 > 10)
				{
					ClearFlag = 1;
					y1 = 3;
				}
			}
		}
	}
}
