
#include "config.h"

extern void  delay(unsigned long uldata);
extern uint8_t idata nAsrStatus;
uint8_t idata ucRegVal;


void ProcessInt0(void);


volatile uint8_t  sRecog[VOCIE_MAX_NUMBER][VOCIE_LENGTH] = {
									"a bo luo",\		
									"ji qi ren",\
									 "qian jin",\
									 "hou tui",\
									 "zuo zhuan",\
									 "you zhuan",\
									 "ting zhi",\
									 "jia su", \
									 "jian su", \
									 "kai deng",\
									 "guan deng", \
									 "he jiu", \
									 "he he da", \
									 "dian nao", \
									 "zhu ban", \
									 "nei cun tiao", \
									 "ying pan", \
									 "kai pao", \
									 "da fei ji", \
									 "he jiu", \
									 "zhong xing", \
									 "wei wei wei", \
									 "ni hao"
								};
uint8_t  g_indexsBuffer[VOCIE_MAX_NUMBER] = {
							INDEX_APOLLO,\
							INDEX_ROBOT,\
							INDEX_FORWARD,\
							INDEX_BACKWARD,\
							INDEX_LEFT,\
							INDEX_RIGHT,\
							INDEX_STOP, \
							INDEX_SPEED_UP,\
							INDEX_SPEED_DOWN, \
							INDEX_TURN_ON, \
							INDEX_TURN_OFF, \
							INDEX_11, \
							INDEX_12, \
							INDEX_13, \
							INDEX_14, \
							INDEX_15, \
							INDEX_16, \
							INDEX_17, \
							INDEX_18, \
							INDEX_19, \
							INDEX_20, \
							INDEX_21,\
							INDEX_22, \
							INDEX_23, \
							INDEX_24, \
							INDEX_25, \
							INDEX_26, \
							INDEX_27, \
							INDEX_28, \
							INDEX_29, \
							INDEX_30, \
							INDEX_31, \
							INDEX_32, \
							INDEX_33, \
							INDEX_34
						 };	

/************************************************************************
���������� 	 ��λLDģ��
��ڲ�����	 none
�� �� ֵ�� 	 none
����˵����	 none
**************************************************************************/
void LD3320Reset()
{
	RSTB=1;
	delay(5);
	RSTB=0;
	delay(5);
	RSTB=1;

	delay(5);
	CSB=0;
	delay(5);
	CSB=1;
	delay(5);
}
/************************************************************************
���������� LDģ�������ʼ��
��ڲ����� none
�� �� ֵ�� none
����˵���� �ú���Ϊ�������ã�һ�㲻��Ҫ�޸ģ�
					 ����Ȥ�Ŀͻ��ɶ��տ����ֲ������Ҫ�����޸ġ�
**************************************************************************/
void LD3320CommonInit()
{
	LD_ReadReg(0x06);  
	LD_WriteReg(0x17, 0x35); 
	delay(10);
	LD_ReadReg(0x06);  

	LD_WriteReg(0x89, 0x03);  
	delay(5);
	LD_WriteReg(0xCF, 0x43);   
	delay(5);
	LD_WriteReg(0xCB, 0x02);
	
	/*PLL setting*/
	LD_WriteReg(0x11, LD_PLL_11);       

	LD_WriteReg(0x1E,0x00);
	LD_WriteReg(0x19, LD_PLL_ASR_19); 
	LD_WriteReg(0x1B, LD_PLL_ASR_1B);		
  	LD_WriteReg(0x1D, LD_PLL_ASR_1D);
	delay(10);
	
    LD_WriteReg(0xCD, 0x04);
//	LD_WriteReg(0x17, 0x4c); 
	delay(5);
	LD_WriteReg(0xB9, 0x00);
	LD_WriteReg(0xCF, 0x4F); 
	LD_WriteReg(0x6F, 0xFF); 
}

