#include "pwm.h"
#include "PID.h"
#include "math.h"
#include "TIM.h"
#include "DataScope_DP.h"
#include "delay.h"
#include "Control_Motor.h"

//     TIM3->CCR1=1000;     // ���1 CCR1>0 ����   CCR2>0  ����
//     TIM3->CCR2=0;   
//      
//     TIM3->CCR3=1000;     //���2 CCR3>0 ����   CCR4>0  ����
//     TIM3->CCR4=0;
     
//  TIM2->CCR3=2000;     //���3 CCR3>0 ����  CCR4>0  ����
//  TIM2->CCR4=0;
     
//  TIM2->CCR1=0;     //���4  CCR2>0 ����   CCR1>0  ����
//  TIM2->CCR2=1000;
     
/*
   1/3 X�� pitch 3 Ϊ������
   
   2/4 Y�� roll  4 Ϊ������
*/

/*
   ���1/2  IN1/2/3/4  �� PA6/7  PB0/1
   ���3/4  IN1/2/3/4  �� PA1/0/2/3  
   
   TIM3  CCR1/2 ����2�ŵ����CCR1>0  
   TIM3  CCR3/4 ����1�ŵ����CCR3>0  ����   
   
   TIM2  CCR1/2  ����4�ŵ��
   PWM_X_13   ռ�ձ�>0���Ƕȴ���Ŀ��  ��1��������ƶ�  3��  1 ��
 
*/
int PWM_Y_13(int Nextpoint){   //1��3���  Y��  3����Ƕ�Ϊ��  
   if(Nextpoint<0){    // �Ƕ�С��Ŀ�꣬���������ƶ�  3 �� 1 ��
     
     TIM2->CCR4=0;       //���3 CCR3>0 ����  CCR4>0  ����
     TIM2->CCR3=-Nextpoint;  
       
     TIM3->CCR3=0;    // ���1 CCR3>0  ����   CCR4>0  ����
     TIM3->CCR4=-Nextpoint;       
   }
  else{   
       
     TIM2->CCR4=Nextpoint;     
     TIM2->CCR3=0;  
       
     TIM3->CCR4=0;   
     TIM3->CCR3=Nextpoint;         
    } 
    return 0;   }

int PWM_X_24(int Nextpoint){   // 2��4���  4Ϊ������
  if(Nextpoint<0){      // ��2 �����ƶ�  2 ����  4 ����

     TIM2->CCR1=0;     //���4  CCR2>0 ����   CCR1>0  ����
     TIM2->CCR2=-Nextpoint;      
     TIM3->CCR2=-Nextpoint;  //���2 CCR3>0 ����   CCR4>0  ����
     TIM3->CCR1=0;  
    }else{
     
     TIM2->CCR1=Nextpoint;     //���4  CCR2>0 ����   CCR1>0  ����
     TIM2->CCR2=0;
      
     TIM3->CCR2=0;  //���2 CCR3>0 ����   CCR4>0  ����
     TIM3->CCR1=Nextpoint;    
    }
   return 0;   }


float R = 20.0; 					 //�뾶����(cm)
float angle = 40.0;					 //�ڶ��Ƕ�����(��)
float B=0;

extern float pitch,roll;
extern Pid_struct motor13_parameter,motor24_parameter;

/*------------------------------------------
 ��������:��1��PID����
------------------------------------------*/
void Test_1(void)
{
	const float priod = 1410.0;  //��������(����)
	static uint32_t MoveTimeCnt = 0;
	float set_y = 0.0;
	float A = 0.0;
	float Normalization = 0.0;
	float Omega = 0.0;
				
	MoveTimeCnt += 5;							 //ÿ5ms����1��
	Normalization = (float)MoveTimeCnt / priod;	 //�Ե������ڹ�һ��
	Omega = 2.0*3.14159*Normalization;			 //��2�н��й�һ������
	A = atan((R/102.0f))*57.2958f;				 //���ݰڷ�����Ƕ�A,102Ϊ�ڸ˾�����泤��cm
	set_y = A*sin(Omega);                        //�������ǰ�ڽ� 	
	
	Speed_PIDInit(&motor13_parameter,30,0.2,100);   
	Speed_PIDInit(&motor13_parameter,30,0.2,100);   	

  PWM_Y_13(Speed_PIDAdjust(&motor13_parameter,set_y,roll));  //x����0   y�������set_y��΢���ڶ�����
  PWM_X_24(Speed_PIDAdjust(&motor24_parameter,0,pitch));           
}


