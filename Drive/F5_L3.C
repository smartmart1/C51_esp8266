#include "centre.h"

vu16  AD_Value[N][M];   //用来存放ADC转换结果，也是DMA的目标地址
vu16  After_filter[M];    //用来存放求平均值之后的结果

void RCC_Configuration(void) 
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOD | RCC_APB2Periph_GPIOE, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  | RCC_APB2Periph_AFIO, ENABLE); 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3, ENABLE);  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1   | RCC_APB2Periph_USART1, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3   | RCC_APB1Periph_TIM4 | RCC_APB2Periph_ADC1, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);                
	RCC_ADCCLKConfig(RCC_PCLK2_Div8);                                 
}



void RCC_Conf() {

    ErrorStatus HSE_Status;
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON); //开启HSE
    HSE_Status = RCC_WaitForHSEStartUp();
    if ( HSE_Status == SUCCESS ) {
        RCC_HCLKConfig(RCC_SYSCLK_Div1);   //AHB	1分频
        RCC_PCLK2Config(RCC_HCLK_Div1);     //APB2	1分频
        RCC_PCLK1Config(RCC_HCLK_Div2);    //APB1	2分频
        FLASH_SetLatency(FLASH_Latency_2);  //sysclk在48M以上时必须选择2个等待周期
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9); //8*9=72
        RCC_PLLCmd(ENABLE);
        while ( RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET ); //PLL复位时一直等待
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); //选择PLL输出作为系统时钟源，72M
        while ( RCC_GetSYSCLKSource() != 0x08 ); //等待PLL成为系统时钟源后继续
    }
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_BKP | RCC_APB1Periph_PWR, ENABLE);  //打开BPK时钟表和PWR时钟
    PWR_BackupAccessCmd(ENABLE); //允许写备份域

//#ifdef IAPBOOT
 //   NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x4000);
//#else
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);

//#endif
		if(SysTick_Config(SystemCoreClock /1000)){
		    while(1);
		}
}

void ADC_DMA_Config(void) {
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_DeInit(DMA1_Channel1);

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Value;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = N * M;;

    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //外设地址不变

    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //内存地址递增

    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;

    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;

    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;

    DMA_InitStructure.DMA_Priority = DMA_Priority_High;

    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(DMA1_Channel1, &DMA_InitStructure);

    DMA_Cmd(DMA1_Channel1, ENABLE); //启动DMA通道

}


void ADC_Configuration(void)  
{

    ADC_InitTypeDef ADC_InitStructure;

    ADC_DMA_Config();


    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC1和ADC2工作在独立模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE; //多通道
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; //连续转换
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; //软件启动转换
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //转换结果右对齐
    ADC_InitStructure.ADC_NbrOfChannel = M; //通道数目
    ADC_Init(ADC1, &ADC_InitStructure);

    // ADC_TempSensorVrefintCmd(ENABLE);//使能片内温度传感器



    RCC_ADCCLKConfig(RCC_PCLK2_Div6); //PCLK 6分频
    // ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_55Cycles5);//通道，转换次序，转换时间
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 1, ADC_SampleTime_55Cycles5);

//    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 3, ADC_SampleTime_55Cycles5); 
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 4, ADC_SampleTime_55Cycles5); 
//    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 5, ADC_SampleTime_55Cycles5); 


    ADC_DMACmd(ADC1, ENABLE);


    ADC_Cmd(ADC1, ENABLE);

    ADC_ResetCalibration(ADC1);

    while ( ADC_GetResetCalibrationStatus(ADC1) );

    ADC_StartCalibration(ADC1); //开始校准

    while ( ADC_GetCalibrationStatus(ADC1) );

    ADC_SoftwareStartConvCmd(ADC1, ENABLE); //使能ADC的软件转换启动功能

}


void UART1_Conf(int baud) 
{
    GPIO_InitTypeDef UART_TX_InitStucture;
    GPIO_InitTypeDef UART_RX_InitStucture;
    USART_InitTypeDef UART_InitStructure;
    //TX的配置
    UART_TX_InitStucture.GPIO_Pin = GPIO_Pin_9;
    UART_TX_InitStucture.GPIO_Speed = GPIO_Speed_50MHz;
    UART_TX_InitStucture.GPIO_Mode = GPIO_Mode_AF_PP; //推挽输出
    GPIO_Init(GPIOA, &UART_TX_InitStucture);
    //RX的配置
    UART_RX_InitStucture.GPIO_Pin = GPIO_Pin_10;
    UART_RX_InitStucture.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOA, &UART_RX_InitStucture);

    //设置uart
    UART_InitStructure.USART_BaudRate = baud;
    UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    UART_InitStructure.USART_StopBits = USART_StopBits_1;
    UART_InitStructure.USART_Parity = USART_Parity_No;
    UART_InitStructure.USART_WordLength = USART_WordLength_8b;
#ifndef LIAN_DI
#else
    //UART_InitStructure.USART_Parity=USART_Parity_No;
//	UART_InitStructure.USART_WordLength=USART_WordLength_8b;
#endif
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

    USART_Init(USART1, &UART_InitStructure);   //初始化
    USART_Cmd(USART1, ENABLE);                //使能

}

