#include  "EXTI.h"
#include "USART2.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "delay.h"
#include "PID.h"
#include "main.h"
//�ⲿ�жϳ�ʼ������


void EXTI1_IRQHandler(void)  //̤Ƶ�ж�   ÿһ���жϼ���һ��   TIM��ʱ��1s�������ֵ  
{                               //��Ϊ���ٲ���
  if(EXTI_GetITStatus(EXTI_Line1)!=RESET)  
   {          

   }    
  EXTI_ClearITPendingBit(EXTI_Line1);  //����жϱ�־λ 
}



 void EXTI0_IRQHandler(void)   //��¼��������֮���ʱ��   ������
{	    

        EXTI_ClearITPendingBit(EXTI_Line0);   
}


void EXTI9_5_IRQHandler(void)  
{	       

       EXTI_ClearITPendingBit(EXTI_Line11);    //���LINE5�ϵ��жϱ�־λ  
}


 void EXTI15_10_IRQHandler(void)   //PC1  ɲ���ж�  
{	       

       EXTI_ClearITPendingBit(EXTI_Line11);    //���LINE5�ϵ��жϱ�־λ  
}



void EXTI10_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��  
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA

    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
 	GPIO_Init(GPIOB, &GPIO_InitStructure);  
    GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
 	GPIO_Init(GPIOB, &GPIO_InitStructure);  
    
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource11);
    
    EXTI_InitStructure.EXTI_Line=EXTI_Line10;
 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 	EXTI_Init(&EXTI_InitStructure);	 
    
 	EXTI_InitStructure.EXTI_Line=EXTI_Line11;
 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 	EXTI_Init(&EXTI_InitStructure);	 	  
}


void EXTI1_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTA,PORTCʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
 	GPIO_Init(GPIOA, &GPIO_InitStructure);   
    
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);

 	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ��� 
}

void EXTI0_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//�ⲿ�жϣ���Ҫʹ��AFIOʱ��  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTA,PORTCʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
 	GPIO_Init(GPIOA, &GPIO_InitStructure);   
    
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

 	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ��� 
}

