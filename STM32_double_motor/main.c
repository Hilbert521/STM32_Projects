#include "main.h"
#include "pwm.h"
#include "SDS.h"
#include "DataScope_DP.h"


u32 SET_SPEED=300;
u8 Key=0;	
Pid_struct Main_pid_parameter,Slave_pid_parameter;

int main(void)
{
    unsigned char i;          //��������
    unsigned char Send_Count; //������Ҫ���͵����ݸ���
 
 /********�ж����ȼ�����********/    
    Rcc_init();
    NVIC_Config();
 
    delay_init();
    PWM_Init(3999,0);    //0��Ƶ��PWMƵ��=72000/(4000*1)=14Khz 
  
    USART1_Config();
    KEY_Init();    
   
    TIM1_init();
    TIM2_init();   // TIM1/2 ������
    
    TIM4_Init(4999,71);   // 5ms ��������ʱ  
    
    Speed_PIDInit(&Main_pid_parameter);    //PID��ʼ�� 
    Speed_PIDInit(&Slave_pid_parameter); 
		
 while(1){
     
//      Send_Count = DataScope_Data_Generate(3); //����10��ͨ���� ��ʽ��֡���ݣ�����֡���ݳ���
//	    for( i = 0 ; i < Send_Count; i++)  //ѭ������,ֱ���������   
//	    {
//	      while((USART1->SR&0X40)==0);  
//  	      USART1->DR = DataScope_OutPut_Buffer[i]; //�Ӵ��ڶ�һ���ֽ����ݳ�ȥ      
//	     }
//        
//      delay_ms(5); //20fps, ֡���ʱ�䡣 ��ͬ�������ü� USB-TTL �豸�����Ӿ���Ӱ���ʱ��ĳ��̣�����ʵ��Ϊ׼��  
    
 }   }


void NVIC_Config(void)
{
	 NVIC_InitTypeDef NVIC_InitStructure; 					 //��λNVIC�Ĵ���ΪĬ��ֵ	
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);           //�����ж����ȼ���
	
	
     NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;   // 10ms �ж�
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	 NVIC_Init(&NVIC_InitStructure);	
          
      NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0; 
      NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);  
}


