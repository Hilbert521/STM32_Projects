#include "main.h"
#include "pwm.h"

/*********ȫ�ֱ���***********/

u32 SET_SPEED=200;
u8 Key=0;	
u8 Adjust_Mode_Flag;


int main(void)
{
  /***********IOʱ�ӳ�ʼ��*********************/
    Rcc_init();
  /********�ж����ȼ�����********/
    NVIC_Config();
    
  /**********��Ļ��ʼ��*****************/  
    delay_init();
    PWM3_RESET;
    PWM_Init(3999,0);    //0��Ƶ��PWMƵ��=72000/(4000*1)=14Khz 
    
/********����/AD/��ʱ��/�ⲿ�жϳ�ʼ��*************************/ 
    
	 USART1_GPIOConfig();
     USART1_Config();
    
    TIM1_init();
    TIM2_init();   // TIM1/2 ������
    
    TIM4_Init(4999,71);   // 5ms ��������ʱ  
    Motor_Slave_Pid_Init(); 
    Motor_Pid_Init();     //PID��ʼ��  Ϊ������һ���̶��ٶ�
    Adjust_Mode_Flag = SPEED_MODE;  
    
 while(1){
  
   //    USART_Reci();      
   
 }
}
 


/*****************��ª�棬��Ҫ����д��������*********************/
void USART_Reci(void)
 {
     if(USART1_RX_STA)			//���յ�һ��������
   {
      if(USART1_RX_BUF[0]=='s')
        {  
          SET_SPEED+=100;
        }	
       else if(USART1_RX_BUF[0]=='d')
         {  
            SET_SPEED-=100;            
         }	
       else if(USART1_RX_BUF[0]=='p')
         {  
            Motor1(0); 
            SET_SPEED=0;            
         }	
         
       USART1_RX_STA=0;	
   }	
}		

  


void NVIC_Config(void)
{
	 NVIC_InitTypeDef NVIC_InitStructure; 					 //��λNVIC�Ĵ���ΪĬ��ֵ	
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);           //�����ж����ȼ���
	
	
     NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;   // 10ms �ж�
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	 NVIC_Init(&NVIC_InitStructure);	
          
      NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1; 
      NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
   
}


