
#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_it.h"
#include "delay.h"
#include "BLDC.h"
#include "main.h"


/*********ȫ�ֱ���***********/
FlagStatus Direction = SET;
FlagStatus zheng_fan = RESET;
extern u8 duzhuan_value;	

void BLDC_GPIOConfig(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
 	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;  //TIM1���
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
			 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;//TIM1���
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;						   
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

//    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;              //TIM3�Ļ�������
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//    GPIO_Init(GPIOA, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                         //TIM3�Ļ�������
//    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//    GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);    //��ӳ�䵽 PB6/7/8
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;              //TIM3�Ļ�������
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;                         //TIM3�Ļ���
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOC, &GPIO_InitStructure);  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;                         //PB12  
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);  
    
	   
}
/**********************************************************************
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
**********************************************************************/
void BLDC_TIM1Config(void)
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;			   //�����ṹ���������
   TIM_OCInitTypeDef  		TIM_OCInitStructure;               //����ṹ���������
   TIM_BDTRInitTypeDef  	TIM_BDTRInitStructure;			   //����ɲ���ṹ���������
   
   TIM_DeInit(TIM1);   //ȱʡֵ

   TIM_TimeBaseStructure.TIM_Prescaler = 2;					   //TIM������ʼ��
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_CenterAligned2;//����������ģʽ,����Ƚϱ�־λֻ���ڱȽ������ϼ��㱻����
   TIM_TimeBaseStructure.TIM_Period = 2500 - 1;					   //PWM 16K
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;				   

   TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);   //��ʼ��TIM1 ʱ�����

   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 		   //TIM���ͨ����ʼ��
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable; 
   TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;                  
   TIM_OCInitStructure.TIM_Pulse =0; 
    
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
   TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;         
   TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;//TIM_OCIdleState_Set;
   TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;          
  
   TIM_OC1Init(TIM1,&TIM_OCInitStructure); 

   TIM_OCInitStructure.TIM_Pulse =0;        //��ʼ��TIM1 ��ͨ��2
   TIM_OC2Init(TIM1,&TIM_OCInitStructure);

   TIM_OCInitStructure.TIM_Pulse =0;
   TIM_OC3Init(TIM1,&TIM_OCInitStructure);

   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 		   //TIM���ͨ��4��ʼ������������ADע�����
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Disable;                   
   TIM_OCInitStructure.TIM_Pulse =1500; 
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;           
  
   TIM_OC4Init(TIM1,&TIM_OCInitStructure); 
 
   TIM_SelectOutputTrigger(TIM1,TIM_TRGOSource_OC4Ref);
 
   TIM_BDTRInitStructure.TIM_OSSRState = TIM_OSSRState_Enable;	//����ɲ����ʼ��
   TIM_BDTRInitStructure.TIM_OSSIState = TIM_OSSIState_Enable;
   TIM_BDTRInitStructure.TIM_LOCKLevel = TIM_LOCKLevel_1; 
   TIM_BDTRInitStructure.TIM_DeadTime = 100;
   TIM_BDTRInitStructure.TIM_Break = TIM_Break_Disable;	 //��򿪣������������״̬���ң�������
   TIM_BDTRInitStructure.TIM_BreakPolarity =  TIM_BreakPolarity_Low ;
   TIM_BDTRInitStructure.TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;

   TIM_BDTRConfig(TIM1,&TIM_BDTRInitStructure);

   TIM_OC1PreloadConfig(TIM1,TIM_OCPreload_Enable);  //ʹ�ܲ���ȽϼĴ���Ԥװ�أ�ͨ��1��

   TIM_OC2PreloadConfig(TIM1,TIM_OCPreload_Enable);	 //ʹ�ܲ���ȽϼĴ���Ԥװ�أ�ͨ��2��

   TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);	 //ʹ�ܲ���ȽϼĴ���Ԥװ�أ�ͨ��3��
   
   TIM_SelectInputTrigger(TIM1, TIM_TS_ITR2);        //���봥��Դѡ��TIM3   
    
 //   TIM_CCPreloadControl(TIM1,ENABLE);   // CCPC1Ԥװ��ʹ��
   // TIM_SelectCOM(TIM1,ENABLE);
      
    // TIM_ITConfig(TIM1, TIM_IT_CC4 ,ENABLE);    //����AD����
    
     TIM_SelectInputTrigger(TIM1, TIM_TS_ITR2);        //���봥��Դѡ��TIM3   
  
   //TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Trigger);//��ģʽѡ�� ����	  

     TIM_CtrlPWMOutputs(TIM1,ENABLE);

//   TIM_ClearITPendingBit(TIM1, TIM_IT_Break|TIM_IT_COM);
//   TIM_ITConfig(TIM1, TIM_IT_Break | TIM_IT_COM ,ENABLE);
   TIM_ClearITPendingBit( TIM1, TIM_IT_COM);
   TIM_ITConfig(TIM1, TIM_IT_COM ,ENABLE);
   TIM_Cmd(TIM1,ENABLE);  
}

/**********************************************************************
* Description    : None
* Input          : None
* Output         : None
* Return         : None
* Attention      : None
**********************************************************************/

