#include "process.h"
extern Serial_TypeDef serial;

typedef struct
{
	u16 no_display_num;//未显示的数
	u16 all_num;//0为字符串，1为Hex
	u8 ProcessMode;//0为屏幕显示，1为转发给pc，2为sd卡存储
}Show_TypeDef;
//屏幕显示位置坐标
#define X_MIN 		0
#define X_MAX 		29
#define Y_MIN 		2
#define Y_MAX 		19

u8 x = 0, y = 2;//光标位置
u8 first_flag = 1;//第一次不用回车换行
//屏幕显示相关函数
u8 ScreenDisplay(void);
void ShowHead1(void);
void ShowHead2(void);
u8 AddX(u8 add);
u8 AddY(u8 add);
void Show1(void);
void Show2(void);
//转发到电脑相关函数
void SendToPC(void);
//转发到SD卡相关函数
void SendToSD(void);
void my_f_write(uint8_t *arr, uint16_t len);

void process(u8 mode)//处理数据模式
{
	if(mode == 0){ScreenDisplay();}
	if(mode == 1){SendToPC();}
	if(mode == 2){SendToSD();}
}

u8 ScreenDisplay(void)
{
	u8 KeyNum = 0;
	x = 0, y = 2;//光标位置
	first_flag = 1;
	while(1)
	{
		KeyNum = myKey_GetNum();
		if(KeyNum == 1)
		{
			//清空显示区
			LCD_Fill(0, Y[2]+1, 240, 320,WHITE);
			return 1;
		}
		if(serial.USART1_NoDisplayLen)//有没显示的数据，则显示开始显示数据
		{
			Show1();
			uart1DmaClear();    // 清空DMA接收通道
			serial.USART1_RecvBuffLen = 0;
			serial.USART1_NoDisplayLen = 0;
			serial.USART1_DisplayLen = 0;
		}
		//显示2发送的数据
		if(serial.USART3_NoDisplayLen)//有没显示的数据，则显示开始显示数据
		{
			Show2();
			uart3DmaClear();    // 清空DMA接收通道
			serial.USART3_RecvBuffLen = 0;
			serial.USART3_NoDisplayLen = 0;
			serial.USART3_DisplayLen = 0;
		}
	}
}
void SendToPC(void)
{
	u8 KeyNum = 0;
	while(1)
	{
		//按键返回
		KeyNum = myKey_GetNum();
		if(KeyNum == 1)
		{
			//清空显示区
			LCD_Fill(0, Y[2]+1, 240, 320,WHITE);
			return;
		}
		//发送数据给pc
		if(serial.USART1_RecvBuffLen)//有没显示的数据，则显示开始发送数据
		{
			uart2SendArray("[1>>]:", 6);
			uart2SendArray(serial.USART1_RecvBuff, serial.USART1_RecvBuffLen);        // 使用DMA发送数据
			uart1DmaClear();    // 清空DMA接收通道
			serial.USART1_RecvBuffLen = 0;
			serial.USART1_NoDisplayLen = 0;
			serial.USART1_DisplayLen = 0;
		}
		if(serial.USART3_RecvBuffLen)//有没显示的数据，则显示开始发送数据
		{
			uart2SendArray("[2>>]:", 6);
			uart2SendArray(serial.USART3_RecvBuff, serial.USART3_RecvBuffLen);        // 使用DMA发送数据
			uart3DmaClear();    // 清空DMA接收通道
			serial.USART3_RecvBuffLen = 0;
			serial.USART3_NoDisplayLen = 0;
			serial.USART3_DisplayLen = 0;
		}
	}
}
void SendToSD(void)
{
	u8 res_sd = 0;
	static u8 init_flag = 0;//初始化标志，0表示没初始化 
	u8 first_flag = 0;//第一次进入函数
	u8 KeyNum = 0;
	while(1)
	{
		//按键返回
		KeyNum = myKey_GetNum();
		if(KeyNum == 1)
		{
			f_close(&fs_file);//关闭文件
			//清空显示区
			LCD_Fill(0, Y[2]+1, 240, 320,WHITE);
			return;
		}
		//sd卡操作
		if(!init_flag)//初始化sd卡
		{
			while(SD_Initialize())//检测不到SD卡
			{
				myLCD_ShowString(X[5], Y[5], "SD Card Error!");
				delay_ms(500);					
				myLCD_ShowString(X[5], Y[5], "Please Check! ");
				delay_ms(500);
			}
			myLCD_ShowString(X[5], Y[5], "SD Card Init Success");
			res_sd = f_mount(&fs,"0:",1); //挂载SD卡 
			//打印信息
			sprintf(print_buf, "\nres_sd = %d\n",res_sd);uart2SendArray((u8*)print_buf, strlen(print_buf));
			if(res_sd)
			{
				myLCD_ShowString(X[5], Y[19], "SD Mount Fail");
			}
			/*----------------------- 格式化测试 ---------------------------*/
			/* 如果没有文件系统就格式化创建创建文件系统 */
			if (res_sd == FR_NO_FILESYSTEM) 
			{
				res_sd=f_mkfs("0:", 0, fs_work, sizeof(fs_work));
				sprintf(print_buf, "\nres_sd = %d\n",res_sd);uart2SendArray((u8*)print_buf, strlen(print_buf));
				res_sd = f_mount(&fs,"0:",1); //挂载SD卡 
			}	
			init_flag = 1;
		}
		if(!first_flag)
		{
			u32 total = 0,free = 0;
			exf_getfree((u8*)"0:",&total,&free);	//得到SD卡的总容量和剩余容量												 
			sprintf(print_buf, "SD Total Size:%d MB\n",total>>10);
			myLCD_ShowString(X[5], Y[10], (u8*)print_buf);
			sprintf(print_buf, "SD Free Size: %d MB\n",free>>10);
			myLCD_ShowString(X[5], Y[11], (u8*)print_buf);
			
			/* 打开文件，如果文件不存在则创建它 */
			res_sd=f_open(&fs_file,"0:usart_data.txt",FA_OPEN_APPEND|FA_WRITE);//"a",追加文件
			sprintf(print_buf, "f_open res_sd = %d\n", res_sd);uart2SendArray((u8*)print_buf, strlen(print_buf));
			if(res_sd == FR_OK )
			{
				sprintf(print_buf, "打开/创建文件成功\r\n");uart2SendArray((u8*)print_buf, strlen(print_buf));
			} 
			else
			{
				sprintf(print_buf, "打开/创建文件失败。\r\n");uart2SendArray((u8*)print_buf, strlen(print_buf));
			}
			first_flag = 1;
		}
		if(serial.USART1_RecvBuffLen)//有没显示的数据，则显示开始发送数据
		{
			strcpy((char *)fs_buffer, "\n[1>>]:");
			res_sd=f_write(&fs_file, fs_buffer, strlen((char *)fs_buffer),&fs_bw);
			my_f_write(serial.USART1_RecvBuff, serial.USART1_RecvBuffLen);        //写数据
			uart1DmaClear();    // 清空DMA接收通道
			serial.USART1_RecvBuffLen = 0;
			serial.USART1_NoDisplayLen = 0;
			serial.USART1_DisplayLen = 0;
		}
		if(serial.USART3_RecvBuffLen)//有没显示的数据，则显示开始发送数据
		{
			strcpy((char *)fs_buffer, "\n[2>>]:");
			res_sd=f_write(&fs_file, fs_buffer, strlen((char *)fs_buffer),&fs_bw);
			my_f_write(serial.USART3_RecvBuff, serial.USART3_RecvBuffLen);        //写数据
			uart3DmaClear();    // 清空DMA接收通道
			serial.USART3_RecvBuffLen = 0;
			serial.USART3_NoDisplayLen = 0;
			serial.USART3_DisplayLen = 0;
		}
	}
}

