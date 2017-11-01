#include "stdio.h"
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_it.h"
#include "BLDC.h"
#include "KEY.h"
#include "USART2.h"
#include "PID.h"
#include "main.h"

uint16_t Speed_count;
u8 duzhuan_value;		   //��ת������
extern u32 feedback_value;

void TIM1_TRG_COM_IRQHandler(void)
{ 
   TIM1->SR&=0;           //���жϱ�־ 
 //  Get_Adc_Average();	  
   // uprintf(USART1,"feedback %d\n",feedback_value);   
}
extern u8 ERROR_flag;
u16 duzhuan;
extern u8 frequent_1;
void TIM3_IRQHandler(void)
{ u8 j;
  static uint8_t time_over  = 0; 	//��ʱ���������
  static u8 enc_j;
  static u32 sp_sum;
  static u32 data[6]; 
  
  if(TIM3->SR&0x0010)       		//�����Ƚ�/�����¼����жϼ��������  
  {
     
	TIM3->SR&=0xffef;	    		//���жϱ�־
	if(time_over>2)
	{
	   /*������㡡��ת����*/
	   time_over=0;	   		  
	   duzhuan_value++;       	//stalling_count ��ת����*/	  
	}
	time_over++;       
//  if(frequent_1!=0&&state==RUN&&duzhuan_value>3){
//      state=FAULT; 
//      ERROR_flag=3;
//      BLDC_Stop();
//      }  
  if(state==CART&&duzhuan_value>4){
      state=FAULT; 
      ERROR_flag=3;
      BLDC_Stop();
      }  
  
  } 
	
if(TIM3->SR&0x0040)      		//TIF�����жϱ��   0100 0000  
  { 
    TIM3->SR&=0xffbf;        	//���жϱ�־
      
  // TIM3->CCR2=TIM3->CCR1>>1;	// ���㻻���   bug��������com�¼�
   huanxiang(); 
   data[enc_j]=TIM3->CCR1+(65535*time_over);
      
   enc_j++;   
  if(enc_j>5)		 
    {
        for(j=0;j<6;j++)
        {
            sp_sum+=data[j];
        }
	   sp_sum/=6;
	   enc_j=0;
	}	
   feedback_value=40000-sp_sum*2;  //����ٶȷ���ֵ	  // ������ ���һ�����ȹ�ϵ
                                  // �ٶ�Խ�죬����ֵԽС��������  �ٶȿ�--����ֵ��
	time_over = 0;			  	//�����������
	duzhuan_value = 0;		
  }     		
}





