#include "pwm.h"

void PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
    
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB , ENABLE);  	                                                                     	

   //���ø�����Ϊ�����������,���TIM3 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;     //TIM3_CH1��2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;     //TIM3_CH3��4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;     //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
	TIM_TimeBaseStructure.TIM_Period = arr; 	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_Pulse = 0; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  
    
    TIM_OCInitStructure.TIM_Pulse = 0; 
    TIM_OC2Init(TIM3, &TIM_OCInitStructure);  
    
    TIM_OCInitStructure.TIM_Pulse = 0; 
    TIM_OC3Init(TIM3, &TIM_OCInitStructure);  
    
    TIM_OCInitStructure.TIM_Pulse = 0; 
    TIM_OC4Init(TIM3, &TIM_OCInitStructure);  
    
    TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); 
    TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable); 	
    TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable); 
    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable); 
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM3, ENABLE);  
}

int Motor1(int Nextpoint){
    if(Nextpoint<0){
     TIM3->CCR2=-Nextpoint;
     TIM3->CCR1=0;
    }else{
     TIM3->CCR2=0;
     TIM3->CCR1=Nextpoint;
   
    }
    return 0;
}

//int Motor1(int Nextpoint){
//    if(Nextpoint<0){
//     TIM3->CCR1=-Nextpoint;
//     TIM3->CCR2=0;
//    }else{
//     TIM3->CCR1=0;
//     TIM3->CCR2=Nextpoint;
//   
//    }
//    return 0;
//}

int Motor2(int Nextpoint){

    if(Nextpoint<0){
     TIM3->CCR3=-Nextpoint;
     TIM3->CCR4=0;
    }else{
     TIM3->CCR3=0;
     TIM3->CCR4=Nextpoint;

    }
   return 0;
}

