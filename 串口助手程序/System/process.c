#include "process.h"
extern Serial_TypeDef serial;

typedef struct
{
	u16 no_display_num;//δ��ʾ����
	u16 all_num;//0Ϊ�ַ�����1ΪHex
	u8 ProcessMode;//0Ϊ��Ļ��ʾ��1Ϊת����pc��2Ϊsd���洢
}Show_TypeDef;
//��Ļ��ʾλ������
#define X_MIN 		0
#define X_MAX 		29
#define Y_MIN 		2
#define Y_MAX 		18

u8 x = 0, y = 2;//���λ��
u8 first_flag = 1;//��һ�β��ûس�����
//��Ļ��ʾ��غ���
u8 ScreenDisplay(void);
void ShowHead1(void);
void ShowHead2(void);
u8 AddX(u8 add);
u8 AddY(u8 add);
void Show1(void);
void Show2(void);
//ת����������غ���
void SendToPC(void);
//ת����SD����غ���
void SendToSD(void);
void my_f_write(uint8_t *arr, uint16_t len);

void process(u8 mode)//��������ģʽ
{
	if(mode == 0){ScreenDisplay();}
	if(mode == 1){SendToPC();}
	if(mode == 2){SendToSD();}
}

u8 ScreenDisplay(void)
{
	u8 KeyNum = 0;
	x = 0, y = 2;//���λ��
	first_flag = 1;
	while(1)
	{
		KeyNum = myKey_GetNum();
		if(KeyNum == 1)
		{
			//�����ʾ��
			LCD_Fill(0, Y[2], 240, 303,WHITE);
			return 1;
		}
		if(serial.USART1_NoDisplayLen)//��û��ʾ�����ݣ�����ʾ��ʼ��ʾ����
		{
			Show1();
			uart1DmaClear();    // ���DMA����ͨ��
			serial.USART1_RecvBuffLen = 0;
			serial.USART1_NoDisplayLen = 0;
			serial.USART1_DisplayLen = 0;
		}
		//��ʾ2���͵�����
		if(serial.USART3_NoDisplayLen)//��û��ʾ�����ݣ�����ʾ��ʼ��ʾ����
		{
			Show2();
			uart3DmaClear();    // ���DMA����ͨ��
			serial.USART3_RecvBuffLen = 0;
			serial.USART3_NoDisplayLen = 0;
			serial.USART3_DisplayLen = 0;
		}
	}
}
void SendToPC(void)
{
	u8 KeyNum = 0;
	Show_Str(X[7],Y[10],BLACK, WHITE, "ת�����ݵ�����...",16,0);
	while(1)
	{
		//��������
		KeyNum = myKey_GetNum();
		if(KeyNum == 1)
		{
			//�����ʾ��
			LCD_Fill(0, Y[2], 240, 303,WHITE);
			return;
		}
		//�������ݸ�pc
		if(serial.USART1_RecvBuffLen)//��û��ʾ�����ݣ�����ʾ��ʼ��������
		{
			uart2SendArray("[1>>]:", 6);
			uart2SendArray(serial.USART1_RecvBuff, serial.USART1_RecvBuffLen);        // ʹ��DMA��������
			uart1DmaClear();    // ���DMA����ͨ��
			serial.USART1_RecvBuffLen = 0;
			serial.USART1_NoDisplayLen = 0;
			serial.USART1_DisplayLen = 0;
		}
		if(serial.USART3_RecvBuffLen)//��û��ʾ�����ݣ�����ʾ��ʼ��������
		{
			uart2SendArray("[2>>]:", 6);
			uart2SendArray(serial.USART3_RecvBuff, serial.USART3_RecvBuffLen);        // ʹ��DMA��������
			uart3DmaClear();    // ���DMA����ͨ��
			serial.USART3_RecvBuffLen = 0;
			serial.USART3_NoDisplayLen = 0;
			serial.USART3_DisplayLen = 0;
		}
	}
}
void SendToSD(void)
{
	u8 res_sd = 0;
	static u8 init_flag = 0;//��ʼ����־��0��ʾû��ʼ��  
	u8 first_flag = 0;//��һ�ν��뺯��
	u8 KeyNum = 0;
	while(1)
	{
		//��������
		KeyNum = myKey_GetNum();
		if(KeyNum)
		{
			if(KeyNum == 1)
			{
				f_close(&fs_file);//�ر��ļ�
				//�����ʾ��
				LCD_Fill(0, Y[2], 240, 303,WHITE);
				return;
			}
			if(KeyNum == 4)//����
			{
				f_close(&fs_file);//�ر��ļ�
				f_open(&fs_file,"0:usart_data.txt",FA_OPEN_APPEND|FA_WRITE);//���ļ�
			}
		}
		//sd������
		if(!init_flag)//��ʼ��sd��
		{
			u8 t = 100;
			while(SD_Initialize()&&t)//��ⲻ��SD��
			{
				KeyNum = myKey_GetNum();
				if(KeyNum == 1)
				{
					f_close(&fs_file);//�ر��ļ�
					//�����ʾ��
					LCD_Fill(0, Y[2], 240, 303,WHITE);
					return;
				}
				if(t == 100)
				{
					myLCD_ShowString(X[5], Y[7], "Please Check TFcard!");
				}
				t--;
			}
			if(t == 0)//��ʼ��ʧ��
			{
				Gui_StrCenter(X[0],Y[18],WHITE, RED, "TF Initialize Error!",16,0);
				LCD_Fill(0, Y[2], 240, Y[18]-1,WHITE);
				return;
			}
			myLCD_ShowString(X[5], Y[7], "TFcard Init Success ");
			res_sd = f_mount(&fs,"0:",1); //����SD�� 
			//��ӡ��Ϣ
			sprintf(print_buf, "\nf_mount res_sd = %d\n",res_sd);uart2SendArray((u8*)print_buf, strlen(print_buf));
			if(res_sd)
			{
				Gui_StrCenter(X[0],Y[18],WHITE, RED, "TFcard Mount Fail!",16,0);
			}
			/*----------------------- ��ʽ������ ---------------------------*/
			/* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
			if (res_sd == FR_NO_FILESYSTEM) 
			{
				res_sd=f_mkfs("0:", 0, fs_work, sizeof(fs_work));
				sprintf(print_buf, "\nf_mkfs res_sd = %d\n",res_sd);uart2SendArray((u8*)print_buf, strlen(print_buf));
				res_sd = f_mount(&fs,"0:",1); //����SD�� 
			}	
			init_flag = 1;
		}
		if(!first_flag)
		{
			u32 total = 0,free = 0;
			exf_getfree((u8*)"0:",&total,&free);	//�õ�SD������������ʣ������												 
			sprintf(print_buf, "TF Total Size:   %d MB\n",total>>10);
			myLCD_ShowString(X[3], Y[10], (u8*)print_buf);
			sprintf(print_buf, "TF Free Size:    %d MB\n",free>>10);
			myLCD_ShowString(X[3], Y[11], (u8*)print_buf);
			Show_Str(X[25],Y[19],WHITE, BLUE, "����",16,0);
			/* ���ļ�������ļ��������򴴽��� */
			res_sd=f_open(&fs_file,"0:usart_data.txt",FA_OPEN_APPEND|FA_WRITE);//"a",׷���ļ�
			sprintf(print_buf, "f_open res_sd = %d\n", res_sd);uart2SendArray((u8*)print_buf, strlen(print_buf));
			if(res_sd == FR_OK )
			{
				sprintf(print_buf, "��/�����ļ��ɹ�\r\n");uart2SendArray((u8*)print_buf, strlen(print_buf));
			} 
			else
			{
				sprintf(print_buf, "��/�����ļ�ʧ�ܡ�\r\n");uart2SendArray((u8*)print_buf, strlen(print_buf));
			}
			first_flag = 1;
		}
		if(serial.USART1_RecvBuffLen)//��û��ʾ�����ݣ�����ʾ��ʼ��������
		{
			strcpy((char *)fs_buffer, "\n[1>>]:");
			res_sd=f_write(&fs_file, fs_buffer, strlen((char *)fs_buffer),&fs_bw);
			my_f_write(serial.USART1_RecvBuff, serial.USART1_RecvBuffLen);        //д����
			uart1DmaClear();    // ���DMA����ͨ��
			serial.USART1_RecvBuffLen = 0;
			serial.USART1_NoDisplayLen = 0;
			serial.USART1_DisplayLen = 0;
		}
		if(serial.USART3_RecvBuffLen)//��û��ʾ�����ݣ�����ʾ��ʼ��������
		{
			strcpy((char *)fs_buffer, "\n[2>>]:");
			res_sd=f_write(&fs_file, fs_buffer, strlen((char *)fs_buffer),&fs_bw);
			my_f_write(serial.USART3_RecvBuff, serial.USART3_RecvBuffLen);        //д����
			uart3DmaClear();    // ���DMA����ͨ��
			serial.USART3_RecvBuffLen = 0;
			serial.USART3_NoDisplayLen = 0;
			serial.USART3_DisplayLen = 0;
		}
	}
}

