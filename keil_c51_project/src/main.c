#include "config.h"
#include "string.h"

/************************************************************************************/
//	nAsrStatus ������main�������б�ʾ�������е�״̬������LD3320оƬ�ڲ���״̬�Ĵ���
//	LD_ASR_IDLE:		��ʾû������ASRʶ��
//	LD_ASR_RUNING��		��ʾLD3320������ASRʶ����
//	LD_ASR_FOUND_OK:		��ʾһ��ʶ�����̽�������һ��ʶ����
//	LD_ASR_FOUND_NONE:	��ʾһ��ʶ�����̽�����û��ʶ����
//	LD_ASR_ERROR:		��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬
/***********************************************************************************/

void mcuInit(); 
void ProcessInt0(); //ʶ������
void  delay(unsigned long uldata);
void 	userHandle(uint8_t dat);//�û�ִ�в�������
void Delay200ms();
void ledTest(void);//��Ƭ������ָʾ

sbit LED=P4^2;//�ź�ָʾ��
uint8_t idata nAsrStatus=0;	

extern volatile uint8_t  sRecog[VOCIE_MAX_NUMBER][VOCIE_LENGTH];
extern uint8_t   rxBuffer[20];
extern uint8_t   rxFlag;
/***********************************************************
* ��    �ƣ� void  main(void)
* ��    �ܣ� ������	�������
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
**********************************************************/ 
void  main(void)
{
    uint8_t idata nAsrRes;
    uint8_t i=0;
    ledTest();
    mcuInit();
    LD3320Reset();
    uartInit(); /*���ڳ�ʼ��*/
    nAsrStatus = LD_ASR_IDLE;		//	��ʼ״̬��û������ASR

    while(1)
    {
        switch(nAsrStatus)
        {
        case LD_ASR_RUNING:
            break;
        case LD_ASR_ERROR:
            nAsrStatus = LD_ASR_IDLE;
            break;
        case LD_ASR_IDLE:
        {
            nAsrStatus=LD_ASR_RUNING;
            if (runASR()==0)	/*	����һ��ASRʶ�����̣�ASR��ʼ����ASR��ӹؼ��������ASR����*/
            {
                nAsrStatus = LD_ASR_ERROR;
            }
            break;
        }
        case LD_ASR_FOUND_OK: /*	һ��ASRʶ�����̽�����ȥȡASRʶ����*/
        {
            nAsrRes = LD3320GetResult();		/*��ȡ���*/
            userHandle(nAsrRes);//�û�ִ�к���
            nAsrStatus = LD_ASR_IDLE;
            break;
        }
        case LD_ASR_FOUND_NONE:
        default:
        {
//            printCom("NONE\r\n");
            nAsrStatus = LD_ASR_IDLE;
            break;
        }
        }// switch

        if(rxFlag == 1)
        {
            uint8_t returnValue;
            uint8_t index = rxBuffer[0]-0x30;
            uint8_t command[VOCIE_LENGTH] = {0};
            rxFlag= 0 ;
            printCom(rxBuffer+1);
            memcpy(command, rxBuffer+1, 19);
            memcpy(sRecog[index], command , VOCIE_LENGTH);
            memset(rxBuffer, 0, 20);
            returnValue = runASR();
            while(!returnValue)
            {
                returnValue = LD3320ASRAddCommand();
            }
        }
    }// while

}
/***********************************************************
* ��    �ƣ� 	 LED�Ʋ���
* ��    �ܣ� ��Ƭ���Ƿ���ָʾ
* ��ڲ����� �� 
* ���ڲ�������
* ˵    ���� 					 
**********************************************************/
void ledTest(void)
{
    LED=~ LED;
    Delay200ms();
    LED=~ LED;
    Delay200ms();
    LED=~ LED;
    Delay200ms();
    LED=~ LED;
    Delay200ms();
    LED=~ LED;
    Delay200ms();
    LED=~ LED;
}
/***********************************************************
* ��    �ƣ� void mcuInit()
* ��    �ܣ� ��Ƭ����ʼ��
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
**********************************************************/ 
void mcuInit()
{
    P0 = 0xff;
    P1 = 0xff;
    P2 = 0xff;
    P3 = 0xff;
    P4 = 0xff;

    P1M0=0XFF;	//P1�˿�����Ϊ����������ܣ������IO�������������������̵���ģ�鹤��
    P1M1=0X00;

    LD_MODE = 0;		//	����MD�ܽ�Ϊ�ͣ�����ģʽ��д
    IE0=1;
    EX0=1;
    EA=1;
}
/***********************************************************
* ��    �ƣ�	��ʱ����
* ��    �ܣ�
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
**********************************************************/ 
void Delay200us()		//@22.1184MHz
{
    unsigned char i, j;
    _nop_();
    _nop_();
    i = 5;
    j = 73;
    do
    {
        while (--j);
    } while (--i);
}

