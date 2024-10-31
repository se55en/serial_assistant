#include "menu.h"


#define BaudModeNum 4
const u32 Baud[BaudModeNum] = {4800, 9600, 38400, 115200};
u8 ShowMode[2][5] = {"�ַ�", "Hex "};
u8 ProcessMode[3][20] = {"��Ļ��ʾ����", "ת��������  ", "�洢��TF��  "};

void ShowCursor(u8 flag);
void ClearMenu(void);
u8 ChooseDir(u8 DirNum);
void menu3_SendToPC(void);
void menu4_toPCBaud(void);
u32 user_define(void);

void Menu_ShowHead(void)//ͷβҳ��
{
	LCD_Fill(0,0,lcddev.width,31,BLUE);//0-31
	LCD_Fill(0,lcddev.height-16,lcddev.width,lcddev.height-1,BLUE);//��16
	Show_Str(X[0],Y[0],WHITE, BLUE, "ģʽ:��Ļ��ʾ����",16,0);
	Show_Str(X[0],Y[1],WHITE, BLUE, "������: 115200",16,0);
	Show_Str(X[17],Y[1],WHITE, BLUE, "��ʾģʽ:�ַ�",16,0);
	
	Show_Str(X[0],Y[19],WHITE, BLUE, "K1:�˵�|K2:�� |K3:�� |K4:ȷ��",16,0);
}

u8 Menu(void)
{
	LCD_DrawRectangle(X[2], Y[3], X[28], Y[15]);
	LCD_DrawLine(X[2], Y[4], X[28], Y[4]);
	Gui_StrCenter(X[0],Y[3],BLACK,WHITE,"�˵�",16,1);//������ʾ
	Show_Str(X[10],Y[5],BLACK,WHITE, "���ղ�����",16,0);
	Show_Str(X[10],Y[7],BLACK,WHITE, "��ʾģʽ",16,0);
	Show_Str(X[10],Y[9],BLACK,WHITE, "ģʽ",16,0);
	myLCD_ShowString(X[8], Y[5], ">");
	Show_Str(X[3],Y[19],WHITE, BLUE, "����",16,0);
	Show_Str(X[25],Y[19],WHITE, BLUE, "ȷ��",16,0);
	u8 result = ChooseDir(3);
	if(result == 0)
	{
		//��ȫ����˵�
		LCD_Fill(0, Y[2], 240, 303,WHITE);
		Show_Str(X[3],Y[19],WHITE, BLUE, "�˵�",16,0);
	}
	return result;
}


