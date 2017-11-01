#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "PID.h"
#include "pwm.h"
#include "TIM.h"

#define Out_Speed_Max  3999	   //2500ռ�ձ�

void Speed_PIDInit(Pid_struct *motor_pid)      // PID��ʼ��   ��ʼռ�ձ�����Ϊ600    ����ʵ�ʵ���
{ 
	motor_pid->speed_target = 0;
	motor_pid->out_duty = 0;
	
	motor_pid->speed_p = 0.06;
	motor_pid->speed_i = 0.0002;
	motor_pid->speed_d = 0;
	
	motor_pid->ek_0 	= 0;	
	motor_pid->ek_1 	= 0;	
	motor_pid->ek_sum = 0; 
}

int Speed_PIDAdjust(Pid_struct *motor_pid,int Targe,int Feedback_speed)   //Next_point Ŀ���ٶ�  
{
    float de;
		
		motor_pid->speed_target = Targe;   //Target Ŀ���ٶ�
    motor_pid->ek_0= motor_pid->speed_target - Feedback_speed;//  Ŀ��ֵ-�����ٶ�   
    de=motor_pid->ek_0-motor_pid->ek_1;
    motor_pid->ek_sum+=motor_pid->ek_0;
    
  if(motor_pid->speed_i!=0){    //�������޴���  
    if( motor_pid->ek_sum>(Out_Speed_Max/motor_pid->speed_i))   motor_pid->ek_sum=Out_Speed_Max/motor_pid->speed_i;     
   } 
    motor_pid->out_duty = motor_pid->ek_0*motor_pid->speed_p + motor_pid->speed_i*motor_pid->ek_sum+de*motor_pid->speed_d;  //PI
    
    if( motor_pid->out_duty >= Out_Speed_Max) 		motor_pid->out_duty= Out_Speed_Max;
    if(motor_pid->out_duty <= (-1) * Out_Speed_Max)		motor_pid->out_duty =(-1)*Out_Speed_Max;
	   
     motor_pid->ek_1=motor_pid->ek_0;		 
     return (motor_pid->out_duty);
}

// PIDOUT��PID������Ʋ�����ΪPWM��ȵĵ����� 
//Kp���������ڲ��� 
//Ki�����ֵ��ڲ��� 
//Kd��΢�ֵ��ڲ��� 
//error���������Ϊ�����ٶ�-ʵ���ٶȣ�����Ϊ���������� 
//d_error��������������ϴ������֮�error-pre_error�� 
//dd_error���ϴ�����������ϴ������֮�pre_error-pre_pre_error��
