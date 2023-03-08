#ifndef __F5_L3_H__
#define __F5_L3_H__

#include "stm32f10x.h"



void UART1_Conf(int baud);
extern void GPIO_CONFIG(void);
void ICRW_DETECT(void);
void NVIC_Conf(void);
unsigned char ReadDspOneSensor(u8 Channel);
void sysconfig(void);
void Delay_ms(unsigned long ms);
void Delay_us(unsigned long us);
void UART1_Conf(int baud);
void UART3_Conf(int baud);
void SetMechValue(void);


#endif


