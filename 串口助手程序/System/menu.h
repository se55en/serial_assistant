#ifndef _MENU_H
#define _MENU_H

#include "lcd.h"
#include "gui.h"
#include "myLCD.h"
#include "myKey.h"
#include "Serial.h"

extern Serial_TypeDef serial;
extern const u16 X[30];
extern const u16 Y[20];
u8 Menu(void);
u8 menu2_baud(void);
u8 menu2_showmode(void);
u8 menu2_processmode(void);
void Menu_ShowHead(void);

#endif
