#include "centre.h"

unsigned long Dms;
unsigned long Dus;

void USART1_IRQHandler(void)
{
   Uart1_IRQ ();
}


void USART2_IRQHandler(void)
{
    if (USART_GetFlagStatus(USART2,USART_IT_RXNE)==SET) {
        USART_ClearITPendingBit(USART2, USART_IT_RXNE);
    }

}


void USART3_IRQHandler(void)
{
    Uart3_IRQ ();

}


void SysTick_Handler(void)
{
    static unsigned char bCnt = 0;

    if (Dms )Dms--;

    if (bCnt++>=10) {
        bCnt = 0;
        SoftTimeISR();
    }


}