/************************************************************************
���������� 	 LDģ�� ASR���ܳ�ʼ��
��ڲ�����	 none
�� �� ֵ�� 	 none
����˵����	 �ú���Ϊ�������ã�һ�㲻��Ҫ�޸ģ�
					 ����Ȥ�Ŀͻ��ɶ��տ����ֲ������Ҫ�����޸ġ�
**************************************************************************/
void LD3320ASRInit()
{
	LD3320CommonInit();
	LD_WriteReg(0xBD, 0x00);
	LD_WriteReg(0x17, 0x48);
	delay( 10 );
	LD_WriteReg(0x3C, 0x80);    
	LD_WriteReg(0x3E, 0x07);
	LD_WriteReg(0x38, 0xff);    
	LD_WriteReg(0x3A, 0x07);
  LD_WriteReg(0x40, 0);          
	LD_WriteReg(0x42, 8);
	LD_WriteReg(0x44, 0);    
	LD_WriteReg(0x46, 8); 
	delay( 1 );
}

/************************************************************************
���������� 	�жϴ�����
��ڲ�����	 none
�� �� ֵ�� 	 none
����˵����	��LDģ����յ���Ƶ�ź�ʱ��������ú�����
						�ж�ʶ���Ƿ��н�������û�д������ü�
            ����׼����һ�ε�ʶ��
**************************************************************************/
void ProcessInt0(void)
{
	uint8_t nAsrResCount=0;

	EX0=0;
	ucRegVal = LD_ReadReg(0x2B);
	LD_WriteReg(0x29,0) ;
	LD_WriteReg(0x02,0) ;
	if((ucRegVal & 0x10) &&
		LD_ReadReg(0xb2)==0x21 && 
		LD_ReadReg(0xbf)==0x35)			/*ʶ��ɹ�*/
	{	
		nAsrResCount = LD_ReadReg(0xba);
		if(nAsrResCount>0 && nAsrResCount<=4) 
		{
			nAsrStatus=LD_ASR_FOUND_OK;
		}
		else
	    {
			nAsrStatus=LD_ASR_FOUND_NONE;
		}	
	}															 /*û��ʶ����*/
	else
	{	 
		nAsrStatus=LD_ASR_FOUND_NONE;
	}
		
  LD_WriteReg(0x2b, 0);
  LD_WriteReg(0x1C,0);/*д0:ADC������*/

	LD_WriteReg(0x29,0) ;
	LD_WriteReg(0x02,0) ;
	LD_WriteReg(0x2B,  0);
	LD_WriteReg(0xBA, 0);	
	LD_WriteReg(0xBC,0);	
	LD_WriteReg(0x08,1);	 /*���FIFO_DATA*/
	LD_WriteReg(0x08,0);	/*���FIFO_DATA�� �ٴ�д0*/


	EX0=1;
}

/************************************************************************
���������� 	����ASRʶ������
��ڲ�����	none
�� �� ֵ��  asrflag��1->�����ɹ��� 0��>����ʧ��
����˵����	ʶ��˳������:
						1��runASR()����ʵ����һ��������ASR����ʶ������
						2��LD3320ASRInit() ����ʵ����ASR��ʼ��
						3��LD3320ASRAddCommand() ����ʵ������ӹؼ����ﵽLD3320оƬ��
						4��LD3320ASRStart()	����������һ��ASR����ʶ������					
						�κ�һ��ASRʶ�����̣�����Ҫ�������˳�򣬴ӳ�ʼ����ʼ��
**************************************************************************/
uint8_t runASR(void)
{
	uint8_t i=0;
	uint8_t asrflag=0;
	for (i=0; i<5; i++)			//	��ֹ����Ӳ��ԭ����LD3320оƬ����������������һ������5������ASRʶ������
	{
		LD3320ASRInit();
		delay(50);
		if (LD3320ASRAddCommand()==0)
		{
			LD3320Reset();			//	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
			delay(50);			//	���ӳ�ʼ����ʼ����ASRʶ������
			continue;
		}
		delay(10);
		if (LD3320ASRStart() == 0)
		{
			LD3320Reset();			//	LD3320оƬ�ڲ����ֲ���������������LD3320оƬ
			delay(50);			//	���ӳ�ʼ����ʼ����ASRʶ������
			continue;
		}
		asrflag=1;
		break;					//	ASR���������ɹ����˳���ǰforѭ������ʼ�ȴ�LD3320�ͳ����ж��ź�
	}

	return asrflag;
}
/************************************************************************
����������  ���LDģ���Ƿ����
��ڲ�����	none
�� �� ֵ�� 	flag��1-> ����
����˵����	none
**************************************************************************/
uint8_t LD3320CheckStatu()
{
	uint8_t j;
	uint8_t flag = 0;
	for (j=0; j<10; j++)
	{
		if (LD_ReadReg(0xb2) == 0x21)
		{
			flag = 1;
			break;
		}
		delay(10);		
	}
	return flag;
}