void my_f_write(uint8_t *arr, uint16_t len)
{
	u8 res_sd;
	if(len == 0)	// �жϳ����Ƿ���Ч
    return;
	
    if(arr) 
	{
		f_write(&fs_file, arr, len,&fs_bw);
//		if (res_sd==FR_OK) 
//		{
//			sprintf(print_buf, "�ļ�д��ɹ���д���ֽ����ݣ�%d\n",fs_bw);uart2SendArray((u8*)print_buf, strlen(print_buf));
//        }
//		else
//		{
//			sprintf(print_buf, "�����ļ�д��ʧ�ܣ�(%d)\n",res_sd);uart2SendArray((u8*)print_buf, strlen(print_buf));
//        }
	}
}
//��ʾ����ͷ
void ShowHead1(void)
{
	if(first_flag == 1)
	{
		first_flag = 0;
	}
	else
	{
		x = 0;//�س�����
		AddY(1);
	}

	myLCD_ShowString(X[x], Y[y], "[1>>]:");
	AddX(6);
}
void ShowHead2(void)
{
	if(first_flag == 1)
	{
		first_flag = 0;
	}
	else
	{
		x = 0;//�س�����
		AddY(1);
	}

	myLCD_ShowString(X[x], Y[y], "[2>>:]");
	AddX(6);
}
//�ƶ����,����������־,1Ϊ����
u8 AddX(u8 add)
{
	u8 result = 0;
	while(add--)
	{
		x ++;
		if(x>X_MAX)
		{
			x = X_MIN;
			y++;
			if(y > Y_MAX)
			{
				//�����ʾ��
				LCD_Fill(0, Y[2], 240-1, 303,WHITE);
				y = Y_MIN;
				result = 1;
			}
		}
	}
	return result;
}
u8 AddY(u8 add)
{
	u8 result = 0;
	while(add--)
	{
		y++;
		if(y > Y_MAX)
		{
			//�����ʾ��
			LCD_Fill(0, Y[2], 240-1, 303,WHITE);
			y = Y_MIN;
			result = 1;
		}
	}
	return result;
}
void Show1(void)
{
	ShowHead1();
	u8* buff = serial.USART1_RecvBuff;
	while(serial.USART1_NoDisplayLen--)
	{
		if(*buff == 0x0D)
		{
			x = 0;
			continue;
		}
		if(*buff == 0x0A)
		{
			AddY(1);
			continue;
		}
		if(serial.ShowMode == 0)//�ַ���ʾ
		{
			//�ж��ǲ��ǷǷ��ַ�
			if((*buff>=' ') && (*buff<'~'))
			{
				myLCD_ShowChar(X[x],Y[y],*buff);
				AddX(1);
			}
		}
		else if(serial.ShowMode == 1)//hex��ʾ
		{
			myLCD_ShowHexNum(X[x],Y[y],*buff);
			AddX(2);
			myLCD_ShowChar(X[x],Y[y],' ');
			AddX(1);
		}
		buff++;
		serial.USART1_DisplayLen++;
	}
}
void Show2(void)
{
	ShowHead2();
	u8* buff = serial.USART3_RecvBuff;
	while(serial.USART3_NoDisplayLen--)
	{
		if(*buff == 0x0D)
		{
			x = 0;
			continue;
		}
		if(*buff == 0x0A)
		{
			AddY(1);
			continue;
		}
		if(serial.ShowMode == 0)//�ַ���ʾ
		{
			//�ж��ǲ��ǷǷ��ַ�
			if((*buff>=' ') && (*buff<'~'))
			{
				myLCD_ShowChar(X[x],Y[y],*buff);
				AddX(1);
			}
		}
		else if(serial.ShowMode == 1)//hex��ʾ
		{
			myLCD_ShowHexNum(X[x],Y[y],*buff);
			AddX(2);
			myLCD_ShowChar(X[x],Y[y],' ');
			AddX(1);
		}
		buff++;
		serial.USART3_DisplayLen++;
	}
}
