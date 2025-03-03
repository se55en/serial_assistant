
#include "Serial.h"

char print_buf[512];
Serial_TypeDef serial;
void serial_ChangeConfig(u8 flag);

void Serial_Init(void)
{
	//数据初始化
//	serial.toPCBaud = 115200;
//	serial.PCDataBit = 0;
//	serial.PCParityBit = 0;
//	serial.PCStopBit = 1;
//	serial.Baud = 115200;
//	serial.DataBit = 0;
//	serial.ParityBit = 0;
//	serial.StopBit = 1;
//	serial.ShowMode = 0;
//	serial.ProcessMode = 0;
	serial.USART1_RecvBuffLen = 0;
	serial.USART1_DisplayLen = 0;
	serial.USART1_NoDisplayLen = 0;
	memset(serial.USART1_RecvBuff, '\0', Recv_Max_Len); // 清空接收缓冲区
	
	serial.USART3_RecvBuffLen = 0;
	serial.USART3_DisplayLen = 0;
	serial.USART3_NoDisplayLen = 0;
	memset(serial.USART3_RecvBuff, '\0', Recv_Max_Len); // 清空接收缓冲区
	
	serial.USART2_TxFlag = 0;
	serial.USART2_TxBuffLen = 0;
	memset(serial.USART2_TxBuff, '\0', Recv_Max_Len); // 清空发送缓冲区
	//开启usart1时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	//开启usart23时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	//开启gpioA和B的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
/***********************************************************************/	
	
	//配置PA2-usart2-tx
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//配置PA3-usart2-rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//配置PA10-usart1-rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	//配置PB11-usart3-rx
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
/***********************************************************************/		
	
//	//配置USART2
//	USART_InitTypeDef USART_InitStructure;
//	USART_InitStructure.USART_BaudRate = serial.toPCBaud;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Tx;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_Init(USART2, &USART_InitStructure);
//	//配置USART1
//	USART_InitStructure.USART_BaudRate = serial.Baud;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_Init(USART1, &USART_InitStructure);
//	//配置USART3
//	USART_InitStructure.USART_BaudRate = serial.Baud;
//	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
//	USART_InitStructure.USART_Mode = USART_Mode_Rx;
//	USART_InitStructure.USART_Parity = USART_Parity_No;
//	USART_InitStructure.USART_StopBits = USART_StopBits_1;
//	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
//	USART_Init(USART3, &USART_InitStructure);
	serial_ChangeConfig(0);
	serial_ChangeConfig(1);
	
////DMA配置	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);  // 使能 DMA1 时钟
    
	DMA_InitTypeDef DMA_InitStruct;
    // usart1_rx DMA1通道5
    DMA_InitStruct.DMA_BufferSize = Recv_Max_Len;      			// 定义了接收的最大长度
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;             // 串口接收，方向是外设->内存
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;                   // 本次是外设到内存，所以关闭内存到内存
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)serial.USART1_RecvBuff;// 内存的基地址，要存储在哪里
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;// 内存数据宽度，按照字节存储
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;        // 内存递增，每次串口收到数据存在内存中，下次收到自动存储在内存的下一个位置
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;                  // 正常模式
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR; 	// 串口的数据寄存器
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;    // 外设的数据宽度，按照字节存储，与内存的数据宽度一致
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;   // 接收只有一个数据寄存器 RDR，所以外设地址不递增
    DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;            // 优先级
    DMA_Init(DMA1_Channel5, &DMA_InitStruct);
	//usart3_rx DMA1通道3
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)serial.USART3_RecvBuff;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART3->DR;
	DMA_Init(DMA1_Channel3, &DMA_InitStruct);
	//串口2发送 DMA1通道7
	DMA_InitStruct.DMA_BufferSize = 0;//初始化为0，不发送
	DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralDST;             // 发送是方向是内存到外设，外设作为目的地
	DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)serial.USART2_TxBuff;
	DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)&USART2->DR;
	DMA_Init(DMA1_Channel7, &DMA_InitStruct);
     
    USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);// 使能USART1-DMA串口接受请求
	USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);// 使能USART1-DMA串口接受请求
	USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);
	
    DMA_Cmd(DMA1_Channel5, ENABLE);     // 使能接收
	DMA_Cmd(DMA1_Channel3, ENABLE);     // 使能接收
	DMA_Cmd(DMA1_Channel7, DISABLE);    // 禁止发送
	

//配置中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
	USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);//串口空闲中断
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitTypeDef NVIC_InitStructure;
	//串口1中断
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	//串口2中断
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	//串口3中断
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
	
	//开启串口
	USART_Cmd(USART1, ENABLE);
	USART_Cmd(USART2, ENABLE);
	USART_Cmd(USART3, ENABLE);
}