void my_f_write(uint8_t *arr, uint16_t len)
{
//	u8 res_sd;
	if(len == 0)	// 判断长度是否有效
    return;
	
    if(arr) 
	{
		f_write(&fs_file, arr, len,&fs_bw);
//		if (res_sd==FR_OK) 
//		{
//			sprintf(print_buf, "文件写入成功，写入字节数据：%d\n",fs_bw);uart2SendArray((u8*)print_buf, strlen(print_buf));
//        }
//		else
//		{
//			sprintf(print_buf, "！！文件写入失败：(%d)\n",res_sd);uart2SendArray((u8*)print_buf, strlen(print_buf));
//        }
	}
}
//显示数据头
void ShowHead1(void)
{
	if(first_flag == 1)
	{
		first_flag = 0;
	}
	else
	{
		x = 0;//回车换行
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
		x = 0;//回车换行
		AddY(1);
	}

	myLCD_ShowString(X[x], Y[y], "[2>>:]");
	AddX(6);
}
//移动光标,返回清屏标志,1为清屏
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
				//清空显示区
				LCD_Fill(0, Y[2]+1, 240-1, 320-1,WHITE);
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
			//清空显示区
			LCD_Fill(0, Y[2]+1, 240-1, 320-1,WHITE);
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
		if(serial.ShowMode == 0)//字符显示
		{
			//判断是不是非法字符
			if((*buff>=' ') && (*buff<'~'))
			{
				myLCD_ShowChar(X[x],Y[y],*buff);
				AddX(1);
			}
		}
		else if(serial.ShowMode == 1)//hex显示
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
		if(serial.ShowMode == 0)//字符显示
		{
			//判断是不是非法字符
			if((*buff>=' ') && (*buff<'~'))
			{
				myLCD_ShowChar(X[x],Y[y],*buff);
				AddX(1);
			}
		}
		else if(serial.ShowMode == 1)//hex显示
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
