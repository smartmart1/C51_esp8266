#include "centre.h"

#define AT_CWMODE  		"AT+CWMODE=2\r\n"
#define AT_CIPMUX  		"AT+CIPMUX=1\r\n"
#define AT_CIPSERVER 	"AT+CIPSERVER=1,8080\r\n"
#define AT_CIPSEND    "AT+CIPSEND=0,6\r\n"

uint8_t Datapack[6];
unsigned char GetBCC(unsigned char *p,unsigned short n);

unsigned char GetBCC(unsigned char *p,unsigned short n){
    unsigned int loop;
    unsigned char bBCC = 0;
    for (loop=0;loop<n;loop++)
        bBCC ^=*p++;
    return bBCC;
}

unsigned char USART_SendChar(unsigned char option,unsigned char bdata){
    USART_SendData(USART1,bdata);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) ;
    USART_ClearFlag(USART1,USART_FLAG_TXE);
	  return 0;
}

unsigned char USART3_SendChar(unsigned char option,unsigned char bdata){
    USART_SendData(USART3,bdata);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET) ;
    USART_ClearFlag(USART3,USART_FLAG_TXE);
	  return 0;
}

unsigned char USART_TX_DATA(unsigned char *bdata,unsigned char length)
{   
    unsigned char i = 0;	
		for(i = 0 ;i < length ; i++)
		{
       while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);	
       USART_SendData(USART2,*bdata++);			
		}
	  return 0;
}

void Esp8266_Init()
{
	 USART_TX_DATA((unsigned char *)AT_CWMODE,strlen(AT_CWMODE));  
	 Delay_ms(10);
	 USART_TX_DATA((unsigned char *)AT_CIPMUX,strlen(AT_CIPMUX));  
	 Delay_ms(10);
	 USART_TX_DATA((unsigned char *)AT_CIPSERVER,strlen(AT_CIPSERVER)); 
   Delay_ms(10);	
}

void send_pack(uint8_t data)
{
	 uint8_t bcc=0;
	 
	 Datapack[0] = 0x02;
   Datapack[1] = 0x01;
	 Datapack[2] = 0x01;
   Datapack[3] = data;
	 Datapack[4] = 0x03;
	 bcc = GetBCC(Datapack,5); 
   Datapack[5] = bcc;
	 USART_TX_DATA((unsigned char *)AT_CIPSEND,strlen(AT_CIPSEND)); 
   Delay_ms(10);	
   USART_TX_DATA(Datapack,6);   
	 Delay_ms(10);	
}

uint8_t tx_packet[32];
uint8_t ControlWord = 0;

void Uart2_IRQ (void){
    volatile unsigned char bSbuf;
	  static uint16_t  i = 0;
	  uint8_t bcccheck = 0;
    //接收终端
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
        tx_packet[i++] = USART_ReceiveData(USART2) ;//接收处理   
			   
			   switch(i)
				 {
						 case 1:
									if(tx_packet[i-1] != 0x02)
										i=0;
									break;
						 case 2:
									if(tx_packet[i-1] != 0x01)
										i=0;
									break; 
						 case 3:
									if(tx_packet[i-1] != 0x01)
										i=0;
									break;
						 case 5:
							    if(tx_packet[i-1] != 0x03)  //包尾
										i=0;
									break;
						 case 6:
							    bcccheck = GetBCC(tx_packet,5);  
							    if(tx_packet[i-1] != bcccheck)
									{
										i=0;
										break;
									}else
									{
										ControlWord = tx_packet[3];
										i = 0;
									}
									break;
				 }	 
    }
}

void Uart1_IRQ (void){
    volatile unsigned char bSbuf;
    //接收终端
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
        bSbuf = USART_ReceiveData(USART1) ;//接收处理  
    }
}


void Uart3_IRQ (void){
    volatile unsigned char bSbuf;
    //接收终端
    if (USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        USART_ClearITPendingBit(USART3, USART_IT_RXNE);
        bSbuf = USART_ReceiveData(USART3) ;//接收处理
    }
}








































