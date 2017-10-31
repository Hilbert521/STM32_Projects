#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "PID.h"
#include "main.h"
#include "pwm.h"

#define ADC_Speed_Max   600	 //2500ռ�ձ�
#define ADC_Speed_Min   2200
  
typedef struct PI 
{ 
	int Target;     //�趨Ŀ�� 
	int Uk;			//��ǰ�ٶ�
    int Set_pwmvalue;  // ���ռ�ձ�
	double P; 		//��������
	double I; 		//���ֳ���
	int ek_0;		//��ǰ���
	int ek_sum; 	// ������
}PI;


static PI Motor; 
static PI *Motor_Pid = &Motor; 

void Motor_Pid_Init(void)      // PID��ʼ��   ��ʼռ�ձ�����Ϊ600    ����ʵ�ʵ���
{ 
	Motor_Pid->Target = 0;
	Motor_Pid->Uk		= 0;
    Motor_Pid->Set_pwmvalue     = 0; 
	Motor_Pid->ek_0 	= 0;	
	Motor_Pid->ek_sum = 0; 
	Motor_Pid->P 		= 6;  //��������
	Motor_Pid->I 		= 0.25; 	 //���ֳ���
}

s32 Feedback_Motor=0;		   //�ٶȷ���ֵ
s32 Feedback_Motor_Slave=0;		   //�ٶȷ���ֵ

int Motor_PIDAdjust(int NEXT_POINT)   //Next_point Ŀ���ٶ�  
{    
  
    
	Motor_Pid->Target = NEXT_POINT;   //Target Ŀ���ٶ�
   
    Motor_Pid->ek_0= Motor_Pid->Target - Feedback_Motor;//  Ŀ��ֵ-�����ٶ�
    
    Motor_Pid->ek_sum+=Motor_Pid->ek_0;
    
  if(Motor_Pid->I!=0){    //�������޴���  
    if( Motor_Pid->ek_sum>(4000/Motor_Pid->I))
     {
       Motor_Pid->ek_sum=4000/Motor_Pid->I;
     }   
  }
  
    Motor_Pid->Set_pwmvalue = Motor_Pid->ek_0*Motor_Pid->P + Motor_Pid->I*Motor_Pid->ek_sum;  //PI
    
    if( Motor_Pid->Set_pwmvalue >= 3999)
	{
		Motor_Pid->Set_pwmvalue= 3999;
	}
	
    if(Motor_Pid->Set_pwmvalue <= -3999)
	{
		Motor_Pid->Set_pwmvalue =-3999;
	}  
    
     Motor1(Motor_Pid->Set_pwmvalue);
    	
    return(Motor_Pid->Set_pwmvalue);
}



static PI Motor_Slave; 
static PI *Motor_Slave_Pid = &Motor_Slave; 

void Motor_Slave_Pid_Init(void)      // PID��ʼ��   ��ʼռ�ձ�����Ϊ600    ����ʵ�ʵ���
{ 
	Motor_Slave_Pid->Target = 0;
	Motor_Slave_Pid->Uk		= 0;
    Motor_Slave_Pid->Set_pwmvalue     = 0; 
	Motor_Slave_Pid->ek_0 	= 0;	
	Motor_Slave_Pid->ek_sum = 0; 
	Motor_Slave_Pid->P 		=6;  //��������
	Motor_Slave_Pid->I 		= 0.25; 	 //���ֳ���
}

 u8  Motor_Slave_Position_Kp=0;
 s32  Motor_Position_ek_0=0;
 s32  Motor_Position=0;
 s16  Motor_Position_Stop=0;
 s16  Motor_Position_Speed=0;