/*------------------------------------------
 ��������:��2��PID����
------------------------------------------*/
void Test_2(void)
{
	const float priod = 1905.0;  //��������(����)
	static uint32_t MoveTimeCnt = 0;
	float set_y = 0.0;
	float A = 0.0;
	float Normalization = 0.0;
	float Omega = 0.0;
				
	MoveTimeCnt +=5;							 //ÿ5ms����1��
	Normalization = (float)MoveTimeCnt / priod;	 //�Ե������ڹ�һ��
	Omega = 2.0*3.14159*Normalization;			 //��2�н��й�һ������
  A = atan((R/110.0f))*57.2958f;				 //���ݰڷ�����Ƕ�A��57.3ת����,102�ڸ���ظ߶�
  set_y = A*sin(Omega);               //�������ǰ�ڽ� 	
   
 	Speed_PIDInit(&motor13_parameter,60,0.2,200);   
	Speed_PIDInit(&motor13_parameter,60,0.2,200);   	

  PWM_Y_13(Speed_PIDAdjust(&motor13_parameter,set_y,roll));  //x����0   y�������set_y��΢���ڶ�����
  PWM_X_24(Speed_PIDAdjust(&motor24_parameter,0,pitch));     // ����R   ���ڰڶ����ȴ�С
}


/*------------------------------------------
 ��������:��3��PID����
------------------------------------------*/
void Test_3(void)
{
	const float priod = 1795.0;  //��������(����)
	             //��λ���� 0, 10   20   30   40   50   60   70   80   90   100  110  120  130  140  150  160  170 180
	const float Phase[19]= {0,-0.1,-0.05,0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,0.05,0.05,0.05,0.07,0};
	static uint32_t MoveTimeCnt = 0;
	float set_x = 0.0;
	float set_y = 0.0;
	float Ax = 0.0;
	float Ay = 0.0;
	float A = 0.0;
	uint32_t pOffset = 0;
	float Normalization = 0.0;
	float Omega = 0.0;
	
	pOffset = (uint32_t)(angle/10.0f);			 //��λ���������±�
        
	MoveTimeCnt += 5;							 //ÿ5ms����1��
	Normalization = (float)MoveTimeCnt / priod;	 //�Ե������ڹ�һ��    
	Omega = 2.0*3.14159*Normalization;			 //��2�н��й�һ������
	A = atan((R/102.0f))*57.2958f;              //���ݰڷ�����Ƕ�A��57.3ת����,102�ڸ���ظ߶�
        
	Ax = A*cos(angle*0.017453);	 //�����X����ڷ�����0.017453Ϊ����ת��
	Ay = A*sin(angle*0.017453);	 //�����Y����ڷ�����
	set_x = Ax*sin(Omega); 		 //�����X����ǰ�ڽ�
	set_y = Ay*sin(Omega+Phase[pOffset]); //�����Y����ǰ�ڽ�	 ��һ��Ư��
	
 	Speed_PIDInit(&motor13_parameter,60,0.2,200);   
	Speed_PIDInit(&motor13_parameter,60,0.2,200);   	

  PWM_Y_13(Speed_PIDAdjust(&motor13_parameter,set_y,roll));  //x����0   y�������set_y��΢���ڶ�����
  PWM_X_24(Speed_PIDAdjust(&motor24_parameter,set_x,pitch));     // ����R   ���ڰڶ����ȴ�С 
	
}


/*------------------------------------------
 ��������:��4��PID����
------------------------------------------*/ 
void Test_4(void)
{	
	if(abs(pitch)<45.0 && abs(roll)<45.0)	//С��45�ȲŽ����ƶ�
	{
	
 	Speed_PIDInit(&motor13_parameter,60,0.2,200);   
	Speed_PIDInit(&motor13_parameter,60,0.2,200);   	

  PWM_Y_13(Speed_PIDAdjust(&motor13_parameter,0,roll));  //С��45��ʱ��pid����x_y����Ƕ�Ϊ0  
  PWM_X_24(Speed_PIDAdjust(&motor24_parameter,0,pitch));   
	}
	else	
	{
    // ϵͳֹͣ����
	}
}



/*------------------------------------------
 ��������:��5��PID����
------------------------------------------*/
void Test_5(void)
{
	const float priod = 1410.0;  //��������(����)
	static uint32_t MoveTimeCnt = 0;
	float set_x = 0.0;
	float set_y = 0.0;
	float A = 0.0;
	float phase = 0.0;
	float Normalization = 0.0;
	float Omega = 0.0;
	
	MoveTimeCnt += 5;							 //ÿ5ms����1��
	Normalization = (float)MoveTimeCnt / priod;	 //�Ե������ڹ�һ��
	Omega = 2.0*3.14159*Normalization;			 //��2�н��й�һ������				
	A = atan((R/88.0f))*57.2958f;    //���ݰ뾶�����Ӧ�����A
	
//   Set_Motor_PID(120,0,2);   // ��һ���PID����
 //   Set_PID_2(120,0,2);
}