void BLDC_TIM3Config(void)
{
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;			   //�����ṹ���������
   TIM_ICInitTypeDef  TIM_ICInitStructure;                      //����ṹ�����
   TIM_OCInitTypeDef  TIM_OCInitStructure;                     //����ṹ���������
      
   TIM_DeInit(TIM3);

   TIM_TimeBaseStructure.TIM_Prescaler = 71;				   //TIM������ʼ��
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
   TIM_TimeBaseStructure.TIM_Period =65535;     //ֻ��ͨ�������źű仯����
   TIM_TimeBaseStructure.TIM_ClockDivision = 0;
   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

   TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);     
   
   TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;            //ѡ��ͨ��1
   TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; //���������ز���  
   TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_TRC;  //����ͨ��Ϊ���룬��ӳ�䵽����
   TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;       //���벶��Ԥ��Ƶֵ
   TIM_ICInitStructure.TIM_ICFilter = 10;                      //�����˲�����������

   TIM_ICInit(TIM3, &TIM_ICInitStructure);                     //����ͨ������

   TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 		    //TIM���ͨ����ʼ��
   TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;             
   TIM_OCInitStructure.TIM_Pulse =100; 
   TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;      
  
   TIM_OC2Init(TIM3,&TIM_OCInitStructure);

   TIM_SelectHallSensor(TIM3,ENABLE);      // //TIMx CH1/2/3 ͨ��������TRGO
   
   TIM_SelectInputTrigger(TIM3, TIM_TS_TI1F_ED);               //���봥��Դѡ�� TI1F_ED    
  
   TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);             //��ģʽ ��λ 

   TIM_SelectMasterSlaveMode(TIM3, TIM_MasterSlaveMode_Enable);//����ģʽѡ��        
   
   TIM_SelectOutputTrigger(TIM3, TIM_TRGOSource_OC2Ref); //ѡ���������ģʽ(TRGO��)��ͨ��OC2����com�¼�
   
}

void huanxiang(void)
{
  u8 irq_zhuanzi; 

  irq_zhuanzi =(u8)((GPIOC->IDR&0x000000c0)>>6);	//��λ�� 1100 0000 C6/7  0011
  irq_zhuanzi|=(u8)((GPIOC->IDR&0x00000100)>>6);   // 1 0000 0000 C8  0100
  irq_zhuanzi &= 0x07;   // ����������  0000  0111
	
//   if(zheng_fan==RESET)
//  
//  {									 //��ת
  switch(irq_zhuanzi)				 //����ת��λ�ã�����CCER�����λ��ת����ƫ����
    {
	  case 0x06:{	                      												
			  	  TIM1->CCER=0x3081;  //1,4  ba   
			    };break;
	  case 0x02:{
				  TIM1->CCER=0x3180;	  //4,5	   bc     	   
	            };break;							
  	case 0x03:{
				  TIM1->CCER=0x3108;	//2,5    ac
			    };break;
	  case 0x01:{
				  TIM1->CCER=0x3018;	//2,3	    ab      		
	            };break;
	  case 0x05:{
				  TIM1->CCER=0x3810;	//3,6	    cb      		
	            };break;
	  case 0x04:{
	             			
				  TIM1->CCER=0x3801;    //1,6	  ca        		
	            };break;
				default:break;
}	 
//}
// else
//  {									 //��ת
//	switch(irq_zhuanzi)				 //����ת��λ�ã�����CCER�����λ��ת����ƫ����
//    {
//    //*
//	   case 0x02:{										
//			    TIM1->CCER=0x3810;//3,6    cb
//			    };break;
//	  case 0x06:{
//				  TIM1->CCER=0x3018;//2,3		 ab         	   
//	            };break;							
// 	  case 0x04:{
//				  TIM1->CCER=0x3108;//2,5    ac
//			    };break;
//	  case 0x05:{
//				  TIM1->CCER=0x3180;//4,5		  bc        		
//	            };break;
//	  case 0x01:{
//				  TIM1->CCER=0x3081;//1,4		  ba        		
//	            };break;
//	  case 0x03:{ 	              			
//				  TIM1->CCER=0x3801;//1,6		  ca        		
//	            };break;
//	  default:break;	  	
//    }
// }
 
}
/**************����******************/
void BLDC_Start1(void)
{
    BLDC_Start();
    delay_ms(30);
    BLDC_Stop();
    delay_ms(20);
    BLDC_Start();
}

void BLDC_Start(void)
{
   TIM_ITConfig(TIM3, TIM_IT_Trigger, ENABLE);      //����ʱ���ж�  
   TIM1->SMCR|=0x0006;        //��TIM1�����봥��	
   TIM1->DIER=0x0040;         //��TIM1�Ĵ����ж�
   huanxiang();			      //���û�����������	
   TIM3->CR1|=0x0001;		  //��TIM3
   TIM3->DIER|=0x0050;		  //��TIM3�ж�  
}
//extern u8 Assist;
//extern u8 Mileage;
//extern u8 datatemp[2];
void BLDC_Stop(void)
{
//   begin=0;  
   TIM1->SMCR&=0xfff8;		  //�ر�TIM1�����봥��
   TIM1->CCER=0;              //�ر�TIM1����·���
   TIM1->CCER=0x0ccc;         //����·�¹ܣ������ܺ��ƶ�
   TIM1->CCER=0;              //�ر�TIM1����·�������ɲ��		  
   TIM3->CR1&=0xfffe;         //�ر�TIM3						  
   TIM3->CNT=0;               //��TIM3�ļ�����				   
   TIM3->DIER&=0xffaf;		  //��TIM3�ж�    
//   Flash_Buff[0]=Assist;
//   Flash_Buff[1]=Mileage;    
//   STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Flash_Buff,2); 
    
}