int Motor_Slave_PIDAdjust(int NEXT_POINT)   //Next_point Ŀ���ٶ�  
{

    
  if(Adjust_Mode_Flag==POSITION_MODE){
    Motor_Position_ek_0=NEXT_POINT-Motor_Position; 
    Motor_Position_Stop=1000;
    if(Motor_Position_ek_0>0){
      if(Motor_Position_ek_0>Motor_Position_Stop){
         Motor_Position_Speed=3000;
      }else{
        Motor_Position_Speed= Motor_Position_ek_0*3000/10;   // ���*����ٶ�/����ϵ��
      }
    }else {
         if(abs(Motor_Position_ek_0)>Motor_Position_Stop){
             Motor_Position_Speed=-3000;
          }else{
            Motor_Position_Speed=-(Motor_Position_ek_0*3000/10);   // ���*����ٶ�/����ϵ��
          } 
     }         
    }
 
    
	Motor_Slave_Pid->Target = NEXT_POINT;   //Target Ŀ���ٶ�
   
    Motor_Slave_Pid->ek_0= Motor_Slave_Pid->Target - Feedback_Motor_Slave;//  Ŀ��ֵ-�����ٶ�
    
    Motor_Slave_Pid->ek_sum+=Motor_Slave_Pid->ek_0;
    
  if(Motor_Slave_Pid->I!=0){    //�������޴���  
    if( Motor_Slave_Pid->ek_sum>(4000/Motor_Slave_Pid->I))
     {
       Motor_Slave_Pid->ek_sum=4000/Motor_Slave_Pid->I;
     }   
  }
  
    Motor_Slave_Pid->Set_pwmvalue = Motor_Slave_Pid->ek_0*Motor_Slave_Pid->P + Motor_Slave_Pid->I*Motor_Slave_Pid->ek_sum;  //PI
    
    if( Motor_Slave_Pid->Set_pwmvalue >= 3000)
	{
		Motor_Slave_Pid->Set_pwmvalue= 3000;
	}
	
    if(Motor_Slave_Pid->Set_pwmvalue <= -3000)
	{
		Motor_Slave_Pid->Set_pwmvalue =-30000;
	}  
     
    Motor2(Motor_Slave_Pid->Set_pwmvalue);   
	
    return(Motor_Slave_Pid->Set_pwmvalue);
}





//void SpeedAdjust()
//{
//    long d_error,dd_error,error;        //��������
//    error = (int)(speed_ept - now_speed);           //���㱾�����(�ڴ��ٶ�-��ǰ�ٶ�)
//    d_error = error - pre_error;        //����������ϴ����֮��
//    dd_error = pre_error - pre_pre_error;       //�ϴ���������ϴ����֮��
//    pre_error = error;          //��������ֵ���ϴ����(�´μ�����)
//    pre_pre_error = pre_error;      //���ϴ���ֵ�����ϴ����(�´μ�����)
//    pwm_tmp = pwm_tmp + PID_P*d_error + PID_I*error + PID_D*dd_error;       //����pwm��ȵ�����
//    if(now_speed>22)        //�����ǰ�ٶȴ���22
//    {
//        Set_DCMotor(0,0);       //���������
//    }
//    else        //����
//    {
//        if(pwm_tmp>=0)      //���pwm��ȵ���Ϊ��(���ӿ��)
//        {
//            Set_DCMotor(pwm_tmp,0);     //����ֱ�����ת��
//        }
//        else        //����
//        {
//            Set_DCMotor(pwm_tmp,1);     //����ֱ�����ת��
//        }
//    }
//    before_speed = now_speed;       //�������ٶȸ�ֵ���ϴ��ٶ�(�´�ʹ��)
//}


// PIDOUT��PID������Ʋ�����ΪPWM��ȵĵ����� 
//Kp���������ڲ��� 
//Ki�����ֵ��ڲ��� 
//Kd��΢�ֵ��ڲ��� 
//error���������Ϊ�����ٶ�-ʵ���ٶȣ�����Ϊ���������� 
//d_error��������������ϴ������֮�error-pre_error�� 
//dd_error���ϴ�����������ϴ������֮�pre_error-pre_pre_error��
