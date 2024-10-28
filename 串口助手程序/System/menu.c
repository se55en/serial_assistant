#include "menu.h"


#define BaudModeNum 4
const u32 Baud[BaudModeNum] = {4800, 9600, 38400, 115200};
u8 ShowMode[2][10] = {"String", "Hex   "};
u8 ProcessMode[3][20] = {"Screen Display  ", "Send to computer", "Stored on TFcard"};

void ShowCursor(u8 flag);
void ClearMenu(void);
u8 ChooseDir(u8 DirNum);
void menu3_SendToPC(void);
void menu4_toPCBaud(void);

void Menu_ShowHead(void)
{
	myLCD_ShowString(X[0], Y[0], "Process:Screen Display");
	myLCD_ShowString(X[0], Y[1], "Baud:");
	LCD_ShowNum(X[5],Y[1], serial.Baud, 7, 16);
	myLCD_ShowString(X[15], Y[1], "ShowMode:String");
	LCD_DrawLine(0, Y[2]-1, 240-1, Y[2]-1);
}

u8 Menu(void)
{
	LCD_DrawRectangle(X[2], Y[3], X[28], Y[13]);
	LCD_DrawLine(X[2], Y[4], X[28], Y[4]);
	Gui_StrCenter(X[0],Y[3],BLACK,WHITE,"Menu",16,1);//居中显示
	myLCD_ShowString(X[10], Y[5], "Receive Baud");
	myLCD_ShowString(X[10], Y[7], "Show Mode");
	myLCD_ShowString(X[10], Y[9], "Process Mode");
	myLCD_ShowString(X[8], Y[5], ">");
	u8 result = ChooseDir(3);
	if(result == 0)
	{
		//完全清除菜单
		LCD_Fill(0, Y[2], 240, 320,WHITE);
	}
	return result;
}


u8 menu2_baud(void)
{
	u8 dir_flag = 0;//目录返回值
	myLCD_ShowString(X[10], Y[5], "4800");
	myLCD_ShowString(X[10], Y[7], "9600");
	myLCD_ShowString(X[10], Y[9], "38400");
	myLCD_ShowString(X[10], Y[11], "115200");
	myLCD_ShowString(X[8], Y[5], ">");
	dir_flag = ChooseDir(BaudModeNum);
	if(dir_flag==0){
		ClearMenu();return 0;
	}
	serial.Baud = Baud[dir_flag-1];
	LCD_ShowNum(X[5],Y[1], serial.Baud, 7, 16);
	serial_Change13Baud(serial.Baud);//修改串口13波特率
	return 0;
}
u8 menu2_showmode(void)
{
	u8 dir_flag = 0;//目录返回值
	myLCD_ShowString(X[10], Y[5], "String");
	myLCD_ShowString(X[10], Y[7], "Hex");
	myLCD_ShowString(X[8], Y[5], ">");
	dir_flag = ChooseDir(2);
	if(dir_flag==0){
		ClearMenu();return 0;
	}
	serial.ShowMode = dir_flag - 1;
	myLCD_ShowString(X[24], Y[1], ShowMode[serial.ShowMode]);
	return 0;
}
u8 menu2_processmode(void)
{
	u8 dir_flag = 0;//目录返回值
	myLCD_ShowString(X[10], Y[5], "Screen Display  ");
	myLCD_ShowString(X[10], Y[7], "Send to computer");
	myLCD_ShowString(X[10], Y[9], "Stored on TFcard");
	myLCD_ShowString(X[8], Y[5], ">");
	dir_flag = ChooseDir(3);
	if(dir_flag==0){
		ClearMenu();return 0;
	}
	serial.ProcessMode = dir_flag - 1;
	myLCD_ShowString(X[8], Y[0], ProcessMode[serial.ProcessMode]);
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
		//清除上下的光标
		case 1://光标指向第1个选项
		{
			Show_Str(X[8],Y[5],BLACK,WHITE, ">",16,0);
			Show_Str(X[8],Y[7],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[9],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[11],BLACK,WHITE, " ",16,0);
			break;
		}
		case 2://光标指向第2个选项
		{
			Show_Str(X[8],Y[5],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[7],BLACK,WHITE, ">",16,0);
			Show_Str(X[8],Y[9],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[11],BLACK,WHITE, " ",16,0);
			break;
		}
		case 3://光标指向第3个选项
		{
			Show_Str(X[8],Y[5],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[7],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[9],BLACK,WHITE, ">",16,0);
			Show_Str(X[8],Y[11],BLACK,WHITE, " ",16,0);
			break;
		}
		case 4:
		{
			Show_Str(X[8],Y[5],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[7],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[9],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[11],BLACK,WHITE, ">",16,0);
			break;
		}
	}
}
void ClearMenu(void)
{
	LCD_Fill(X[2]+1, Y[4]+1, X[28]-1, Y[12]-1,WHITE);
} 
/*****************************************************************************
 * @function   :选择目录
 * @parameters :
		DirNum :目录个数
 * @retvalue   :选择的目录序号
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
				case 2://选择下一个选项
				{
					MenuFlag++;
					if(MenuFlag > DirNum)
					{
						MenuFlag = 1;
					}
					ShowCursor(MenuFlag);
					break;
				}
				case 3://选择上一个选项
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
	u8 dir_flag = 0;//目录返回值
	while(1)
	{
		myLCD_ShowString(X[10], Y[5], "PC Baud:");
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
//修改发送给电脑的波特率
void menu4_toPCBaud(void)
{
	u8 dir_flag = 0;//目录返回值
	myLCD_ShowString(X[10], Y[5], "4800");
	myLCD_ShowString(X[10], Y[7], "9600");
	myLCD_ShowString(X[10], Y[9], "38400");
	myLCD_ShowString(X[10], Y[11], "115200");
	myLCD_ShowString(X[8], Y[5], ">");
	dir_flag = ChooseDir(BaudModeNum);
	if(dir_flag==0){
		ClearMenu();return;
	}
	serial.toPCBaud = Baud[dir_flag-1];
	serial_Change2Baud(serial.toPCBaud);
}
