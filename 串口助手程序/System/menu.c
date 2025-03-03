#include "menu.h"


#define BaudModeNum 4
#define USART_13 0
#define USART_2  1 
const u32 Baud[BaudModeNum] = {4800, 9600, 38400, 115200};
u8 ShowMode[2][5] = {"字符", "Hex "};
u8 ProcessMode[3][20] = {"屏幕显示数据", "转发到电脑  ", "存储到TF卡  "};

void ShowCursor(u8 flag);
void ClearMenu(void);
u8 ChooseDir(u8 DirNum);
u8 ChooseDir2(u8 DirNum, u8 menuflag);//不清屏版本
void menu3_SendToPC(void);
void menu4_toPCBaud(void);
u32 user_define(void);

void menu3_databit(u8 flag);
void menu3_stopbit(u8 flag);
void menu3_paritybit(u8 flag);

void Menu_ShowHead(void)//头尾页面
{
	LCD_Fill(0,0,lcddev.width,31,BLUE);//0-31
	LCD_Fill(0,lcddev.height-16,lcddev.width,lcddev.height-1,BLUE);//后16
	Show_Str(X[0],Y[0],WHITE, BLUE, "模式:",16,0);
	Show_Str(X[5],Y[0],WHITE,BLUE, ProcessMode[serial.ProcessMode],16,0);
	Show_Str(X[0],Y[1],WHITE, BLUE, "波特率:",16,0);
	u16 fctemp = POINT_COLOR;
	u16 bctemp = BACK_COLOR;
	POINT_COLOR = WHITE;
	BACK_COLOR = BLUE;
	LCD_ShowNum(X[7],Y[1], serial.Baud, 7, 16);
	POINT_COLOR = fctemp;
	BACK_COLOR = bctemp;
	Show_Str(X[17],Y[1],WHITE, BLUE, "显示模式:",16,0);
	Show_Str(X[26],Y[1],WHITE,BLUE, ShowMode[serial.ShowMode],16,0);
	Show_Str(X[0],Y[19],WHITE, BLUE, "K1:返回|K2:↓ |K3:↑ |K4:确定",16,0);
}

u8 Menu(void)
{
//	mf_test();
	LCD_DrawRectangle(X[2], Y[3], X[28], Y[15]);
	LCD_DrawLine(X[2], Y[4], X[28], Y[4]);
	Gui_StrCenter(X[0],Y[3],BLACK,WHITE,"菜单",16,1);//居中显示
	Show_Str(X[10],Y[5],BLACK,WHITE, "接收波特率",16,0);
	Show_Str(X[10],Y[7],BLACK,WHITE, "显示模式",16,0);
	Show_Str(X[10],Y[9],BLACK,WHITE, "处理模式",16,0);
	Show_Str(X[10],Y[11],BLACK,WHITE, "其他配置",16,0);
	myLCD_ShowString(X[8], Y[5], ">");
	Show_Str(X[0],Y[19],WHITE, BLUE, "K1:返回|K2:↓ |K3:↑ |K4:确定",16,0);
	u8 result = ChooseDir(4);
	if(result == 0)
	{
		//完全清除菜单
		LCD_Fill(0, Y[2], 240, 303,WHITE);
		Show_Str(X[3],Y[19],WHITE, BLUE, "菜单",16,0);
	}
	return result;
}


