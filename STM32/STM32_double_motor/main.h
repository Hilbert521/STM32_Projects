#ifndef __MAIN_H
#define __MAIN_H

#include "RCC.h"
#include "stm32f10x.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_it.h"
#include "KEY.h"
#include "USART2.h"
#include "ADC.h"
#include "PID.h"
#include "TIM.h"
#include "delay.h" 
#include "stm32f10x_it.h"
#include "core_cm3.h"
#include "Flash.h"

#define FLASH_SAVE_ADDR  0x8000000+1024*127	//����FLASH �����ַ(����Ϊż���� 
extern u8 Flash_Buff[2];                                       //����ֵҪ���ڱ�������ռ��FLASH�Ĵ�С+0X08000000)

extern u32 SET_SPEED;

void  NVIC_Config(void);
void  USART_Reci(void);
void KEY_Scan(void);

#endif

