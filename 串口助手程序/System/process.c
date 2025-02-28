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
u8 new_line = 1;//���б�־λ
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
u32 write_num = 0;//д��������Ŀ
void SendToSD(void);
void my_f_write1(uint8_t *arr, uint16_t len);
void my_f_write2(uint8_t *arr, uint16_t len);

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
	new_line = 1;
	Show_Str(X[0],Y[19],WHITE, BLUE, "K1:�˵�|K2:   |K3:   |K4:����",16,0);
	while(1)
	{
		KeyNum = myKey_GetNum();
		if(KeyNum == 1)
		{
			//�����ʾ��
			LCD_Fill(0, Y[2], 240, 303,WHITE);
			return 1;
		}
		if(KeyNum == 4)
		{
			//�����ʾ��
			x = 0;
			y = 2;
			new_line = 1;
			LCD_Fill(0, Y[2], 240, 303,WHITE);
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
	sprintf(print_buf, "PC Baud = %7d",serial.toPCBaud);Gui_StrCenter(X[0],Y[9],BLACK, WHITE,(u8*)print_buf,16,0);
	Show_Str(X[0],Y[19],WHITE, BLUE, "K1:�˵�|K2:   |K3:   |K4:    ",16,0);
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
	u32 size = 0;//�ļ���С
	u8 res_tf = 0;
	u8 KeyNum = 0;
	static u8 first_flag = 0;//��һ�ι���
	Show_Str(X[0],Y[19],WHITE, BLUE, "K1:����|K2:   |K3:   |K4:    ",16,0);
	//sd������
	if(!first_flag)
	{
		res_tf = f_mount(&fs_FatFs,"0:",1); //����SD�� 
//		sprintf(print_buf, "f_mount res = %d",res_tf);myLCD_ShowString(X[3], Y[12], (u8*)print_buf);
		if(res_tf == FR_NO_FILESYSTEM) /* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
		{
			res_tf = f_mkfs("0:", 0, fs_work, sizeof(fs_work));
//			sprintf(print_buf, "f_mkfs res = %d",res_tf);myLCD_ShowString(X[3], Y[14], (u8*)print_buf);
			res_tf = f_mount(&fs_FatFs,"0:",1); //����SD�� 
		}	
		if(res_tf == FR_OK)
		{
			myLCD_ShowString(X[5], Y[7], "TFcard Mount Success");
			/* ���ļ�������ļ��������򴴽��� */
			res_tf = f_open(&fs_file,"0:usart_data.txt",FA_OPEN_APPEND|FA_WRITE|FA_READ);//׷���ļ�
//			sprintf(print_buf, "f_open res = %d",res_tf);myLCD_ShowString(X[3], Y[13], (u8*)print_buf);
		}
		else
		{
			Gui_StrCenter(X[0],Y[18],WHITE, RED, "TFcard Mount Error!!!",16,0);
		}
		first_flag = 1;
	}
	u32 total = 0,free = 0;
	exf_getfree((u8*)"0:",&total,&free);	//�õ�SD������������ʣ������												 
	sprintf(print_buf, "SD Total Size:%d MB\n",total>>10);
	myLCD_ShowString(X[5], Y[9], (u8*)print_buf);
	sprintf(print_buf, "SD Free Size: %d MB\n",free>>10);
	myLCD_ShowString(X[5], Y[10], (u8*)print_buf);
	Show_Str(X[6],Y[11],BLACK, WHITE, "�洢���ݵ�TF��...",16,0);
	while(1)
	{
		//��������
		KeyNum = myKey_GetNum();
		if(KeyNum)
		{
			if(KeyNum == 1)
			{
				write_num = 0;
				LCD_Fill(0, Y[2], 240, 303,WHITE);//�����ʾ��
				res_tf = f_sync(&fs_file);//�����ļ�
				size = f_size(&fs_file);
//				sprintf(print_buf, "File Size = %d Byte,res=%d", size,res_tf);
				sprintf(print_buf, "File Size = %d", size);
				Gui_StrCenter(X[0],Y[18],BLACK, WHITE, (u8*)print_buf,16,0);
				return;
			}
		}
		if(serial.USART1_NoDisplayLen)//��û��ʾ�����ݣ���д���ļ�
		{
			while(serial.USART1_NoDisplayLen)
			{
				f_write(&fs_file, "\n[1>>]:", 7,&fs_bw);
				write_num += 7;
				my_f_write1(serial.USART1_RecvBuff+serial.USART1_DisplayLen, serial.USART1_NoDisplayLen);        //д����
			}
			uart1DmaClear();    // ���DMA����ͨ��
			serial.USART1_RecvBuffLen = 0;
			serial.USART1_NoDisplayLen = 0;
			serial.USART1_DisplayLen = 0;
		}
		if(serial.USART3_NoDisplayLen)//��û��ʾ�����ݣ�����ʾ��ʼ��������
		{
			while(serial.USART3_NoDisplayLen)
			{
				f_write(&fs_file, "\n[2>>]:", 7,&fs_bw);
				write_num += 7;
				my_f_write2(serial.USART3_RecvBuff+serial.USART3_DisplayLen, serial.USART3_NoDisplayLen);        //д����
			}
			uart3DmaClear();    // ���DMA����ͨ��
			serial.USART3_RecvBuffLen = 0;
			serial.USART3_NoDisplayLen = 0;
			serial.USART3_DisplayLen = 0;
		}
	}
}

void my_f_write1(uint8_t *arr, uint16_t len)
{
	u8 res_tf = 0;
	res_tf = f_write(&fs_file, arr, len,&fs_bw);
	serial.USART1_DisplayLen += len;
	serial.USART1_NoDisplayLen -= len;
	write_num += fs_bw;
	if(!res_tf)
	{
		sprintf(print_buf, "Write Success");myLCD_ShowString(X[6], Y[12], (u8*)print_buf);
		sprintf(print_buf, "Write: %d Byte",write_num);myLCD_ShowString(X[6], Y[13], (u8*)print_buf);
	}
	else{
		sprintf(print_buf, "Write Fail Please Check TFCard");myLCD_ShowString(X[6], Y[12], (u8*)print_buf);
	}
	

}
void my_f_write2(uint8_t *arr, uint16_t len)
{
	f_write(&fs_file, arr, len,&fs_bw);
	serial.USART3_DisplayLen += len;
	serial.USART3_NoDisplayLen -= len;
	write_num += fs_bw;
	sprintf(print_buf, "Write: %d Byte",write_num);myLCD_ShowString(X[6], Y[13], (u8*)print_buf);
}
//��ʾ����ͷ
void ShowHead1(void)
{
	if(new_line == 1)
	{
		new_line = 0;
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
	if(new_line == 1)
	{
		new_line = 0;
	}
	else
	{
		x = 0;//�س�����
		AddY(1);
	}

	myLCD_ShowString(X[x], Y[y], "[2>>]:");
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
