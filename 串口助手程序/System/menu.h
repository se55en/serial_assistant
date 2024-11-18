#ifndef _MENU_H
#define _MENU_H

#include "lcd.h"
#include "gui.h"
#include "myLCD.h"
#include "myKey.h"
#include "Serial.h"
#include "stmflash.h"

extern Serial_TypeDef serial;
extern const u16 X[30];
extern const u16 Y[20];
u8 Menu(void);
void menu2_baud(void);
void menu2_showmode(void);
void menu2_processmode(void);
void menu2_other(u8 flag);
void Menu_ShowHead(void);
u32 user_define(void);

#endif
