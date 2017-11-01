#include "sys.h"	
#include "delay.h"	
#include "key.h"   
#include "mpu6050.h"  
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "USART2.h"
#include "pwm.h"
#include "PID.h"
#include "TIM.h"
#include "KEY.h"
#include "outputdata.h"

u8 RUN_MODE=0;
u8 Print=0;

float pitch,roll,yaw; 		//ŷ����
short aacx,aacy,aacz;		//���ٶȴ�����ԭʼ����
short gyrox,gyroy,gyroz;	//������ԭʼ����
Pid_struct motor13_parameter,motor24_parameter;  // ���pid����

void NVIC_Config(void);

int main(void)
{ 
	  short temp;					//�¶�	    
	  
		SystemInit();
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    NVIC_Config(); 
   	delay_init();	    	 //��ʱ������ʼ��	
    PWM_Init(3000,0);    //0��Ƶ��PWMƵ��=72000/(4000*1)=14Khz 
    PWM_Init_2(3000,0);   
    USART1_Config();   
    KEY_Init();    
 
   Speed_PIDInit(&motor13_parameter,30,0.2,100);   // ���pid ������ʼ��
	 Speed_PIDInit(&motor24_parameter,60,0.2,200);
	
 	 MPU_Init();		
   delay_ms(500);   
   while(mpu_dmp_init());   //��ʼ��MPU6050_DMP
     
   TIM4_Init(10000-1,71);   //��ʱ����ʼ��
   uprintf(USART1," ϵͳ�����ɹ�!\r\n");
	 
while(1)
{               
   //  	 MPU_Get_Accelerometer(&aacx,&aacy,&aacz);	//�õ����ٶȴ���������
   //    MPU_Get_Gyroscope(&gyrox,&gyroy,&gyroz);	//�õ�����������   
  KEY_Scan();
  if(Print==1){   // �������ߵ���λ��
    
     OutData[0]=pitch;
     OutData[1]=roll;
     OutData[2]= motor13_parameter.out_duty/100;         
     OutPut_Data(); 
}  	} 	}


void NVIC_Config(void)
{
         
    
	 NVIC_InitTypeDef NVIC_InitStructure; 					 //��λNVIC�Ĵ���ΪĬ��ֵ	
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);           //�����ж����ȼ���

	RCC_APB2PeriphClockCmd(   RCC_APB2Periph_GPIOA 
							| RCC_APB2Periph_GPIOB
			 	 	 	 	| RCC_APB2Periph_GPIOC 
         //   | RCC_APB2Periph_GPIOD 
					//	| RCC_APB2Periph_AFIO
							| RCC_APB2Periph_TIM1
			  	//	| RCC_APB2Periph_ADC1
              | RCC_APB2Periph_USART1
 							, ENABLE );

 	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3
							| RCC_APB1Periph_TIM4
              | RCC_APB1Periph_TIM2
							, ENABLE);
      
    
              //��λNVIC�Ĵ���ΪĬ��ֵ	  
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   //��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  	    //��Ӧ���ȼ�0
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	            
	  NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0; 
		NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

