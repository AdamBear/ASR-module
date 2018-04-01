#ifndef LD_CHIP_H
#define LD_CHIP_H

#define VOCIE_MAX_NUMBER  35  /*�����ά��ֵ*/
#define VOCIE_LENGTH      20		/*����һά��ֵ*/

//	�������״̬����������¼������������ASRʶ������е��ĸ�״̬
#define LD_ASR_IDLE				0x00	 /*	��ʾû������ASRʶ��*/
#define LD_ASR_RUNING			0x01	/*	��ʾLD3320������ASRʶ����*/
#define LD_ASR_FOUND_OK			0x10	/*��ʾһ��ʶ�����̽�������һ��ʶ����*/
#define LD_ASR_FOUND_NONE 		0x11	/*��ʾһ��ʶ�����̽�����û��ʶ����*/
#define LD_ASR_ERROR	 		0x31	/*��ʾһ��ʶ��������LD3320оƬ�ڲ����ֲ���ȷ��״̬*/


#define CLK_IN   		22.1184	/* �û�ע���޸�����ľ���ʱ�Ӵ�С */
#define LD_PLL_11			(uint8_t)((CLK_IN/2.0)-1)
#define LD_PLL_MP3_19		0x0f
#define LD_PLL_MP3_1B		0x18
#define LD_PLL_MP3_1D   	(uint8_t)(((90.0*((LD_PLL_11)+1))/(CLK_IN))-1)

#define LD_PLL_ASR_19 		(uint8_t)(CLK_IN*32.0/(LD_PLL_11+1) - 0.51)
#define LD_PLL_ASR_1B 		0x48
#define LD_PLL_ASR_1D 		0x1f

//��������
void LD3320Reset();
void LD3320CommonInit();
void LD3320ASRInit();
uint8_t runASR(void);
uint8_t LD3320ASRStart();
uint8_t LD3320ASRAddCommand();
uint8_t LD3320GetResult();


//ʶ����ͻ��޸Ĵ� 
#define		INDEX_APOLLO    0x00
#define 	INDEX_ROBOT     0x01
#define  	INDEX_FORWARD 	0x02
#define  	INDEX_BACKWARD  0x03
#define  	INDEX_LEFT 		0x04
#define 	INDEX_RIGHT		0x05
#define     INDEX_STOP              0x06
#define 	INDEX_SPEED_UP	0x07
#define 	INDEX_SPEED_DOWN  	0x08
#define 	INDEX_TURN_ON		0x09
#define		INDEX_TURN_OFF		0x0A
#define 	INDEX_11			0x0B
#define 	INDEX_12			0x0C
#define 	INDEX_13			0x0D
#define 	INDEX_14			0x0E
#define 	INDEX_15			0x0F
#define 	INDEX_16			0x10
#define 	INDEX_17			0x11
#define 	INDEX_18			0x12
#define		INDEX_19			0x13
#define		INDEX_20			0x14
#define 	INDEX_21			0x15
#define		INDEX_22			0x16
#define		INDEX_23			0x17
#define		INDEX_24			0x18
#define		INDEX_25			0x19
#define		INDEX_26			0x1A
#define		INDEX_27			0x1B
#define		INDEX_28			0x1C
#define 	INDEX_29			0x1D
#define 	INDEX_30			0x1E
#define		INDEX_31			0x1F
#define		INDEX_32			0x20
#define		INDEX_33			0x21
#define		INDEX_34			0x22
#define		INDEX_35			0x23



//��ֵԽ��Խ����ʶ�����ԽԶ������ʶ���ʾ�Խ�� �����Լ���ʵ��������ڡ�
#define MIC_VOL 0x55	 //��ͷ���棨�����ȵ��ڣ� ��Χ��00-7f

 
#endif
