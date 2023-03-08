#ifndef __CENTRE_H__
#define __CENTRE_H__

#include "stm32f10x.h"
#include "F5_L2.h"
#include "F5_L3.h"
#include "timer.h"
#include "uart.h"
#include "LinkLayer.h"
#include "string.h"
#include "stm32f10x_it.h"


#define LEDGreen_GetStatus()   GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13)  //程序运行指示灯
#define LEDGreen_OFF()         GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define LEDGreen_ON()          GPIO_ResetBits(GPIOC,GPIO_Pin_13)

#define SPRINKLER_ON()     GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define SPRINKLER_OFF()    GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define  N   10	     //每通道采10次
#define  M   1	     //为1个通道

extern vu16  AD_Value[N][M];   //用来存放ADC转换结果，也是DMA的目标地址
extern vu16  After_filter[M];    //用来存放求平均值之后的结果


extern unsigned long Dms;
extern  unsigned long Dus;



extern void Uart1_IRQ (void);
extern void Uart3_IRQ (void);
extern void SensorColServ(void);
extern unsigned char USART_SendChar(unsigned char option,unsigned char bdata);

void Esp8266_Init(void);
void send_pack(uint8_t data);



#endif
