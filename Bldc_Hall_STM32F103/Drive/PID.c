#include "stdio.h"
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_it.h"
#include "BLDC.h"
#include "KEY.h"
#include "USART2.h"
#include "PID.h"
#include "main.h"


extern u16 feedback_value;

void Speed_PIDInit(Pid_struct *motor_pid)      // PID��ʼ��   ��ʼռ�ձ�����Ϊ600    ����ʵ�ʵ���
{ 
	motor_pid->speed_target = 0;
	motor_pid->out_duty = 0;
	
	motor_pid->speed_p = 0.06;
	motor_pid->speed_i = 0.0002;
	motor_pid->speed_d = 0;
	
	motor_pid->ek_0 	= 0;	
	motor_pid->ek_sum = 0; 
}


int Speed_PIDAdjust(Pid_struct *motor_pid,int Targe)   //Targe Ŀ���ٶ�  
{
	//����PID����Ŀ���ٶȵķ�Χ
	motor_pid->speed_target = Targe > MAX_Speed_Targe ? MAX_Speed_Targe : Targe;
	motor_pid->speed_target = Targe < MIN_Speed_Targe ? MIN_Speed_Targe : Targe;
	
	motor_pid->speed_target = Targe;   //speed_target Ŀ���ٶ�
    
  motor_pid->ek_0= motor_pid->speed_target - feedback_value;//  Ŀ��ֵ-�����ٶ�
    
  motor_pid->ek_sum+=motor_pid->ek_0;
    
  if(motor_pid->speed_i!=0){    //�������޴���  
    if( motor_pid->ek_sum>(Out_Speed_Max/motor_pid->speed_i))
     {
       motor_pid->ek_sum=Out_Speed_Max/motor_pid->speed_i;
     }   
  }
  
   motor_pid->out_duty = motor_pid->ek_0*motor_pid->speed_p + motor_pid->speed_i*motor_pid->ek_sum;  //PI
    
   if(motor_pid->out_duty >= Out_Speed_Max) 	motor_pid->out_duty= Out_Speed_Max;
	 if(motor_pid->out_duty <= Out_Speed_Min) 	motor_pid->out_duty= Out_Speed_Min;
	     
   TIM1->CCR1=motor_pid->out_duty;	
   TIM1->CCR2=motor_pid->out_duty;
   TIM1->CCR3=motor_pid->out_duty;
	 
   return(motor_pid->out_duty);
}


