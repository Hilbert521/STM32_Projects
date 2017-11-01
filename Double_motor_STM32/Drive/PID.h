#ifndef _PID_H
#define _PID_H



typedef struct  
{ 
	int speed_target;     //�趨Ŀ�� 
  int out_duty;  // ���ռ�ձ�
	
	float speed_p; 		//��������
	float speed_i; 		//���ֳ���
	float speed_d;     //΢�ֲ���
	
	float ek_0;		  //��ǰ���
	float ek_1;		  //��ǰ���
	float ek_sum; 	// ������
}Pid_struct;


extern int Speed_PIDAdjust(Pid_struct *motor_pid,int Targe,int Feedback_speed) ;
extern void Speed_PIDInit(Pid_struct *motor_pid);

#endif