u8 menu2_baud(void)
{
	u8 dir_flag = 0;//Ŀ¼����ֵ
	myLCD_ShowString(X[10], Y[5], "4800");
	myLCD_ShowString(X[10], Y[7], "9600");
	myLCD_ShowString(X[10], Y[9], "38400");
	myLCD_ShowString(X[10], Y[11], "115200");
	Show_Str(X[10],Y[13],BLACK,WHITE, "�Զ���",16,0);
	myLCD_ShowString(X[8], Y[5], ">");
	dir_flag = ChooseDir(BaudModeNum+1);
	if(dir_flag==0){
		ClearMenu();return 0;
	}
	if(dir_flag == 5)//�Զ��岨����
	{
		ClearMenu();
		serial.Baud = user_define();//�Զ��岨����
		if(serial.Baud == 0)
		{
			Gui_StrCenter(X[0],Y[18],WHITE, RED, "baud error!!!",16,0);
		}
		else
		{
			u16 fctemp = POINT_COLOR;//��ʾ������
			u16 bctemp = BACK_COLOR;
			POINT_COLOR = WHITE;
			BACK_COLOR = BLUE;
			LCD_ShowNum(X[7],Y[1], serial.Baud, 7, 16);
			POINT_COLOR = fctemp;
			BACK_COLOR = bctemp;
			serial_Change13Baud(serial.Baud);//�޸Ĵ���13������
		}
		return 0;
	}
	serial.Baud = Baud[dir_flag-1];
	u16 fctemp = POINT_COLOR;
	u16 bctemp = BACK_COLOR;
	POINT_COLOR = WHITE;
	BACK_COLOR = BLUE;
	LCD_ShowNum(X[7],Y[1], serial.Baud, 7, 16);
	POINT_COLOR = fctemp;
	BACK_COLOR = bctemp;
	serial_Change13Baud(serial.Baud);//�޸Ĵ���13������
	return 0;
}
u8 menu2_showmode(void)
{
	u8 dir_flag = 0;//Ŀ¼����ֵ
	Show_Str(X[10],Y[5],BLACK,WHITE, "�ַ���ʾ",16,0);
	Show_Str(X[10],Y[7],BLACK,WHITE, "HEX��ʾ",16,0);
	myLCD_ShowString(X[8], Y[5], ">");
	dir_flag = ChooseDir(2);
	if(dir_flag==0){
		ClearMenu();return 0;
	}
	serial.ShowMode = dir_flag - 1;
	Show_Str(X[26],Y[1],WHITE,BLUE, ShowMode[serial.ShowMode],16,0);
	return 0;
}
u8 menu2_processmode(void)
{
	u8 dir_flag = 0;//Ŀ¼����ֵ
	Show_Str(X[10],Y[5],BLACK,WHITE, "��Ļ��ʾ����",16,0);
	Show_Str(X[10],Y[7],BLACK,WHITE, "ת��������",16,0);
	Show_Str(X[10],Y[9],BLACK,WHITE, "�洢��TF��",16,0);
	myLCD_ShowString(X[8], Y[5], ">");
	dir_flag = ChooseDir(3);
	if(dir_flag==0){
		ClearMenu();return 0;
	}
	serial.ProcessMode = dir_flag - 1;
	Show_Str(X[5],Y[0],WHITE,BLUE, ProcessMode[serial.ProcessMode],16,0);
	if(dir_flag == 2)
	{
		menu3_SendToPC();
	}
	return 0;
}
void ShowCursor(u8 flag)
{
	switch(flag)
	{
		//������µĹ��
		case 1://���ָ���1��ѡ��
		{
			Show_Str(X[8],Y[5],BLACK,WHITE, ">",16,0);
			Show_Str(X[8],Y[7],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[9],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[11],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[13],BLACK,WHITE, " ",16,0);
			break;
		}
		case 2://���ָ���2��ѡ��
		{
			Show_Str(X[8],Y[5],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[7],BLACK,WHITE, ">",16,0);
			Show_Str(X[8],Y[9],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[11],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[13],BLACK,WHITE, " ",16,0);
			break;
		}
		case 3://���ָ���3��ѡ��
		{
			Show_Str(X[8],Y[5],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[7],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[9],BLACK,WHITE, ">",16,0);
			Show_Str(X[8],Y[11],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[13],BLACK,WHITE, " ",16,0);
			break;
		}
		case 4:
		{
			Show_Str(X[8],Y[5],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[7],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[9],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[11],BLACK,WHITE, ">",16,0);
			Show_Str(X[8],Y[13],BLACK,WHITE, " ",16,0);
			break;
		}
		case 5:
		{
			Show_Str(X[8],Y[5],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[7],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[9],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[11],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[13],BLACK,WHITE, ">",16,0);
			break;
		}
	}
}
void ClearMenu(void)
{
	LCD_Fill(X[2]+1, Y[4]+1, X[28]-1, Y[14]-1,WHITE);
	
	LCD_Fill(X[0], Y[18], lcddev.width-1, Y[19]-1,WHITE);
} 
/*****************************************************************************
 * @function   :ѡ��Ŀ¼
 * @parameters :
		DirNum :Ŀ¼����
 * @retvalue   :ѡ���Ŀ¼���
******************************************************************************/ 
u8 ChooseDir(u8 DirNum)
{
	u8 KeyNum = 0;
	u8 MenuFlag = 1;
	while(1)
	{
		KeyNum = myKey_GetNum();
		if(KeyNum != 0)
		{
			switch(KeyNum)
			{
				case 1:
				{	
					return 0;
				}
				case 2://ѡ����һ��ѡ��
				{
					MenuFlag++;
					if(MenuFlag > DirNum)
					{
						MenuFlag = 1;
					}
					ShowCursor(MenuFlag);
					break;
				}
				case 3://ѡ����һ��ѡ��
				{
					MenuFlag--;
					if(MenuFlag < 1)
					{
						MenuFlag = DirNum;
					}
					ShowCursor(MenuFlag);
					break;
				}
				case 4:
				{
					ClearMenu();
					return MenuFlag;
				}
			}
		}
	}
}
void menu3_SendToPC(void)
{
	u8 dir_flag = 0;//Ŀ¼����ֵ
	while(1)
	{
		Show_Str(X[10],Y[5],BLACK,WHITE, "���Խ��ղ�����",16,0);
		myLCD_ShowString(X[8], Y[5], ">");
		dir_flag = ChooseDir(1);
		if(dir_flag==0){
			ClearMenu();return;
		}
		if(dir_flag == 1)
		{
			menu4_toPCBaud();
		}
	}
}
//�޸ķ��͸����ԵĲ�����
void menu4_toPCBaud(void)
{
	u8 dir_flag = 0;//Ŀ¼����ֵ
	myLCD_ShowString(X[10], Y[5], "4800");
	myLCD_ShowString(X[10], Y[7], "9600");
	myLCD_ShowString(X[10], Y[9], "38400");
	myLCD_ShowString(X[10], Y[11], "115200");
	Show_Str(X[10],Y[13],BLACK,WHITE, "�Զ���",16,0);
	myLCD_ShowString(X[8], Y[5], ">");
	dir_flag = ChooseDir(BaudModeNum+1);
	if(dir_flag==0){
		ClearMenu();return;
	}
	if(dir_flag == 5)//�Զ��岨����
	{
		ClearMenu();
		serial.toPCBaud = user_define();//�Զ��岨����
		if(serial.toPCBaud == 0)
		{
			Gui_StrCenter(X[0],Y[18],WHITE, RED, "baud error!!!",16,0);
		}
		else
		{
			serial_Change2Baud(serial.toPCBaud);//�޸Ĵ���2������
		}
		return;
	}
	serial.toPCBaud = Baud[dir_flag-1];
	serial_Change2Baud(serial.toPCBaud);
}
//���أ��Զ��岨����
u32 user_define(void)
{
	u8 KeyNum = 0;
	u32 baud = 0;
	u8 baudarr[7] = {0};//����������
	u8 place = 0;//λ�ñ�־
	myLCD_ShowString(X[11], Y[7], "0000000");
	LCD_ShowChar(X[11+place],Y[7],WHITE, BLUE, baudarr[place]+'0',16,0);
	Show_Str(X[0],Y[19],WHITE, BLUE, "K1:����|K2:�� |K3:+  |K4:-   ",16,0);
	while(1)
	{
		KeyNum = myKey_GetNum();
		if(KeyNum != 0)
		{
			switch(KeyNum)
			{
				case 1://���أ�ȷ��
				{	
					for(u8 i = 0; i < 7; i++)
					{
						baud += baudarr[i]*mypow(10, 6-i);
					}
					ClearMenu();
					Show_Str(X[0],Y[19],WHITE, BLUE, "K1:����|K2:�� |K3:�� |K4:ȷ��",16,0);
					if(baud > 2000000)//����������
					{
						return 0;
					}
					return baud;
				}
				case 2://ѡ����һ��ѡ��
				{
					u8 pre_place = place;
					place++;
					if(place > 6)
					{
						place = 0;
					}
					LCD_ShowChar(X[11+pre_place],Y[7],BLACK, WHITE, baudarr[pre_place]+'0',16,0);
					LCD_ShowChar(X[11+place],Y[7],WHITE, BLUE, baudarr[place]+'0',16,0);
					break;
				}
				case 3://ѡ����һ��ѡ��
				{
					baudarr[place]++;
					if(baudarr[place]>=10)
					{
						baudarr[place] = 0;
					}
					LCD_ShowChar(X[11+place],Y[7],WHITE, BLUE, baudarr[place]+'0',16,0);
					break;
				}
				case 4:
				{
					baudarr[place]--;
					if(baudarr[place]>=10)//u8����0-1=255
					{
						baudarr[place] = 9;
					}
					LCD_ShowChar(X[11+place],Y[7],WHITE, BLUE, baudarr[place]+'0',16,0);
					break;
				}
			}
		}
	}
}
