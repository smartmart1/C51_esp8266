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


#define LEDGreen_GetStatus()   GPIO_ReadOutputDataBit(GPIOC,GPIO_Pin_13)  //��������ָʾ��
#define LEDGreen_OFF()         GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define LEDGreen_ON()          GPIO_ResetBits(GPIOC,GPIO_Pin_13)

#define SPRINKLER_ON()     GPIO_ResetBits(GPIOA,GPIO_Pin_7)
#define SPRINKLER_OFF()    GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define  N   10	     //ÿͨ����10��
#define  M   1	     //Ϊ1��ͨ��

extern vu16  AD_Value[N][M];   //�������ADCת�������Ҳ��DMA��Ŀ���ַ
extern vu16  After_filter[M];    //���������ƽ��ֵ֮��Ľ��


extern unsigned long Dms;
extern  unsigned long Dus;



extern void Uart1_IRQ (void);
extern void Uart3_IRQ (void);
extern void SensorColServ(void);
extern unsigned char USART_SendChar(unsigned char option,unsigned char bdata);

void Esp8266_Init(void);
void send_pack(uint8_t data);



#endif
