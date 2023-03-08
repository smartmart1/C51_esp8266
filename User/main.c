#include "centre.h"

extern uint8_t ControlWord;

int main(void)
{
	uint8_t Flag = 0;
	
    sysconfig();
    NVIC_Conf();
    ICRW_DETECT();
	  Esp8266_Init();
    SoftTimeStart(TIME_GreenLed,50);
	
    while (1) {
        if (SoftTimeGetStatusOV(TIME_GreenLed)) 
				{
            SoftTimeStart(TIME_GreenLed,50);
            if(Flag == 0){
								LEDGreen_ON(); 	
								Flag = 1;
						}else{
								LEDGreen_OFF(); 				
								Flag = 0;
						}
						send_pack(After_filter[0]);
						if(ControlWord == 0x01) //自动控制
						{
							  if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5) == SET) SPRINKLER_OFF();
								else  SPRINKLER_ON(); 
						}
						else if(ControlWord == 0x02) //手动控制  花洒开
						{
								SPRINKLER_ON(); 
						}
						else  //不操作  //花洒关
						{
							 SPRINKLER_OFF();
						}
        }
    }
}