void Serial_USART2_SendByte(uint8_t Byte)
{
	USART_SendData(USART2, Byte);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
}

/* 重置DMA的传输数量寄存器 */
void uart1DmaClear(void)
{
    DMA_Cmd(DMA1_Channel5, DISABLE);    // 关闭 DMA1_Channel5 通道
    DMA_SetCurrDataCounter(DMA1_Channel5, Recv_Max_Len);   // 重新写入要传输的数据数量
    DMA_Cmd(DMA1_Channel5, ENABLE);     // 使能 DMA1_Channel5 通道
}
void uart3DmaClear(void)
{
    DMA_Cmd(DMA1_Channel3, DISABLE);    // 关闭 DMA1_Channel3 通道
    DMA_SetCurrDataCounter(DMA1_Channel3, Recv_Max_Len);   // 重新写入要传输的数据数量
    DMA_Cmd(DMA1_Channel3, ENABLE);     // 使能 DMA1_Channel3 通道
}
void serial_Change13Baud(u32 Baud)//修改串口13波特率
{
	USART_InitTypeDef USART_InitStructure;
	//配置USART1
	USART_InitStructure.USART_BaudRate = Baud;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	//配置USART3
	USART_InitStructure.USART_BaudRate = Baud;
	USART_Init(USART3, &USART_InitStructure);
}
void serial_ChangeConfig(u8 flag)//修改哪个串口的配置，13或2
{	
	struct usart_config{
		u8 databit;
		u8 stopbit;
		u8 paritybit;
		u32 baud;
	}config;
	if(flag)
	{
		config.databit = serial.PCDataBit;
		config.stopbit = serial.PCStopBit;
		config.paritybit = serial.PCParityBit;
		config.baud = serial.toPCBaud;
	}
	else
	{
		config.databit = serial.DataBit;
		config.stopbit = serial.StopBit;
		config.paritybit = serial.ParityBit;
		config.baud = serial.Baud;
	}
	USART_InitTypeDef USART_InitStructure;
	//配置USART1
	USART_InitStructure.USART_BaudRate = config.baud;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	switch(flag)
	{
		case 0:USART_InitStructure.USART_Mode = USART_Mode_Rx;break;//串口13，rx
		case 1:USART_InitStructure.USART_Mode = USART_Mode_Tx;break;//串口2 ，tx
	}
	switch(config.paritybit)
	{
		case 0:USART_InitStructure.USART_Parity = USART_Parity_No;break;
		case 1:USART_InitStructure.USART_Parity = USART_Parity_Even;break;
		case 2:USART_InitStructure.USART_Parity = USART_Parity_Odd;break;
	}
	switch(config.databit)
	{
		case 0:USART_InitStructure.USART_WordLength = USART_WordLength_8b;break;
		case 1:USART_InitStructure.USART_WordLength = USART_WordLength_9b;break;
	}
	switch(config.stopbit)
	{
		case 0:USART_InitStructure.USART_StopBits = USART_StopBits_0_5;break;
		case 1:USART_InitStructure.USART_StopBits = USART_StopBits_1;break;
		case 2:USART_InitStructure.USART_StopBits = USART_StopBits_1_5;break;
		case 3:USART_InitStructure.USART_StopBits = USART_StopBits_2;break;
	}
	switch(flag)
	{
		case 0:
			USART_Init(USART1, &USART_InitStructure);
			USART_Init(USART3, &USART_InitStructure);
			break;//串口13
		case 1:			
			USART_Init(USART2, &USART_InitStructure);
			break;//串口2 
	}
}
void serial_Change2Baud(u32 Baud)//修改串口2波特率
{
	USART_InitTypeDef USART_InitStructure;
	//配置USART1
	USART_InitStructure.USART_BaudRate = Baud;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART2, &USART_InitStructure);
}
/* 串口1发送数组 */
void uart2SendArray(uint8_t *arr, uint16_t len)
{
    if(len == 0)	// 判断长度是否有效
      return;
	
	uint16_t sendLen = (len>sizeof(serial.USART2_TxBuff)) ? sizeof(serial.USART2_TxBuff) : len;	// 防止越界

    
    while (DMA_GetCurrDataCounter(DMA1_Channel7));  // 检查DMA发送通道内是否还有数据//可以写个发送超时函数
    if(arr) 
    memcpy(serial.USART2_TxBuff, arr, sendLen);
    
    // DMA发送数据-要先关 设置发送长度 开启DMA
    DMA_Cmd(DMA1_Channel7, DISABLE);
    DMA_SetCurrDataCounter(DMA1_Channel7, sendLen);   // 重新写入要传输的数据数量
    DMA_Cmd(DMA1_Channel7, ENABLE);     // 启动DMA发送  
}