void  delay(unsigned long uldata)
{
    unsigned int j  =  0;
    unsigned int g  =  0;
    while(uldata--)
        Delay200us();
}

void Delay200ms()		//@22.1184MHz
{
    unsigned char i, j, k;

    i = 17;
    j = 208;
    k = 27;
    do
    {
        do
        {
            while (--k);
        } while (--j);
    } while (--i);
}
/***********************************************************
* ��    �ƣ� �жϴ�����
* ��    �ܣ�
* ��ڲ�����  
* ���ڲ�����
* ˵    ���� 					 
* ���÷����� 
**********************************************************/ 
void ExtInt0Handler(void) interrupt 0  
{ 	
    ProcessInt0();				/*	LD3320 �ͳ��ж��źţ�����ASR�Ͳ���MP3���жϣ���Ҫ���жϴ������зֱ���*/
}
/***********************************************************
* ��    �ƣ��û�ִ�к��� 
* ��    �ܣ�ʶ��ɹ���ִ�ж������ڴ˽����޸� 
* ��ڲ����� �� 
* ���ڲ�������
* ˵    ���� 					 
**********************************************************/
void 	userHandle(uint8_t dat)
{

    switch(dat)		   /*�Խ��ִ����ز���,�ͻ��޸�*/
    {
    case INDEX_APOLLO:

        printCom("0\r\n"); /*text.....*/
        break;
    case INDEX_ROBOT:

        printCom("1\r\n"); /*text.....*/
        break;
    case INDEX_FORWARD:
        printCom("2\r\n"); /*text.....*/
        break;
    case INDEX_BACKWARD:
        printCom("3\r\n"); /*text.....*/
        break;
    case INDEX_LEFT:
        printCom("4\r\n"); /*text.....*/
        break;
    case INDEX_RIGHT:
        printCom("5\r\n"); /*text.....*/
        break;
    case INDEX_STOP:
        printCom("6\r\n"); /*text.....*/
        break;
    case INDEX_SPEED_UP:
        printCom("7\r\n"); /*text.....*/
        break;
    case INDEX_SPEED_DOWN:
        printCom("8\r\n"); /*text.....*/
        break;
    case INDEX_TURN_ON:
        printCom("9\r\n"); /*text.....*/
        break;
    case INDEX_TURN_OFF:
        printCom("10\r\n"); /*text.....*/
        break;
    case INDEX_11:
        printCom("11\r\n"); /*text.....*/
        break;
    case INDEX_12:
        printCom("12\r\n"); /*text.....*/
        break;
    case INDEX_13:
        printCom("13\r\n"); /*text.....*/
        break;
    case INDEX_14:
        printCom("14\r\n"); /*text.....*/
        break;
    case INDEX_15:
        printCom("15\r\n"); /*text.....*/
        break;
    case INDEX_16:
        printCom("16\r\n"); /*text.....*/
        break;
    case INDEX_17:
        printCom("17\r\n"); /*text.....*/
        break;
    case INDEX_18:
        printCom("18\r\n"); /*text.....*/
        break;
    case INDEX_19:

        printCom("19\r\n"); /*text.....*/
        break;
    case INDEX_20:
        printCom("20\r\n"); /*text.....*/
        break;
    case INDEX_21:
        printCom("21\r\n"); /*text.....*/
        break;
    case INDEX_22:
        printCom("22\r\n"); /*text.....*/
        break;
    case INDEX_23:
        printCom("23\r\n"); /*text.....*/
        break;
    case INDEX_24:
        printCom("24\r\n"); /*text.....*/
        break;
    case INDEX_25:
        printCom("25\r\n"); /*text.....*/
        break;
    case INDEX_26:
        printCom("26\r\n"); /*text.....*/
        break;
    case INDEX_27:
        printCom("27\r\n"); /*text.....*/
        break;
    case INDEX_28:
        printCom("28\r\n"); /*text.....*/
        break;
    case INDEX_29:
        printCom("29\r\n"); /*text.....*/
        break;
    case INDEX_30:
        printCom("30\r\n"); /*text.....*/
        break;
    case INDEX_31:
        printCom("31\r\n"); /*text.....*/
        break;
    case INDEX_32:
        printCom("32\r\n"); /*text.....*/
        break;
    case INDEX_33:
        printCom("33\r\n"); /*text.....*/
        break;
    case INDEX_34:
        printCom("34\r\n"); /*text.....*/
        break;

    default:printCom("NONE\r\n"); /*text.....*/
        break;
    }
}	 
