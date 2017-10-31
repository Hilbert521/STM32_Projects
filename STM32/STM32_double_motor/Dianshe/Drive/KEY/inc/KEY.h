#ifndef _KEY_H
#define _KEY_H
#include "stm32f10x.h"

#define KEY0  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)//��ȡ����0
#define KEY1  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)//��ȡ����1
#define KEY2  GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)//��ȡ����2 

#define KEY0_s	3		//KEY0 
#define KEY0_l	4		//KEY0 

#define KEY1_s	5		//KEY1 
#define KEY1_l	6		//KEY1 

#define KEY2_s	7		//KEY1 
#define KEY2_l	8		//KEY1 

void KEY_Init(void);//IO��ʼ��
u8 KEY_Scan1(u8 mode);  	//����ɨ�躯��	

#endif
