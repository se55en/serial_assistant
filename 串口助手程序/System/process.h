#ifndef _PROCESS_H
#define _PROCESS_H

#include "serial.h"
#include "menu.h"
#include "mf.h"

extern const u16 X[30];
extern const u16 Y[20];

int adgf;

void process(u8 mode);
u8 AddX(u8 add);
u8 AddY(u8 add);
#endif
