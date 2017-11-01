#include "stdio.h"
#include "TIM.h"
#include "USART2.h"
#include "PID.h"
#include "math.h"
#include "pwm.h"
#include "SDS.h"
#include "DataScope_DP.h"

/******��־λ********/
extern s16 KEY_Time;
extern u16 KEY_UP[3];
extern Pid_struct Main_pid_parameter,Slave_pid_parameter;

u8 RUN_MODE=0;
float Zheng_W=1;     //ŷ�׸�
u16 Fang_T=300;     //��������

s32 Targe_SPEED=300;   // �趨�����Ŀ��ת��
	
void TIM4_IRQHandler(void)
{   static u32 Fang_Flag=0;
    static float Zheng_Flag=0;
    s32 Feedback_Motor;   // ����������ٶȷ���
    s32 Feedback_Motor_Slave;
		
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
  {	
     TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx�����жϱ�־      
        
        Feedback_Motor=TIM_GetCounter(TIM1)-6000;   // ��������������ٶ�  
        Feedback_Motor_Slave=TIM_GetCounter(TIM2)-6000;  			
        TIM1->CNT =6000; 
        TIM2->CNT =6000;
       
      if(RUN_MODE==0){  // ���Ҳ�
           Zheng_Flag+=0.01;
           Targe_SPEED=300*sin(Zheng_W*Zheng_Flag);
      }else{             //����
           Fang_Flag++;
           if(Fang_Flag%Fang_T==0){
               Targe_SPEED=Targe_SPEED*-1; 
        }    }
        
     Motor1(Speed_PIDAdjust(&Main_pid_parameter,Targe_SPEED,Feedback_Motor));  // pid����������ٶ�
     Motor2(Speed_PIDAdjust(&Slave_pid_parameter,Feedback_Motor,Feedback_Motor_Slave));  // ��������������ٶȵ��ڸ�����ٶ�
       
     DataScope_Get_Channel_Data( Targe_SPEED , 1 );  //������ 1.0  д��ͨ�� 1
     DataScope_Get_Channel_Data( Feedback_Motor , 2 );  //������ 2.0  д��ͨ�� 2
     DataScope_Get_Channel_Data( -Feedback_Motor_Slave , 3 );  //������ 2.0  д��ͨ�� 2           
		
    if(KEY_UP[0]==0||KEY_UP[1]==0||KEY_UP[2]==0)  KEY_Time++;
       	
  }	    }
  

/*
TIM4  ��ʱ�ж�
*/
void TIM4_Init(u16 arr,u16 psc)
{	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
    TIM_Cmd(TIM4, ENABLE);  //ʹ��TIMx		
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); //ʹ��ָ����TIM4�ж�,��������ж� 	  
}


void TIM1_init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef tim_TimeBaseInitTypeDef;
	TIM_ICInitTypeDef tim_ICInitTypeDef;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_TIM1, ENABLE);	
	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_8;   //  PA8/9
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 TIM_DeInit(TIM1);
	 TIM_TimeBaseStructInit(&tim_TimeBaseInitTypeDef);
	 
	 tim_TimeBaseInitTypeDef.TIM_Prescaler = 0x0;  //����Ƶ 
	 tim_TimeBaseInitTypeDef.TIM_Period =0xffff; 
	 tim_TimeBaseInitTypeDef.TIM_ClockDivision =TIM_CKD_DIV1;  
	 tim_TimeBaseInitTypeDef.TIM_CounterMode =TIM_CounterMode_Up; 
	 TIM_TimeBaseInit(TIM1,&tim_TimeBaseInitTypeDef);
	 
	 TIM_EncoderInterfaceConfig(TIM1,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);   //IC1 IC2������
	 TIM_ICStructInit(&tim_ICInitTypeDef);
	 tim_ICInitTypeDef.TIM_ICFilter = 0x6;
	 TIM_ICInit(TIM1,&tim_ICInitTypeDef);
	 
	 TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	 TIM_ITConfig(TIM1, TIM_IT_Update,ENABLE);
	 
	 TIM1->CNT =6000;
	 
	 TIM_Cmd(TIM1, ENABLE); 
}



void TIM2_init(){
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef tim_TimeBaseInitTypeDef;
	TIM_ICInitTypeDef tim_ICInitTypeDef;
	
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
    
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;   //  PA8/9
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	 TIM_DeInit(TIM2);
	 TIM_TimeBaseStructInit(&tim_TimeBaseInitTypeDef);
	 
	 tim_TimeBaseInitTypeDef.TIM_Prescaler = 0x0;  //����Ƶ 
	 tim_TimeBaseInitTypeDef.TIM_Period =0xffff; 
	 tim_TimeBaseInitTypeDef.TIM_ClockDivision =TIM_CKD_DIV1;  
	 tim_TimeBaseInitTypeDef.TIM_CounterMode =TIM_CounterMode_Up; 
	 TIM_TimeBaseInit(TIM2,&tim_TimeBaseInitTypeDef);
	 
	 TIM_EncoderInterfaceConfig(TIM2,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);   //IC1 IC2������
	 TIM_ICStructInit(&tim_ICInitTypeDef);
	 tim_ICInitTypeDef.TIM_ICFilter = 0x6;
	 TIM_ICInit(TIM2,&tim_ICInitTypeDef);
	 
	 TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	 TIM_ITConfig(TIM2, TIM_IT_Update,ENABLE);
	 
	 TIM2->CNT =6000;
	 
	 TIM_Cmd(TIM2, ENABLE); 
}



/*
TIM2  AD����
*/
//void TIM2_init(){
//    TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
//    GPIO_InitTypeDef GPIO_InitStructure;
//    
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; //50Mʱ���ٶ�
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//    
//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
//    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);
//    
//   //����TIMER1��Ϊ������
//    TIM_DeInit(TIM2);

//    TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
//    TIM_TimeBaseStructure.TIM_Prescaler = 0x00;
//    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;
//    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); // Time base configuration
//    
// //   TIM_TIxExternalClockConfig(TIM3,TIM_TIxExternalCLK1Source_TI12,TIM_ICPolarity_Rising,0);
//    TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);

//    TIM_SetCounter(TIM2, 0);
//    TIM_Cmd(TIM2, ENABLE);
//}



//        if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)==SET){
//      //   uprintf(USART1,"I_Thrut 1 %d\n",TIM_GetCounter(TIM2));
//             TIM2->CNT =0; 
//        } else{
//      //  uprintf(USART1,"I_Thrut 1 %d\n",-TIM_GetCounter(TIM2));
//         TIM2->CNT =0; 
//        }  