/************************************************************************
���������� 	����ASR
��ڲ�����	none
�� �� ֵ�� 	1�������ɹ�
����˵����	none
**************************************************************************/
uint8_t LD3320ASRStart()
{
	EX0=0;
	LD_WriteReg(0x35, MIC_VOL);
	LD_WriteReg(0x1C, 0x09);
	LD_WriteReg(0xBD, 0x20);
	LD_WriteReg(0x08, 0x01);
	delay( 1 );
	LD_WriteReg(0x08, 0x00);
	delay( 1 );

	if(LD3320CheckStatu() == 0)
	{
		return 0;
	}
//	LD_WriteReg(0xB6, 0xa); //ʶ��ʱ��	 1S
//	LD_WriteReg(0xB5, 0x1E); //��������ʱ�� 300ms
	LD_WriteReg(0xB8, 10); //һ������ʶ������ڣ�10S

//	LD_WriteReg(0x1C, 0x07); //����˫ͨ����Ƶ�ź���Ϊ�����ź�
	LD_WriteReg(0x1C, 0x0b); //������˷���Ϊ�����ź�


	LD_WriteReg(0xB2, 0xff);
	delay( 1);	
	LD_WriteReg(0x37, 0x06);
	delay( 1 );
    LD_WriteReg(0x37, 0x06);
		delay( 5 );
	LD_WriteReg(0x29, 0x10);
	
	LD_WriteReg(0xBD, 0x00);
	EX0=1;
	return 1;
}
/************************************************************************
���������� ��LDģ����ӹؼ���
��ڲ����� none
�� �� ֵ�� flag��1->��ӳɹ�
����˵���� �û��޸�.
					 1���������¸�ʽ���ƴ���ؼ��ʣ�ͬʱע���޸�sRecog ��g_indexsBuffer ����ĳ���
					 �Ͷ�Ӧ����k��ѭ���á�ƴ������ʶ������һһ��Ӧ�ġ�
					 2�������߿���ѧϰ"����ʶ��оƬLD3320�߽��ؼ�.pdf"��
           ���������������մ�����÷������ṩʶ��Ч����
					 3����xiao jie �� Ϊ�������ÿ��ʶ��ʱ�������ȷ�һ�����С�ݡ�
**************************************************************************/
uint8_t LD3320ASRAddCommand()
{
		uint8_t k, flag;
		uint8_t nAsrAddLength;
		
		flag = 1;
		for (k=0; k<VOCIE_MAX_NUMBER; k++)
		{
				
			if(LD3320CheckStatu() == 0)
			{
				flag = 0;
				break;
			}
			
			LD_WriteReg(0xc1, g_indexsBuffer[k] );
			LD_WriteReg(0xc3, 0 );
			LD_WriteReg(0x08, 0x04);
			delay(1);
			LD_WriteReg(0x08, 0x00);
			delay(1);
	
			for (nAsrAddLength=0; nAsrAddLength<VOCIE_LENGTH; nAsrAddLength++)
			{
				if (sRecog[k][nAsrAddLength] == 0)
					break;
				LD_WriteReg(0x5, sRecog[k][nAsrAddLength]);
			}
			LD_WriteReg(0xb9, nAsrAddLength);
			LD_WriteReg(0xb2, 0xff);
			LD_WriteReg(0x37, 0x04);
		}

	    return flag;
}
/************************************************************************
���������� 	��ȡʶ����
��ڲ�����	none
�� �� ֵ�� 	LD_ReadReg(0xc5 )��  ��ȡ�ڲ��Ĵ�������ʶ���롣
����˵����	none
**************************************************************************/
uint8_t LD3320GetResult()
{		
	return LD_ReadReg(0xc5 );
}
