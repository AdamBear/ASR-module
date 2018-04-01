
#include "config.h"

#define LD_INDEX_PORT		(*((volatile unsigned char xdata*)(0x8100))) 
#define LD_DATA_PORT		(*((volatile unsigned char xdata*)(0x8000))) 


//�������� MCU��P2.0 ���ӵ� LDоƬ��A0
//         MCU��P2.1 ���ӵ� LDоƬ��CSB
//         MCU��RD��WR ���� LDоƬ��RD��WR (xdata ��дʱ�Զ��������ź�)
//
//0x8100�Ķ�������10000001 00000000		CSB=0 A0=1
//                 ^     ^
//0x8000�Ķ�������10000000 00000000		CSB=0 A0=0
//                 ^     ^		
void LD_WriteReg( unsigned char address, unsigned char dataout )
{
	LD_INDEX_PORT  = address;         
	LD_DATA_PORT = dataout;          
}

unsigned char LD_ReadReg( unsigned char address )
{
	LD_INDEX_PORT = address;         
	return (unsigned char)LD_DATA_PORT;     
}

