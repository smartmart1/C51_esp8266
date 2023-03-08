#ifndef _UART_H_
#define _UART_H_
extern LinkPack_F6 LkPackRevFromPC;//接收来自PC数据包 
extern  LinkAppProcessFlag LkPackProcessFlgFromPC;
extern unsigned char LkSendChar(unsigned char option,unsigned char bdata);
#endif
