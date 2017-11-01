#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "PID.h"
#include "pwm.h"
#include "TIM.h"
#include "math.h"
#include "Control_Motor.h"
  
extern float pitch;
extern float roll;

#define Out_Speed_Max  2999	   //2500ռ�ձ�

void Speed_PIDInit(Pid_struct *motor_pid,float kp,float ki,float kd)      // PID��ʼ��   ��ʼռ�ձ�����Ϊ600    ����ʵ�ʵ���
{ 
	motor_pid->speed_target = 0;
	motor_pid->out_duty = 0;
	
	motor_pid->speed_p = kp;
	motor_pid->speed_i = ki;
	motor_pid->speed_d = kd;
	
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


//int Speed_PIDAdjust(float NEXT_POINT)
//{
//    register float  iError,dError;
//    Motor_Pid->Target = NEXT_POINT;
//    
//	Motor_Pid->ek_0= Motor_Pid->Target - roll_1;        // ƫ��
//    
//	Motor_Pid->ek_sum+=Motor_Pid->ek_0;	  // ����
//	if(Motor_Pid->ek_sum > 2300.0)					//�����޷�2300
//		Motor_Pid->ek_sum =2300.0;
//	else if(Motor_Pid->ek_sum < -2300.0)
//		Motor_Pid->ek_sum = -2300.0;	
//    
//	dError =Motor_Pid->ek_0-Motor_Pid->ek_1; 			// ��ǰ΢��
//    
//	Motor_Pid->ek_1=Motor_Pid->ek_0;
//	
//    
//    Motor_Pid->Set_pwmvalue = Motor_Pid->P * Motor_Pid->ek_0           	// ������
//          		    + Motor_Pid->I   * Motor_Pid->ek_sum 		// ������
//          		    + Motor_Pid->D * dError;
//    
//    Motor1(Motor_Pid->Set_pwmvalue);
//	return 0; 
//}

