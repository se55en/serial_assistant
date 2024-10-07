
#ifndef __MY_KEY_H
#define __MY_KEY_H 			   
#include "stm32f10x.h"
const u16 X[30]={
0, 8*1, 8*2, 8*3, 8*4, 8*5, 8*6, 8*7, 8*8, 8*9, 
8*10, 8*11, 8*12, 8*13, 8*14, 8*15, 8*16, 8*17, 8*18, 8*19, 
8*20, 8*21, 8*22, 8*23, 8*24, 8*25, 8*26, 8*27, 8*28, 8*29
};
const u16 Y[20]={
0, 16*1, 16*2, 16*3, 16*4, 16*5, 16*6, 16*7, 16*8, 16*9, 
16*10, 16*11, 16*12, 16*13, 16*14, 16*15, 16*16, 16*17, 16*18, 16*19
};

void MyKey_Init(void);
uint8_t MyKey_GetNum(void);
void MY_KeyScan(void);
//参数1，显示数组地址
//参数2，显示长度
//参数3，已经显示的长度
void Assistant_2ShowBuff(u8* buff, u16 len, u16 show_num);
void Assistant_1ShowBuff(u8* buff, u16 len, u16 show_num);
#endif





























