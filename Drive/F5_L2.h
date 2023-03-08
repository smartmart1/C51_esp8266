
#ifndef __F5_L2_H__
#define __F5_L2_H__

#include "LinkLayer.h"
#include "uart.h"


extern void ProcessCmd(void);
extern unsigned short  CommandTaskPC(LinkPack_F6 *psLkPackRev,
                                     LinkPack_F6 *psLkPackTrs,
                                     LinkAppProcessFlag *psLkPackProcessFlg);


extern void ProcessCmd(void);

void ADC_Configuration(void);





#endif

