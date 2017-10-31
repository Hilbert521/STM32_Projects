#include "stdio.h"
#include "TIM.h"
#include "USART2.h"
#include "ADC.h"
#include "Encode.h"
#include "PID.h"
#include "ADC.h"
#include "BLDC.h"
#include "main.h"

extern u16 Adjust;

extern u16 Exti_speed;
extern u16 Exti_speed_1;
extern u8 frequent;
extern u8 frequent_1;

extern u8 Assist;
extern vu16 I_Thrut[4];  // ��ѹֵ
extern u16 feedback_value;
/******��־λ********/
u8 AD_flag=0;
u8 Frequent_flag=0;

u32 adcx;
extern u16 KEY_Time;
extern vu16 KEY_UP[3];
extern u8 time_over;
extern u8 Adjust_flag;
extern u32 frequent_sum;

void TIM4_IRQHandler(void)
{ 
	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{	
     TIM_ClearITPendingBit(TIM4, TIM_IT_Update);  //���TIMx�����жϱ�־  
        
        Ref_Lcd_flag++;  // ˢ����Ļ��ʱ��־λ
    
      if(Exti_speed<200){
         Exti_speed++;  	   //��¼��������ģ����������֮��Ĳ�ֵ
        }else{
         Exti_speed_1=0;
        }
       
        Frequent_flag++;
     if(Frequent_flag==50)
        {   
        frequent_1=frequent;
        frequent=0;
        Frequent_flag=0;         
        }
  
       AD_flag++;     //30 ms �ɼ�����/��ѹ,DMA����  Ȼ��ȥmain���ж�
       if(AD_flag>=1)   
       {     
           Get_Adc_Average();          
           AD_flag=0;          
       }
              
       if(KEY_UP[0]==0||KEY_UP[1]==0||KEY_UP[2]==0)
       {
           KEY_Time++;
       }
			
			
       //����������״̬��ֻӰ������������  �任�ر��  ���Բ�������ʱ��
//     if(Adjust>100)
//    {
//       if(I_Thrut[0]>150||I_Thrut[1]>150||I_Thrut[2]>150)  //50ms����һ��  ��ֹͣ�Ժ���Ҫ50ms��������
//         {  
//           Speed_PIDAdjust(5000);
//             // IR2136_EN_RESET; 
//            // BLDC_Stop();
//         }else{
//           //  IR2136_EN_SET; 
//            // BLDC_Start();   //��Ӱ��PWM�����  ֻ�Ƕ��ݹر�����MOS   ���ϻָ�
//         }  
//    }      
    
	}	    
}



extern u8 Start_flag;
u8 a=0;
void TIM2_IRQHandler(void)
{ 	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//�Ǹ����ж�
	{	 
    //  if(Start_flag!=0){ 
           a++;  // 30S����һ��
         if(a==60)
          {
           TIMER++;    // ����Ϊ��λ 
           a=0;  
           }
    //  }        
      TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //���TIMx�����жϱ�־  
	}	       
}



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



void TIM2_Init(u16 arr,u16 psc)
{	
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = arr; 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); 
  TIM_Cmd(TIM2, ENABLE);  
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE ); 	  
}
