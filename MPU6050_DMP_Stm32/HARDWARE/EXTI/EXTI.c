#include  "EXTI.h"
#include "USART2.h"
#include "stdio.h"
#include "stm32f10x.h"
#include "delay.h"
#include "PID.h"
#include "main.h"
//外部中断初始化函数


void EXTI1_IRQHandler(void)  //踏频中断   每一次中断计数一次   TIM定时器1s读会计数值  
{                               //作为调速参数
  if(EXTI_GetITStatus(EXTI_Line1)!=RESET)  
   {          

   }    
  EXTI_ClearITPendingBit(EXTI_Line1);  //清除中断标志位 
}



 void EXTI0_IRQHandler(void)   //记录两个脉冲之间的时间   并清零
{	    

        EXTI_ClearITPendingBit(EXTI_Line0);   
}


void EXTI9_5_IRQHandler(void)  
{	       

       EXTI_ClearITPendingBit(EXTI_Line11);    //清除LINE5上的中断标志位  
}


 void EXTI15_10_IRQHandler(void)   //PC1  刹车中断  
{	       

       EXTI_ClearITPendingBit(EXTI_Line11);    //清除LINE5上的中断标志位  
}



void EXTI10_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟  
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//使能PORTA

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
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTC时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_1;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
 	GPIO_Init(GPIOA, &GPIO_InitStructure);   
    
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource1);

 	EXTI_InitStructure.EXTI_Line=EXTI_Line1;
 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器 
}

void EXTI0_Init(void)
{
 	GPIO_InitTypeDef GPIO_InitStructure;
 	EXTI_InitTypeDef EXTI_InitStructure;
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);//外部中断，需要使能AFIO时钟  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA,PORTC时钟
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  
 	GPIO_Init(GPIOA, &GPIO_InitStructure);   
    
 	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);

 	EXTI_InitStructure.EXTI_Line=EXTI_Line0;
 	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
 	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器 
}

