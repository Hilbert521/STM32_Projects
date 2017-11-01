#ifndef _USART1_H
#define _USART1_H
#include "stm32f10x.h"

#define USART1_MAX_RECV_LEN		200					//�����ջ����ֽ���
extern u8  USART1_RX_BUF[USART1_MAX_RECV_LEN]; 		//���ջ���,���USART2_MAX_RECV_LEN�ֽ�
extern u16 USART1_RX_STA;   						//��������״̬

extern void USART1_GPIOConfig(void);
extern void USART1_Config(void);
extern void uprintf(USART_TypeDef* USARTx, char *fmt, ...);
#endif