void UART2_Conf(int baud) {
    GPIO_InitTypeDef UART_TX_InitStucture;
    GPIO_InitTypeDef UART_RX_InitStucture;
    USART_InitTypeDef UART_InitStructure;
   
    //TX的配置
    UART_TX_InitStucture.GPIO_Pin = GPIO_Pin_2;
    UART_TX_InitStucture.GPIO_Speed = GPIO_Speed_50MHz;
    UART_TX_InitStucture.GPIO_Mode = GPIO_Mode_AF_PP; //推挽输出
    GPIO_Init(GPIOA, &UART_TX_InitStucture);
    //RX的配置
    UART_RX_InitStucture.GPIO_Pin = GPIO_Pin_3;
    UART_RX_InitStucture.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOA, &UART_RX_InitStucture);

    //设置uart
    UART_InitStructure.USART_BaudRate = baud;
    UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    UART_InitStructure.USART_Parity = USART_Parity_No;

    UART_InitStructure.USART_StopBits = USART_StopBits_2;
    UART_InitStructure.USART_WordLength = USART_WordLength_8b;

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Init(USART2, &UART_InitStructure);   //初始化
    USART_Cmd(USART2, ENABLE);                //使能
}

void UART3_Conf(int baud) {
    GPIO_InitTypeDef UART_TX_InitStucture;
    GPIO_InitTypeDef UART_RX_InitStucture;
    USART_InitTypeDef UART_InitStructure;
    
    //TX的配置
    UART_TX_InitStucture.GPIO_Pin = GPIO_Pin_10;
    UART_TX_InitStucture.GPIO_Speed = GPIO_Speed_50MHz;
    UART_TX_InitStucture.GPIO_Mode = GPIO_Mode_AF_PP; //推挽输出
    GPIO_Init(GPIOB, &UART_TX_InitStucture);
    //RX的配置
    UART_RX_InitStucture.GPIO_Pin = GPIO_Pin_11;
    UART_RX_InitStucture.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
    GPIO_Init(GPIOB, &UART_RX_InitStucture);

    //设置uart
    UART_InitStructure.USART_BaudRate = baud;
    UART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    UART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    UART_InitStructure.USART_Parity = USART_Parity_No;


    UART_InitStructure.USART_StopBits = USART_StopBits_1;
    UART_InitStructure.USART_WordLength = USART_WordLength_8b;

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    USART_Init(USART3, &UART_InitStructure);   //初始化
    USART_Cmd(USART3, ENABLE);                //使能
}

void NVIC_Conf(void) 
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    /* 开启 TIM2 中断, 0级先占优先级，0级后占优先级 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* 开启 TIM3 中断, 0级先占优先级，0级后占优先级 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    //串口1中断最高优先级   0/0
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    //串口2中断优先级   0/3
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    //串口3中断优先级   0/2
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}


void GPIO_CONFIG() 
{
    //端口
    GPIO_InitTypeDef IO_InitStucture;

    //LED灯
    IO_InitStucture.GPIO_Pin = GPIO_Pin_13;
    IO_InitStucture.GPIO_Speed = GPIO_Speed_50MHz;
    IO_InitStucture.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &IO_InitStucture);  
    
    //MQ-2 D0
	  IO_InitStucture.GPIO_Pin = GPIO_Pin_5;
    IO_InitStucture.GPIO_Speed = GPIO_Speed_50MHz;
    IO_InitStucture.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &IO_InitStucture);

    //MQ-2 AO
    IO_InitStucture.GPIO_Pin = GPIO_Pin_6;
    IO_InitStucture.GPIO_Speed = GPIO_Speed_50MHz;
    IO_InitStucture.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &IO_InitStucture);
	
	  //继电器
    IO_InitStucture.GPIO_Pin = GPIO_Pin_7;
    IO_InitStucture.GPIO_Speed = GPIO_Speed_50MHz;
    IO_InitStucture.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &IO_InitStucture);
		
		SPRINKLER_OFF();
}

void sysconfig(void) 
{
    RCC_Conf();
	  RCC_Configuration();
    GPIO_CONFIG();
   
    ADC_Configuration();
    ADC_SoftwareStartConvCmd(ADC1, ENABLE); 
    DMA_Cmd(DMA1_Channel1, ENABLE);    
}

void ICRW_DETECT(void) 
{
    UART1_Conf( 9600 );
    UART2_Conf( 9600 );
    UART3_Conf( 9600 );	  
}

/*获取ADC的值，将二进制换算为十进制*/
u16 ADCGetVolt(unsigned int advalue)   {
    return(u16)(advalue * 3300 / 4096);   //求的结果扩大了1000倍，方便下面求出小数,这样单位就是mv

}
/*求平均值函数*/
void ADCfilter(void) {
    int  sum = 0;
    unsigned char i;
    u8  count;
    for ( i = 0; i < M; i++ ) {
        for ( count = 0; count < N; count++ ) 
					sum += AD_Value[count][i];
        After_filter[i] = sum / N;
			  After_filter[i] = After_filter[i] % 101;
        sum = 0;
    }
}

/*获取电源电压，单位MV*/
unsigned short GetPowermv() {
//    ADCfilter();
//    return ((ADCGetVolt(After_filter[3]) * 12000) / 1565);  //计算电流  12V电压的时候，检测端为1.43V
}
