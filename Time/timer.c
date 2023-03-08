#include "timer.h"

#define SOFTTIMECNT  15     //定义软件定时器的个数
#define SOFTCNTCNT   1    //定义软件计数器的个数
unsigned char SoftTimerEn[SOFTTIMECNT] ;
unsigned char SoftTimerOv[SOFTTIMECNT];
unsigned short SoftTimer[SOFTTIMECNT];

unsigned int SoftCnt[SOFTCNTCNT];
unsigned char SoftCntEn[SOFTCNTCNT];
unsigned char SoftCntOv[SOFTCNTCNT];

/**************************************************************
* 说明：启动一个计数器
* 
***************************************************************/
void SoftCntStart(unsigned char num){
    if (SoftCntEn[num]==0x01 ) {
        return;
    }
    SoftCntEn[num] = 1;
    SoftCnt[num] = 0;
    SoftCntOv[num] = 0x00;
}

void SoftCntStop(unsigned char num){
    SoftCntEn[num] = 0;
    SoftCnt[num] = 0;
    SoftCntOv[num] = 0x00;
}
/**************************************************************
*说明：获得当前计数器的数值 
* 
***************************************************************/
unsigned int  SoftCntGetCnt(unsigned char num){
    return SoftCnt[num] ;
}

unsigned int  SoftCntGetEn(unsigned char num){
    return SoftCntEn[num] ;
}

/**************************************************************
* 函数：启动定时器
* 输入参数：num--定时器编号
*           count--定时数值，10ms为一个单位
***************************************************************/

void SoftTimeStart(unsigned char num,unsigned int count){
    SoftTimer[num]=count;
    SoftTimerEn[num] = 1;
    SoftTimerOv[num] = 0;
}
void SoftTimeStop(unsigned char num){
    SoftTimer[num]=0;
    SoftTimerEn[num] = 0;
    SoftTimerOv[num] = 0;
}

unsigned char SoftTimeGetStatusOV(unsigned char num){
    return SoftTimerOv[num];

}
unsigned char SoftTimeGetStatusEN(unsigned char num){
    return SoftTimerEn[num];
}
void SoftTimeInitAll(){
    unsigned char i;
    for (i=0;i<SOFTTIMECNT;i++) {
        SoftTimeStop(i);
    }
}

void SoftTimeISR(){
    unsigned char loop;
    for (loop = 0 ;loop < SOFTTIMECNT ;loop++ ) {
        if ((SoftTimerEn[loop] == 1)&&(SoftTimerOv[loop])==0) {
            SoftTimer[loop]-- ;
            if (SoftTimer[loop]== 0)
                SoftTimerOv[loop] = 1 ;
        }
    } 
}
void SoftCntISR(){
     unsigned char loop;
    for (loop = 0; loop<SOFTCNTCNT; loop++ ) {
        if ((SoftCntEn[loop]==0x01)&&(SoftCntOv[loop]==0x00)) {
            if (SoftCnt[loop]==0xffff) SoftCntOv[loop]=0x01;
            else SoftCnt[loop]++;
        }
    } 
}