void menu2_baud(void)
{
	u8 dir_flag = 0;//目录返回值
	myLCD_ShowString(X[10], Y[5], "4800");
	myLCD_ShowString(X[10], Y[7], "9600");
	myLCD_ShowString(X[10], Y[9], "38400");
	myLCD_ShowString(X[10], Y[11], "115200");
	Show_Str(X[10],Y[13],BLACK,WHITE, "自定义",16,0);
	myLCD_ShowString(X[8], Y[5], ">");
	dir_flag = ChooseDir(BaudModeNum+1);
	if(dir_flag==0){
		ClearMenu();return;
	}
	if(dir_flag == 5)//自定义波特率
	{
		ClearMenu();
		serial.Baud = user_define();//自定义波特率
		if(serial.Baud == 0)
		{
			Gui_StrCenter(X[0],Y[18],WHITE, RED, "baud error!!!",16,0);
		}
		else
		{
			u16 fctemp = POINT_COLOR;//显示波特率
			u16 bctemp = BACK_COLOR;
			POINT_COLOR = WHITE;
			BACK_COLOR = BLUE;
			LCD_ShowNum(X[7],Y[1], serial.Baud, 7, 16);
			POINT_COLOR = fctemp;
			BACK_COLOR = bctemp;
			serial_Change13Baud(serial.Baud);//修改串口13波特率
			data_to_string(&serial, Flash_Buffer);
			STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Flash_Buffer,SIZE);
		}
		return;
	}
	serial.Baud = Baud[dir_flag-1];
	u16 fctemp = POINT_COLOR;
	u16 bctemp = BACK_COLOR;
	POINT_COLOR = WHITE;
	BACK_COLOR = BLUE;
	LCD_ShowNum(X[7],Y[1], serial.Baud, 7, 16);
	POINT_COLOR = fctemp;
	BACK_COLOR = bctemp;
	serial_Change13Baud(serial.Baud);//修改串口13波特率
	data_to_string(&serial, Flash_Buffer);
	STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Flash_Buffer,SIZE);
	return;
}
void menu2_showmode(void)
{
	u8 dir_flag = 0;//目录返回值
	Show_Str(X[10],Y[5],BLACK,WHITE, "字符显示",16,0);
	Show_Str(X[10],Y[7],BLACK,WHITE, "HEX显示",16,0);
	myLCD_ShowString(X[8], Y[5], ">");
	dir_flag = ChooseDir(2);
	if(dir_flag==0){
		ClearMenu();return;
	}
	serial.ShowMode = dir_flag - 1;
	Show_Str(X[26],Y[1],WHITE,BLUE, ShowMode[serial.ShowMode],16,0);
	data_to_string(&serial, Flash_Buffer);
	STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Flash_Buffer,SIZE);
	return;
}
void menu2_processmode(void)
{
	u8 dir_flag = 0;//目录返回值
	Show_Str(X[10],Y[5],BLACK,WHITE, "屏幕显示数据",16,0);
	Show_Str(X[10],Y[7],BLACK,WHITE, "转发到电脑",16,0);
	Show_Str(X[10],Y[9],BLACK,WHITE, "存储到TF卡",16,0);
	myLCD_ShowString(X[8], Y[5], ">");
	dir_flag = ChooseDir(3);
	if(dir_flag==0){
		ClearMenu();return;
	}
	serial.ProcessMode = dir_flag - 1;
	Show_Str(X[5],Y[0],WHITE,BLUE, ProcessMode[serial.ProcessMode],16,0);
	data_to_string(&serial, Flash_Buffer);
	STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Flash_Buffer,SIZE);
	if(dir_flag == 2)
	{
		menu3_SendToPC();
	}
	return;
}
void menu2_other(u8 flag)//修改哪一个串口；0是串口13；1是串口2
{
	u8 dir_flag = 1;//目录返回值
	struct usart_config{
		u8 databit;
		u8 stopbit;
		u8 paritybit;
	}config;
	if(flag)
	{
		config.databit = serial.PCDataBit;
		config.stopbit = serial.PCStopBit;
		config.paritybit = serial.PCParityBit;
	}
	else
	{
		config.databit = serial.DataBit;
		config.stopbit = serial.StopBit;
		config.paritybit = serial.ParityBit;
	}
	Show_Str(X[10],Y[5],BLACK,WHITE, "数据位:",16,0);
	Show_Str(X[10],Y[7],BLACK,WHITE, "停止位:",16,0);
	Show_Str(X[10],Y[9],BLACK,WHITE, "校验位:",16,0);
	Show_Str(X[0],Y[19],WHITE, BLUE, "K1:返回|K2:↓ |K3:↑ |K4:修改",16,0);
	switch(config.databit)
	{
		case 0:Show_Str(X[18],Y[5],BLACK,WHITE, "8b",16,0);;break;
		case 1:Show_Str(X[18],Y[5],BLACK,WHITE, "9b",16,0);;break;
	}
	switch(config.stopbit)
	{
		case 0:Show_Str(X[18],Y[7],BLACK,WHITE, "0.5",16,0);;break;
		case 1:Show_Str(X[18],Y[7],BLACK,WHITE, "1  ",16,0);;break;
		case 2:Show_Str(X[18],Y[7],BLACK,WHITE, "1.5",16,0);;break;
		case 3:Show_Str(X[18],Y[7],BLACK,WHITE, "2  ",16,0);;break;
	}
	switch(config.paritybit)
	{
		case 0:Show_Str(X[18],Y[9],BLACK,WHITE, "None",16,0);;break;
		case 1:Show_Str(X[18],Y[9],BLACK,WHITE, "Even",16,0);;break;
		case 2:Show_Str(X[18],Y[9],BLACK,WHITE, "Odd ",16,0);;break;
	}
	myLCD_ShowString(X[8], Y[5], ">");
	while(1)
	{
		dir_flag = ChooseDir2(3, dir_flag);
		if(dir_flag==0){
			serial_ChangeConfig(flag);
			data_to_string(&serial, Flash_Buffer);//保存修改
			STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Flash_Buffer,SIZE);
			ClearMenu();			
			return;
		}
		if(dir_flag == 1){menu3_databit(flag);}
		if(dir_flag == 2){menu3_stopbit(flag);}
		if(dir_flag == 3){menu3_paritybit(flag);}
	}
}
void menu3_databit(u8 flag)
{
	switch(flag)
	{
		case 0:
		{
			serial.DataBit++;
			serial.DataBit %= 2;	
			switch(serial.DataBit)
			{
				case 0:Show_Str(X[18],Y[5],BLACK,WHITE, "8b",16,0);;break;
				case 1:Show_Str(X[18],Y[5],BLACK,WHITE, "9b",16,0);;break;
			}
			break;
		}
		case 1:
		{
			serial.PCDataBit++;
			serial.PCDataBit %= 2;	
			switch(serial.PCDataBit)
			{
				case 0:Show_Str(X[18],Y[5],BLACK,WHITE, "8b",16,0);;break;
				case 1:Show_Str(X[18],Y[5],BLACK,WHITE, "9b",16,0);;break;
			}
			break;
		}
	}
}
void menu3_stopbit(u8 flag)
{
	switch(flag)
	{
		case 0:
		{
			serial.StopBit++;
			serial.StopBit %= 4;
			switch(serial.StopBit)
			{
				case 0:Show_Str(X[18],Y[7],BLACK,WHITE, "0.5",16,0);;break;
				case 1:Show_Str(X[18],Y[7],BLACK,WHITE, "1  ",16,0);;break;
				case 2:Show_Str(X[18],Y[7],BLACK,WHITE, "1.5",16,0);;break;
				case 3:Show_Str(X[18],Y[7],BLACK,WHITE, "2  ",16,0);;break;
			}
			break;
		}
		case 1:
		{
			serial.PCStopBit++;
			serial.PCStopBit %= 4;
			switch(serial.PCStopBit)
			{
				case 0:Show_Str(X[18],Y[7],BLACK,WHITE, "0.5",16,0);;break;
				case 1:Show_Str(X[18],Y[7],BLACK,WHITE, "1  ",16,0);;break;
				case 2:Show_Str(X[18],Y[7],BLACK,WHITE, "1.5",16,0);;break;
				case 3:Show_Str(X[18],Y[7],BLACK,WHITE, "2  ",16,0);;break;
			}
			break;
		}
	}
}
void menu3_paritybit(u8 flag)
{

	switch(flag)
	{
		case 0:
		{
			serial.ParityBit++;
			serial.ParityBit %= 3;
			switch(serial.ParityBit)
			{
				case 0:Show_Str(X[18],Y[9],BLACK,WHITE, "None",16,0);;break;
				case 1:Show_Str(X[18],Y[9],BLACK,WHITE, "Even",16,0);;break;
				case 2:Show_Str(X[18],Y[9],BLACK,WHITE, "Odd ",16,0);;break;
			}
			break;
		}
		case 1:
		{
			serial.PCParityBit++;
			serial.PCParityBit %= 3;
			switch(serial.PCParityBit)
			{
				case 0:Show_Str(X[18],Y[9],BLACK,WHITE, "None",16,0);;break;
				case 1:Show_Str(X[18],Y[9],BLACK,WHITE, "Even",16,0);;break;
				case 2:Show_Str(X[18],Y[9],BLACK,WHITE, "Odd ",16,0);;break;
			}
			break;
		}
	}
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
			Show_Str(X[8],Y[13],BLACK,WHITE, " ",16,0);
			break;
		}
		case 2://光标指向第2个选项
		{
			Show_Str(X[8],Y[5],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[7],BLACK,WHITE, ">",16,0);
			Show_Str(X[8],Y[9],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[11],BLACK,WHITE, " ",16,0);
			Show_Str(X[8],Y[13],BLACK,WHITE, " ",16,0);
			break;
		}
		case 3://光标指向第3个选项
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
u8 ChooseDir2(u8 DirNum, u8 menuflag)//不清屏版本
{
	u8 KeyNum = 0;
	u8 MenuFlag = menuflag;
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
		Show_Str(X[10],Y[5],BLACK,WHITE, "电脑接收波特率",16,0);
		Show_Str(X[10],Y[7],BLACK,WHITE, "其他配置",16,0);
		myLCD_ShowString(X[8], Y[5], ">");
		dir_flag = ChooseDir(2);
		if(dir_flag==0){
			ClearMenu();return;
		}
		if(dir_flag == 1)
		{
			menu4_toPCBaud();
		}
		if(dir_flag == 2)
		{
			menu2_other(1);
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
	Show_Str(X[10],Y[13],BLACK,WHITE, "自定义",16,0);
	myLCD_ShowString(X[8], Y[5], ">");
	dir_flag = ChooseDir(BaudModeNum+1);
	if(dir_flag==0){
		ClearMenu();return;
	}
	if(dir_flag == 5)//自定义波特率
	{
		ClearMenu();
		serial.toPCBaud = user_define();//自定义波特率
		if(serial.toPCBaud == 0)
		{
			Gui_StrCenter(X[0],Y[18],WHITE, RED, "baud error!!!",16,0);
		}
		else
		{
			serial_Change2Baud(serial.toPCBaud);//修改串口2波特率
			data_to_string(&serial, Flash_Buffer);
			STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Flash_Buffer,SIZE);
		}
		return;
	}
	serial.toPCBaud = Baud[dir_flag-1];
	serial_Change2Baud(serial.toPCBaud);//修改串口2波特率
	data_to_string(&serial, Flash_Buffer);
	STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Flash_Buffer,SIZE);
}
//返回，自定义波特率
u32 user_define(void)
{
	u8 KeyNum = 0;
	u32 baud = 0;
	u8 baudarr[7] = {0};//波特率数组
	u8 place = 0;//位置标志
	myLCD_ShowString(X[11], Y[7], "0000000");
	LCD_ShowChar(X[11+place],Y[7],WHITE, BLUE, baudarr[place]+'0',16,0);
	Show_Str(X[0],Y[19],WHITE, BLUE, "K1:返回|K2:→ |K3:+  |K4:-   ",16,0);
	while(1)
	{
		KeyNum = myKey_GetNum();
		if(KeyNum != 0)
		{
			switch(KeyNum)
			{
				case 1://返回，确认
				{	
					for(u8 i = 0; i < 7; i++)
					{
						baud += baudarr[i]*mypow(10, 6-i);
					}
					ClearMenu();
					Show_Str(X[0],Y[19],WHITE, BLUE, "K1:返回|K2:↓ |K3:↑ |K4:确定",16,0);
					if(baud > 2000000)//波特率有误
					{
						return 0;
					}
					return baud;
				}
				case 2://选择下一个选项
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
				case 3://选择上一个选项
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
					if(baudarr[place]>=10)//u8类型0-1=255
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
