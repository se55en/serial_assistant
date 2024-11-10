#include "mf.h"
#include "myKey.h"
#define FS_BUFF 2048
FATFS fs_FatFs;           /* Filesystem object */
FIL fs_file;            /* File object */
UINT fs_bw, fs_br;         //��д����   /* Bytes written */
BYTE fs_work[FF_MAX_SS]; /* Work area (larger is better for processing time) */
BYTE fs_buffer[FS_BUFF];  //�ļ������� /* File copy buffer */

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
	if(SD_ReadDisk(buf,0,1)==0)	//��ȡ0����������
	{	
		myLCD_ShowString(X[1], Y[18], "Read   OK");
		uart2SendArray(buf, 512);
	}
}
void mf_test(void)
{
	while(SD_Initialize())//��ⲻ��SD��
	{
		myLCD_ShowString(X[10], Y[10], "SD Card Error!");
		delay_ms(500);					
		myLCD_ShowString(X[10], Y[10], "Please Check! ");
		delay_ms(500);
	}
	myLCD_ShowString(X[10], Y[10], "SD Card OK    ");
	
	u32 total,free;
	u8 res_sd = 0;
	res_sd = f_mount(&fs_FatFs,"0:",1); //����SD�� 
	//��ӡ��Ϣ
	sprintf(print_buf, "\nres_sd = %d\n",res_sd);uart2SendArray((u8*)print_buf, strlen(print_buf));
	/*----------------------- ��ʽ������ ---------------------------*/
    /* ���û���ļ�ϵͳ�͸�ʽ�����������ļ�ϵͳ */
    if (res_sd == FR_NO_FILESYSTEM) 
	{
		sprintf(print_buf, "FATFS no file system\n");
		uart2SendArray((u8*)print_buf, strlen(print_buf));
		res_sd=f_mkfs("", 0, fs_work, sizeof(fs_work));
		if(res_sd == FR_OK)
		{
			sprintf(print_buf, "FATFS ��ʽ���ɹ�\n");
			uart2SendArray((u8*)print_buf, strlen(print_buf));
		}
		res_sd = f_mount(&fs_FatFs,"0:",1); //����SD�� 
		sprintf(print_buf, "\nres_sd = %d\n",res_sd);uart2SendArray((u8*)print_buf, strlen(print_buf));
	}	
	while(exf_getfree("0:",&total,&free))	//�õ�SD������������ʣ������
	{
		sprintf(print_buf, "FATFS Error\n");
		uart2SendArray((u8*)print_buf, strlen(print_buf));
		delay_ms(1000);
	}													 
	sprintf(print_buf, "SD Total Size:%d MB\n",total>>10);uart2SendArray((u8*)print_buf, strlen(print_buf));
	sprintf(print_buf, "SD Free Size:%d MB\n",free>>10);uart2SendArray((u8*)print_buf, strlen(print_buf));
	/*--------------------- �ļ�ϵͳ���ԣ�д���� -----------------------*/
    /* ���ļ�������ļ��������򴴽��� */
	sprintf(print_buf, "\r\n****** ���������ļ�д�����... ******\r\n");uart2SendArray((u8*)print_buf, strlen(print_buf));
    res_sd=f_open(&fs_file,"0:test.txt",FA_CREATE_ALWAYS|FA_WRITE);//"w"
	sprintf(print_buf, "\nf_open     res_sd = %d\n",res_sd);uart2SendArray((u8*)print_buf, strlen(print_buf));
    if(res_sd == FR_OK )
	{
		sprintf(print_buf, "����/����test.txt�ļ��ɹ������ļ�д�����ݡ�\r\n");uart2SendArray((u8*)print_buf, strlen(print_buf));
        /* ��ָ���洢������д�뵽�ļ��� */
		strcpy((char *)fs_buffer, "hello FATFS\n");
        res_sd=f_write(&fs_file, fs_buffer, strlen((char *)fs_buffer),&fs_bw);
        if (res_sd==FR_OK) {
			sprintf(print_buf, "���ļ�д��ɹ���д���ֽ����ݣ�%d\n",fs_bw);uart2SendArray((u8*)print_buf, strlen(print_buf));
			sprintf(print_buf, "�����ļ�д�������Ϊ��\r\n%s\r\n",fs_buffer);uart2SendArray((u8*)print_buf, strlen(print_buf));
        } else {
			sprintf(print_buf, "�����ļ�д��ʧ�ܣ�(%d)\n",res_sd);uart2SendArray((u8*)print_buf, strlen(print_buf));
        }
        /* ���ٶ�д���ر��ļ� */
        f_close(&fs_file);
    } 
	else
	{
		sprintf(print_buf, "������/�����ļ�ʧ�ܡ�\r\n");uart2SendArray((u8*)print_buf, strlen(print_buf));
    }
	/*------------------ �ļ�ϵͳ���ԣ������� --------------------------*/
	sprintf(print_buf, "****** ���������ļ���ȡ����... ******\r\n");uart2SendArray((u8*)print_buf, strlen(print_buf));
    res_sd=f_open(&fs_file,"0:test.txt",FA_OPEN_EXISTING|FA_READ);
    if (res_sd == FR_OK) {
		sprintf(print_buf, "�����ļ��ɹ���\r\n");uart2SendArray((u8*)print_buf, strlen(print_buf));
        res_sd = f_read(&fs_file, fs_buffer, sizeof(fs_buffer), &fs_br);
        if (res_sd==FR_OK) {
			sprintf(print_buf, "���ļ���ȡ�ɹ�,�����ֽ����ݣ�%d\r\n", fs_br);uart2SendArray((u8*)print_buf, strlen(print_buf));
			sprintf(print_buf, "����ȡ�õ��ļ�����Ϊ��\r\n%s \r\n", fs_buffer);uart2SendArray((u8*)print_buf, strlen(print_buf));
        } else {
			sprintf(print_buf, "�����ļ���ȡʧ�ܣ�(%d)\n",res_sd);uart2SendArray((u8*)print_buf, strlen(print_buf));
        }
    } else {
		sprintf(print_buf, "�������ļ�ʧ�ܡ�\r\n");uart2SendArray((u8*)print_buf, strlen(print_buf));
    }
	f_close(&fs_file);
}
void mf_mytest(void)
{
	u8 res_tf = 0;
	while(SD_Initialize())//��ⲻ��SD��
	{
		myLCD_ShowString(X[10], Y[10], "SD Card Error!");
		delay_ms(500);					
		myLCD_ShowString(X[10], Y[10], "Please Check! ");
		delay_ms(500);
	}
	myLCD_ShowString(X[10], Y[10], "SD Card OK    ");
	res_tf = f_mount(&fs_FatFs,"0:",1); //����SD�� 
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
	f_open(&fs_file,"0:u.txt",FA_OPEN_APPEND|FA_WRITE);//׷���ļ�
	strcpy((char *)fs_buffer, "recode data...\n");
	res_sd = f_write(&fs_file, fs_buffer, strlen((char *)fs_buffer),&fs_bw);
//	res_sd = f_write(&fs_file, fs_buffer, 15,&fs_bw);
	sprintf(print_buf, "f_write res = %d,bw = %d",res_sd,fs_bw);myLCD_ShowString(X[3], Y[18], (u8*)print_buf);
	f_close(&fs_file);
	f_mount(NULL,"0:",1);//ȡ������
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
//	f_mount(NULL,"0:",1);//ȡ������
}
//�õ�����ʣ������
//drv:���̱��("0:"/"1:")
//total:������	 ����λKB��
//free:ʣ������	 ����λKB��
//����ֵ:0,����.����,�������
u8 exf_getfree(u8 *drv,u32 *total,u32 *free)
{
	FATFS *fs1;
	u8 res;
    u32 fre_clust=0, fre_sect=0, tot_sect=0;
    //�õ�������Ϣ�����д�����
    res =(u32)f_getfree((const TCHAR*)drv, (DWORD*)&fre_clust, &fs1);
    if(res==0)
	{											   
	    tot_sect=(fs1->n_fatent-2)*fs1->csize;	//�õ���������
	    fre_sect=fre_clust*fs1->csize;			//�õ�����������	   
  
		*total=tot_sect>>1;	//��λΪKB
		*free=fre_sect>>1;	//��λΪKB 
 	}
	return res;
}





