
#include "config.h"


#define   FOSC     22118400L         //System frequency

uint32_t  baudRate = 9600;           //UART baudrate
uint8_t   rxBuffer[20];
uint8_t   rxCount = 0;
uint8_t   rxFlag =  0;
/************************************************************************
�� �� ���� ���ڳ�ʼ��
���������� STC10L08XE ��Ƭ�����ڳ�ʼ������
���غ����� none
����˵���� none
**************************************************************************/
void uartInit(void)
{
    SCON = 0x50;            //8-bit variable UART
    TMOD = 0x20;            //Set Timer1 as 8-bit auto reload mode
    TH1 = TL1 = -(FOSC/12/32/baudRate); //Set auto-reload vaule
    TR1 = 1;                //Timer1 start run
    ES = 1;                 //Enable UART interrupt
    EA = 1;                 //Open master interrupt switch
}
/***********************************************************
* ��    �ƣ� 
* ��    �ܣ� 
* ��ڲ����� �� 
* ���ڲ�������
* ˵    ���� 					 
**********************************************************/
void UartIsr() interrupt 4 using 1
{	
	if(RI)
	{	
		RI = 0;             //Clear receive interrupt flag
        rxBuffer[rxCount++] = SBUF;          //P0 show UART data
		if(rxCount == 20)	  
			rxCount = 0;
	}
	if(rxBuffer[rxCount-2]== '\r' && rxBuffer[rxCount-1] == '\n')
	 {
	 	rxBuffer[rxCount-2] = 0;
		rxBuffer[rxCount-1]=0;
		rxCount= 0 ;
		rxFlag=1;
	 } 


}
/************************************************************************
���������� 	���ڷ���һ�ֽ�����
��ڲ�����	DAT:�����͵�����
�� �� ֵ�� 	none
����˵����	none
**************************************************************************/
void uartSendByte(uint8_t _data)
{
	ES  =  0;
	TI=0;
	SBUF = _data;
	while(TI==0);
	TI=0;
	ES = 1;
}
/************************************************************************
���������� ���ڷ����ַ�������
��ڲ����� 	*_data���ַ���ָ��
�� �� ֵ�� none
**************************************************************************/
void printCom(uint8_t *_data)
{
	while(*_data)
	{
	 	uartSendByte(*_data++);
	}	
}

