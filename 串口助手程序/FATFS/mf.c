#include "mf.h"
#include "myKey.h"
#define FS_BUFF 2048
FATFS fs_FatFs;           /* Filesystem object */
FIL fs_file;            /* File object */
UINT fs_bw, fs_br;         //读写数量   /* Bytes written */
BYTE fs_work[FF_MAX_SS]; /* Work area (larger is better for processing time) */
BYTE fs_buffer[FS_BUFF];  //文件缓冲区 /* File copy buffer */

void SD_WriteReadTest(void)
{
	u8 *buf;
	u16 i;
	u8 arr[512];
	buf = arr;
	for(i = 0; i < 512; i++)
	{
		buf[i] = '8';
	}
	SD_Initialize();
	SD_WriteDisk(buf,0,1);
	if(SD_ReadDisk(buf,0,1)==0)	//读取0扇区的内容
	{	
		myLCD_ShowString(X[1], Y[18], "Read   OK");
		uart2SendArray(buf, 512);
	}
}
void mf_test(void)
{
	while(SD_Initialize())//检测不到SD卡
	{
		myLCD_ShowString(X[10], Y[10], "SD Card Error!");
		delay_ms(500);					
		myLCD_ShowString(X[10], Y[10], "Please Check! ");
		delay_ms(500);
	}
	myLCD_ShowString(X[10], Y[10], "SD Card OK    ");
	
	u32 total,free;
	u8 res_sd = 0;
	res_sd = f_mount(&fs_FatFs,"0:",1); //挂载SD卡 
	//打印信息
	sprintf(print_buf, "\nres_sd = %d\n",res_sd);uart2SendArray((u8*)print_buf, strlen(print_buf));
	/*----------------------- 格式化测试 ---------------------------*/
    /* 如果没有文件系统就格式化创建创建文件系统 */
    if (res_sd == FR_NO_FILESYSTEM) 
	{
		sprintf(print_buf, "FATFS no file system\n");
		uart2SendArray((u8*)print_buf, strlen(print_buf));
		res_sd=f_mkfs("", 0, fs_work, sizeof(fs_work));
		if(res_sd == FR_OK)
		{
			sprintf(print_buf, "FATFS 格式化成功\n");
			uart2SendArray((u8*)print_buf, strlen(print_buf));
		}
		res_sd = f_mount(&fs_FatFs,"0:",1); //挂载SD卡 
		sprintf(print_buf, "\nres_sd = %d\n",res_sd);uart2SendArray((u8*)print_buf, strlen(print_buf));
	}	
	while(exf_getfree("0:",&total,&free))	//得到SD卡的总容量和剩余容量
	{
		sprintf(print_buf, "FATFS Error\n");
		uart2SendArray((u8*)print_buf, strlen(print_buf));
		delay_ms(1000);
	}													 
	sprintf(print_buf, "SD Total Size:%d MB\n",total>>10);uart2SendArray((u8*)print_buf, strlen(print_buf));
	sprintf(print_buf, "SD Free Size:%d MB\n",free>>10);uart2SendArray((u8*)print_buf, strlen(print_buf));
	/*--------------------- 文件系统测试：写测试 -----------------------*/
    /* 打开文件，如果文件不存在则创建它 */
	sprintf(print_buf, "\r\n****** 即将进行文件写入测试... ******\r\n");uart2SendArray((u8*)print_buf, strlen(print_buf));
    res_sd=f_open(&fs_file,"0:test.txt",FA_CREATE_ALWAYS|FA_WRITE);//"w"
	sprintf(print_buf, "\nf_open     res_sd = %d\n",res_sd);uart2SendArray((u8*)print_buf, strlen(print_buf));
    if(res_sd == FR_OK )
	{
		sprintf(print_buf, "》打开/创建test.txt文件成功，向文件写入数据。\r\n");uart2SendArray((u8*)print_buf, strlen(print_buf));
        /* 将指定存储区内容写入到文件内 */
		strcpy((char *)fs_buffer, "hello FATFS\n");
        res_sd=f_write(&fs_file, fs_buffer, strlen((char *)fs_buffer),&fs_bw);
        if (res_sd==FR_OK) {
			sprintf(print_buf, "》文件写入成功，写入字节数据：%d\n",fs_bw);uart2SendArray((u8*)print_buf, strlen(print_buf));
			sprintf(print_buf, "》向文件写入的数据为：\r\n%s\r\n",fs_buffer);uart2SendArray((u8*)print_buf, strlen(print_buf));
        } else {
			sprintf(print_buf, "！！文件写入失败：(%d)\n",res_sd);uart2SendArray((u8*)print_buf, strlen(print_buf));
        }
        /* 不再读写，关闭文件 */
        f_close(&fs_file);
    } 
	else
	{
		sprintf(print_buf, "！！打开/创建文件失败。\r\n");uart2SendArray((u8*)print_buf, strlen(print_buf));
    }
	/*------------------ 文件系统测试：读测试 --------------------------*/
	sprintf(print_buf, "****** 即将进行文件读取测试... ******\r\n");uart2SendArray((u8*)print_buf, strlen(print_buf));
    res_sd=f_open(&fs_file,"0:test.txt",FA_OPEN_EXISTING|FA_READ);
    if (res_sd == FR_OK) {
		sprintf(print_buf, "》打开文件成功。\r\n");uart2SendArray((u8*)print_buf, strlen(print_buf));
        res_sd = f_read(&fs_file, fs_buffer, sizeof(fs_buffer), &fs_br);
        if (res_sd==FR_OK) {
			sprintf(print_buf, "》文件读取成功,读到字节数据：%d\r\n", fs_br);uart2SendArray((u8*)print_buf, strlen(print_buf));
			sprintf(print_buf, "》读取得的文件数据为：\r\n%s \r\n", fs_buffer);uart2SendArray((u8*)print_buf, strlen(print_buf));
        } else {
			sprintf(print_buf, "！！文件读取失败：(%d)\n",res_sd);uart2SendArray((u8*)print_buf, strlen(print_buf));
        }
    } else {
		sprintf(print_buf, "！！打开文件失败。\r\n");uart2SendArray((u8*)print_buf, strlen(print_buf));
    }
	f_close(&fs_file);
}
void mf_mytest(void)
{
	u8 res_tf = 0;
	while(SD_Initialize())//检测不到SD卡
	{
		myLCD_ShowString(X[10], Y[10], "SD Card Error!");
		delay_ms(500);					
		myLCD_ShowString(X[10], Y[10], "Please Check! ");
		delay_ms(500);
	}
	myLCD_ShowString(X[10], Y[10], "SD Card OK    ");
	res_tf = f_mount(&fs_FatFs,"0:",1); //挂载SD卡 
	sprintf(print_buf, "f_mount res = %d",res_tf);Gui_StrCenter(X[0],Y[2],BLACK, WHITE,(u8*)print_buf,16,0);
	res_tf = f_open(&fs_file,"0:uart.txt",FA_OPEN_APPEND|FA_WRITE|FA_READ);
	sprintf(print_buf, "f_open1 res = %d",res_tf);Gui_StrCenter(X[0],Y[3],BLACK, WHITE,(u8*)print_buf,16,0);
	strcpy((char *)fs_buffer, "hello FATFS\n");
	res_tf=f_write(&fs_file, fs_buffer, strlen((char *)fs_buffer),&fs_bw);
	sprintf(print_buf, "f_write res = %d",res_tf);Gui_StrCenter(X[0],Y[4],BLACK, WHITE,(u8*)print_buf,16,0);
	res_tf = f_close(&fs_file);
	sprintf(print_buf, "f_close res = %d",res_tf);Gui_StrCenter(X[0],Y[5],BLACK, WHITE,(u8*)print_buf,16,0);
	res_tf = f_open(&fs_file,"0:uart.txt",FA_OPEN_APPEND|FA_WRITE|FA_READ);
	sprintf(print_buf, "f_open2 res = %d",res_tf);Gui_StrCenter(X[0],Y[6],BLACK, WHITE,(u8*)print_buf,16,0);
	
}
void mf_write(void)
{
	u8 res_sd;
	f_mount(&fs_FatFs,"0:",0);
	f_open(&fs_file,"0:u.txt",FA_OPEN_APPEND|FA_WRITE);//追加文件
	strcpy((char *)fs_buffer, "recode data...\n");
	res_sd = f_write(&fs_file, fs_buffer, strlen((char *)fs_buffer),&fs_bw);
//	res_sd = f_write(&fs_file, fs_buffer, 15,&fs_bw);
	sprintf(print_buf, "f_write res = %d,bw = %d",res_sd,fs_bw);myLCD_ShowString(X[3], Y[18], (u8*)print_buf);
	f_close(&fs_file);
	f_mount(NULL,"0:",1);//取消挂载
}
void mf_read(void)
{
//	u8 res_sd;
	u32 sum = 0;
//	res_sd = f_mount(&fs_FatFs,"0:",0);
//	sprintf(print_buf, "f_monut res = %d",res_sd);myLCD_ShowString(X[3], Y[15], (u8*)print_buf);
//	res_sd = f_open(&fs_file,"0:u.txt",FA_OPEN_EXISTING|FA_READ);
//	sprintf(print_buf, "f_open res = %d",res_sd);myLCD_ShowString(X[3], Y[17], (u8*)print_buf);
	f_lseek(&fs_file, 0);
	do{
		f_read(&fs_file, fs_buffer, sizeof(fs_buffer), &fs_br);
		sum += fs_br;
	}while(fs_br >= FS_BUFF);
	sprintf(print_buf, "file byte = %d", sum);
	myLCD_ShowString(X[0],Y[16],(u8*)print_buf);
	f_lseek(&fs_file, f_size(&fs_file));
//	f_close(&fs_file);
//	f_mount(NULL,"0:",1);//取消挂载
}
//得到磁盘剩余容量
//drv:磁盘编号("0:"/"1:")
//total:总容量	 （单位KB）
//free:剩余容量	 （单位KB）
//返回值:0,正常.其他,错误代码
u8 exf_getfree(u8 *drv,u32 *total,u32 *free)
{
	FATFS *fs1;
	u8 res;
    u32 fre_clust=0, fre_sect=0, tot_sect=0;
    //得到磁盘信息及空闲簇数量
    res =(u32)f_getfree((const TCHAR*)drv, (DWORD*)&fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect=(fs1->n_fatent-2)*fs1->csize;	//得到总扇区数
	    fre_sect=fre_clust*fs1->csize;			//得到空闲扇区数	   
  
		*total=tot_sect>>1;	//单位为KB
		*free=fre_sect>>1;	//单位为KB 
 	}
	return res;
}





