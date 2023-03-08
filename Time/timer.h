#ifndef __TIMER_H__
#define __TIMER_H__

#define TIME_GreenLed   0

extern void SoftTimeStart(unsigned char num,unsigned int count);
extern void SoftTimeStop(unsigned char num);
extern unsigned char SoftTimeGetStatusOV(unsigned char num);
extern unsigned char SoftTimeGetStatusEN(unsigned char num);
extern void SoftTimeISR(void);
extern void SoftCntISR(void);
extern void Time2_IRQ(void);
#endif  
