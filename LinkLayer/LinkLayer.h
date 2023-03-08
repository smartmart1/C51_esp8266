#ifndef __LINKLAYER_H__
#define __LINKLAYER_H__

#define uint8_t unsigned char
#define int16_t signed short
#define uint16_t unsigned short

#define   LkStu_IDLE                  0X00    //命令空闲
#define   LkStu_LinkHead              0X01    //命令空闲
#define   LkStu_LinkPackEnd           0X02    //命令空闲
#define   LkStu_COMMDOK               0x03    //接收命令OK
#define   LkStu_COMMDResp             0x04    //接收命令OK
#define   LkStu_COMMDWaitRespACK      0x05    //接收命令OK


#define USART_Getchar()  USART_ReceiveData(USART1);  //获取一个字节数据
#define USART_Setchar(bdata)  {  USART_SendData(USART1,bdata);\
                                 while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) ;\
                                 USART_ClearFlag(USART1,USART_FLAG_TXE);}

/*应答数据包*/
typedef struct {
    uint8_t bInd ;// 'C' 发送命令标示 'P' 接受肯定命令标示  'N'  接收错误命令标示
    uint8_t bCm ;
    uint8_t bPm;
    uint8_t bBuffer[64];
}APPPack_F6Resp;

/*请求数据包*/
typedef struct {
    uint8_t bCm ;
    uint8_t bPm;
    uint8_t bBuffer[64];
}APPPack_F6Req;


typedef struct {
    uint8_t bStx;//0x00 0x00 0xff
    uint8_t bLen[2];
    union {
        APPPack_F6Req   APPPackReq;//请求数据包
        APPPack_F6Resp  APPPackResq;//应答数据包
    }APPPack_F6;
    uint16_t WCRC16;//校验
}LinkPack_F6;

typedef struct {
    uint8_t b56;
    uint8_t b3d;
    uint8_t bBuffer[4];
}LinkPack_Batty;



typedef struct {
    uint8_t Headflag ; //数据链路层（"HST"）标志，0表示无链路层
    uint8_t *RevP;     //当前接受字节保存的缓冲区指针
    uint16_t  RevCnt;    //接收有效的字节个数

    uint8_t  PackageType;//数据类型




    uint8_t CommandSQ; //0---表示还没有接受到数据包
    //1---表示接受到完整的数据包文件，但是还没有处理,但是没有放松应道
    //2---应答放送完毕，准备处理数据表
    //3---数据表执行完毕，放松并等待上位机接收的应道

    uint8_t TrsrReSend;
    uint16_t  TrsCnt;    //等待发送的数据长度
    uint8_t *TrsP;     //要发送的数据指针
    uint8_t TrsStatus; //0----表示数据发送完毕，1表示数据还没有发送完毕


    uint8_t   bInterface;//数据接收，0--来自COM0口，0x10--来自USB口
    uint8_t  bTimeNum;//使用的定时器编号，用于字符和帧的保护
    uint16_t  dTimebw10ms;//字符等待时间
    uint16_t  dTimefw10ms;//帧等待时间

}LinkAppProcessFlag;






extern void  Comm_DataParamInit(LinkPack_F6 *psLkPackRev,//接收数据包
                        LinkPack_F6 *psLkPackTrs,//发送数据包
                        LinkAppProcessFlag *psLkPackProcessFlg,
                        uint8_t  bTimeNum,
                        uint16_t dTimebw10ms,
                        uint16_t  dTimefw10ms
                       );

extern void Comm_AppLayerTask(uint16_t (*CommAppFunCallBack)(LinkPack_F6 *psLkPackRev,
                                                      LinkPack_F6 *psLkPackTrs,
                                                      LinkAppProcessFlag *psLkPackProcessFlg),
                       LinkPack_F6 *psLkPackRev,
                       LinkPack_F6 *psLkPackTrs,
                       LinkAppProcessFlag *psLkPackProcessFlg,
                       uint8_t (*SendCharCallBack)(uint8_t boption,//0--立即发送，1--暂存到缓冲区
                                                   uint8_t bdata));

#endif





