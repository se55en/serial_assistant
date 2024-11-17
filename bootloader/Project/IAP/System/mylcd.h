#ifndef _MYLCD_H
#define _MYLCD_H
#include "stm32f10x.h"                  // Device header
#include "delay.h"
#include "sys.h"
#include "lcd.h"
#include "gui.h"
#include "test.h"

extern const u16 X[30];
extern const u16 Y[20];

void myLCD_Power_Init(void);
void myLCD_Power_Off(void);
void myLCD_Power_On(void);

void myLCD_Init(void);
void myLCD_ShowString(u16 x, u16 y, u8 *str);
void myLCD_ShowChar(u16 x,u16 y,u8 num);
void myLCD_ShowHexNum(u16 x,u16 y, u8 num);
#endif
