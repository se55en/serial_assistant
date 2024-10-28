#ifndef __MF_H
#define __MF_H 
#include "stm32f10x.h"                  // Device header
#include "ff.h"
#include "stdio.h"
#include "serial.h"
#include "delay.h"
#include "mmc_sd.h"
#include "mylcd.h"

extern FATFS fs;
extern FIL fs_file;            /* File object */
extern UINT fs_bw, fs_br;         //读写数量   /* Bytes written */
extern BYTE fs_work[FF_MAX_SS]; /* Work area (larger is better for processing time) */
extern BYTE fs_buffer[4096];  //文件缓冲区 /* File copy buffer */

void mf_test(void);
u8 exf_getfree(u8 *drv,u32 *total,u32 *free);

#endif

