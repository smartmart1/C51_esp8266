#include "centre.h"



void Delay_ms(unsigned long ms)
{
    Dms=ms;
    while (Dms!=0);
}

void Delay_us(unsigned long us)
{
    unsigned char i;
    while (us--) {
        for (i=0; i<10; i++);
    }
}
