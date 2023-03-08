#ifndef __LINKLAYER_H__
#define __LINKLAYER_H__

#define uint8_t unsigned char
#define int16_t signed short
#define uint16_t unsigned short

#define   LkStu_IDLE                  0X00    //�������
#define   LkStu_LinkHead              0X01    //�������
#define   LkStu_LinkPackEnd           0X02    //�������
#define   LkStu_COMMDOK               0x03    //��������OK
#define   LkStu_COMMDResp             0x04    //��������OK
#define   LkStu_COMMDWaitRespACK      0x05    //��������OK


#define USART_Getchar()  USART_ReceiveData(USART1);  //��ȡһ���ֽ�����
#define USART_Setchar(bdata)  {  USART_SendData(USART1,bdata);\
                                 while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET) ;\
                                 USART_ClearFlag(USART1,USART_FLAG_TXE);}

/*Ӧ�����ݰ�*/
typedef struct {
    uint8_t bInd ;// 'C' ���������ʾ 'P' ���ܿ϶������ʾ  'N'  ���մ��������ʾ
    uint8_t bCm ;
    uint8_t bPm;
    uint8_t bBuffer[64];
}APPPack_F6Resp;

/*�������ݰ�*/
typedef struct {
    uint8_t bCm ;
    uint8_t bPm;
    uint8_t bBuffer[64];
}APPPack_F6Req;


typedef struct {
    uint8_t bStx;//0x00 0x00 0xff
    uint8_t bLen[2];
    union {
        APPPack_F6Req   APPPackReq;//�������ݰ�
        APPPack_F6Resp  APPPackResq;//Ӧ�����ݰ�
    }APPPack_F6;
    uint16_t WCRC16;//У��
}LinkPack_F6;

typedef struct {
    uint8_t b56;
    uint8_t b3d;
    uint8_t bBuffer[4];
}LinkPack_Batty;



typedef struct {
    uint8_t Headflag ; //������·�㣨"HST"����־��0��ʾ����·��
    uint8_t *RevP;     //��ǰ�����ֽڱ���Ļ�����ָ��
    uint16_t  RevCnt;    //������Ч���ֽڸ���

    uint8_t  PackageType;//��������




    uint8_t CommandSQ; //0---��ʾ��û�н��ܵ����ݰ�
    //1---��ʾ���ܵ����������ݰ��ļ������ǻ�û�д���,����û�з���Ӧ��
    //2---Ӧ�������ϣ�׼���������ݱ�
    //3---���ݱ�ִ����ϣ����ɲ��ȴ���λ�����յ�Ӧ��

    uint8_t TrsrReSend;
    uint16_t  TrsCnt;    //�ȴ����͵����ݳ���
    uint8_t *TrsP;     //Ҫ���͵�����ָ��
    uint8_t TrsStatus; //0----��ʾ���ݷ�����ϣ�1��ʾ���ݻ�û�з������


    uint8_t   bInterface;//���ݽ��գ�0--����COM0�ڣ�0x10--����USB��
    uint8_t  bTimeNum;//ʹ�õĶ�ʱ����ţ������ַ���֡�ı���
    uint16_t  dTimebw10ms;//�ַ��ȴ�ʱ��
    uint16_t  dTimefw10ms;//֡�ȴ�ʱ��

}LinkAppProcessFlag;






extern void  Comm_DataParamInit(LinkPack_F6 *psLkPackRev,//�������ݰ�
                        LinkPack_F6 *psLkPackTrs,//�������ݰ�
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
                       uint8_t (*SendCharCallBack)(uint8_t boption,//0--�������ͣ�1--�ݴ浽������
                                                   uint8_t bdata));

#endif





