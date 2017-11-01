#include "string.h"
#include "stdio.h"
#include "USART2.h"
#include "BLDC.h"
#include "PID.h"
#include "stdarg.h"

//���ڽ��ջ����� 	
u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.
u16 USART1_RX_STA=0;  

void USART1_IRQHandler(void)
{
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)//���յ�����
	{	 
        u8 res;	  
	    res =USART_ReceiveData(USART1);		
		USART1_RX_BUF[USART1_RX_STA++]=res;		//��¼���յ���ֵ	 
	}  											 	
}

void USART1_GPIOConfig(void)   //����1��ӳ����PB6/7 
{
   GPIO_InitTypeDef	GPIO_InitStruct;
    
     GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOB, &GPIO_InitStruct);

     GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
     GPIO_Init(GPIOB,&GPIO_InitStruct);   
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);     
}


void USART1_Config(void)
{
  USART_InitTypeDef USART_InitStruct;

 	USART_InitStruct.USART_BaudRate            = 115200;	  					
	USART_InitStruct.USART_StopBits            = USART_StopBits_1;	 
	USART_InitStruct.USART_WordLength          = USART_WordLength_8b; 
	USART_InitStruct.USART_Parity              = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

	/* ���ô���2 */
	USART_Init(USART1, &USART_InitStruct);
	/* ����1�����ж�ʹ�� USART_IT_RXNE : ���ղ����ж� */
  	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	/* ʹ�ܴ���2 */
	USART_Cmd(USART1, ENABLE);
	
}


void uprintf(USART_TypeDef* USARTx, char *fmt, ...)
 {
	char buffer[128];
	u8 i = 0;
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, 128, fmt, arg_ptr);
	USART_ClearFlag(USARTx,USART_FLAG_TXE);
	while ((i < 128) && buffer[i])
	{
		if(buffer[i] == '\n'){
			
			USART_SendData(USARTx,(u8)'\r');
			while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); 
			USART_SendData(USARTx,(u8)buffer[i++]);
		}else{
	    USART_SendData(USARTx, (u8) buffer[i++]);
		}
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); 
	}
	va_end(arg_ptr);
 }
