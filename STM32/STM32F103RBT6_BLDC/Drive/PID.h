#ifndef _PID_H
#define _PID_H


#define Out_Speed_Max  2200	   //2500ռ�ձ�
#define Out_Speed_Min   0

#define MAX_Speed_Targe  11000
#define MIN_Speed_Targe  0

typedef struct  
{ 
	int speed_target;     //�趨Ŀ�� 
  int out_duty;  // ���ռ�ձ�
	
	float speed_p; 		//��������
	float speed_i; 		//���ֳ���
	float speed_d;     //΢�ֲ���
	
	float ek_0;		  //��ǰ���
	float ek_sum; 	// ������
}Pid_struct;


extern void Speed_PIDInit(Pid_struct *motor_pid);
extern int Speed_PIDAdjust(Pid_struct *motor_pid,int Targe);

#endif
