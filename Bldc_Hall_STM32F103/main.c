#include "main.h"

/*********ȫ�ֱ���***********/
u8 Ref_Lcd_flag=0;
u8 state=0;                  // ������״̬
u32 feedback_value=0;		   //�ٶȷ���ֵ
u32 TIMER=0;
u8 frequent_1=0;  //̤Ƶ����
vu16 I_Thrut[4];  //�����ѹ���������   // I_Thrut[3]Ϊ��ѹ
u8 Run_Pause=0;

u8 Speed_aver=0;
u16 Frequent_aver=0;
u16 Mileage_e0;

u8 Frame_flag=0;
u8 Start_flag=0;
u8 Cart_flag=0;

u8 ERROR_flag=0;   //�������ͱ�־λ

Pid_struct Main_pid;

u8 Assist;       //��λ
u8 Mileage;
u8 datatemp[2];
u8 Flash_Buff[2];

u8 V_BUS_Flag;
u8 begin;
u8 Key=0;	
u32 Mileage_Single=0;
 u8 d=0; 
extern u16 Exti_speed_1;
extern u32 frequent_sum;
int main(void)
{
  /***********IOʱ�ӳ�ʼ��*********************/
    Rcc_init();
  /********�ж����ȼ�����********/
    STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,2); 
    Mileage=datatemp[1];
  //  Assist=datatemp[0];  
    Assist=0;     
    Mileage_e0=Mileage;
    NVIC_Config();
    
  /**********��Ļ��ʼ��*****************/  
    delay_init();
    SPI1_Init();	//SPI1��ʼ��
	  LCD_Init();	
    POINT_COLOR=RED ;//��������Ϊ��ɫ    
    LCD_ShowString(80,160,200,16,24,"WELCOME");  //д����λ��		
    FRAME_INIT();
    
  /********BLDC���ó�ʼ��**********/
     BLDC_GPIOConfig();
     BLDC_TIM1Config();
     BLDC_TIM3Config();
     IR2136_EN_SET;   // ʹ�� PB12  ���ʹ��
    
  /********����/AD/��ʱ��/�ⲿ�жϳ�ʼ��*************************/    
	   USART1_GPIOConfig();
     USART1_Config();
     KEY_Init();  
     
    Adc_Init();
    DMA_Configuration();
    
    TIM4_Init(9999,71);     //  10ms ��������ʱ 
    TIM2_Init(4999,7199);   // 0.5s ��ʱ�ж�  ����
   
    EXTI1_Init();      // Pb9  ̤Ƶ�ж�   J6
    EXTI0_Init();     // Pb0  ���������ж�  
    EXTI10_Init();	 //PC10       ɲ���ж�   J4
		
    Speed_PIDInit(&Main_pid);    //PID��ʼ��  Ϊ������һ���̶��ٶ�
		
    state=STOP;   
    delay_ms(1000);
    
 while(1){
             
     USART_Reci();   
     KEY_Scan();  
  
    if( Assist==0){     // ������ģʽ  ���������
        state=MANPOWER;    
    } 
 
    switch(state)
    {
        case MANPOWER:   // ������ģʽ
              BLDC_Stop();  // �رյ����ֹ�����
              while(1){
                 KEY_Scan();         //����������ģʽ
              if(state==CART){     //��Ӱ�������������ж�����  �Ǳ���������
                    state=CART;     
                    break;     // ������ѭ��                    
               }  
                if(Assist!=0){     //��Ӱ�������������ж�����  �Ǳ���������
                    state=START;     
                    break;     // ������ѭ��                    
                 } 
                 Ref_LCD(); 
              }
              
        case CART:   // һ���Ƴ�ģʽ  
                              
            while(state==CART){        
              
                   TIM1->CCR1=450;
                   TIM1->CCR2=450;
                   TIM1->CCR3=450;
                   BLDC_Start1();
                   USART_Reci();   
              
                KEY_Scan();      //KEY�޸ı�־λ 
                Ref_LCD();         
                if(state!=CART){
                    d=0;
                state=START;                    
                BLDC_Stop();
                break;    
              }
            }
                  
        case START:    //�� ִ����������  ��ת������������״̬   
        //     Run_Pause=1;
        if(frequent_1!=0){
          //    Run_Pause=0;
              Start_flag=1;
              Speed_PIDInit(&Main_pid); 
              BLDC_Start1();             
              state=RUN;  
          }    
            break;
        
        case RUN:    // ����״̬  �߼�������/����������           
             Adjust_Speed();   
             break;
               
        case STOP: 
            BLDC_Stop(); 
            break;
        
        case FAULT:                 
           while(1){
              char f1[5];
              sprintf(f1,"%d",ERROR_flag); 
              LCD_Clear(WHITE);
              BLDC_Stop();   // ������Ϣ����  �رյ��  ��ֹ����� 
              POINT_COLOR=RED ;//��������Ϊ��ɫ    
              LCD_ShowString(80,160,200,16,24,"ERROR");  //д����λ��           
              LCD_ShowString(160,160,200,16,24,f1);  //д����λ��  
              Key=KEY_Scan1(0);		//�õ���ֵ
              if(Key==KEY0_l)
              {
                  NVIC_SystemReset();
              }
           } 
            break;              
    }  
   Ref_LCD();     
 }
}
 
 
void Adjust_Speed(void)
{    u8 sw=0;
   if(frequent_1==0){
       state=START;
       BLDC_Stop();
    }
    else if(frequent_1<=1){
      Speed_PIDAdjust(&Main_pid,2000+Assist*1000);
    }
    else if(frequent_1<=3){
      Speed_PIDAdjust(&Main_pid,2500+Assist*1000);
     } 
    else if(frequent_1<=5){
      Speed_PIDAdjust(&Main_pid,3500+Assist*1000);
     }
    else if(frequent_1<=7){
      Speed_PIDAdjust(&Main_pid,4500+Assist*1000);
     }
 
    if((float)(720*1.0/Exti_speed_1)>25){
      BLDC_Stop();
      sw=1;
    } 
    if(sw==1&&(float)(720*1.0/Exti_speed_1)<20){
      BLDC_Start1();
      sw=0;
    }  
}
  
void KEY_Scan(void)
{
      Key=KEY_Scan1(0);		//�õ���ֵ
		switch(Key)
		{				 
	   case KEY0_s:      
         if(Frame_flag==0){   //Frame_flag=0 ������ʾ����
            FRAME_INIT1();
            Mileage_Single=Mileage_e0-Mileage;
            Speed_aver=(float)((Mileage_Single*1.0/(TIMER*1.0))*6); //��λת�� 
            Frame_flag=1;         
            Frequent_aver=frequent_sum/TIMER; 
            mes_show1((float)Mileage_e0*0.1,(float)Mileage_Single*0.1,TIMER,(float)Speed_aver,frequent_sum/8,Frequent_aver/8);                        
          } 
         else{   //Frame_flag=0 �ڶ�ҳ��
              BLDC_Stop();
              Flash_Buff[0]=Assist;
              Flash_Buff[1]=Mileage;    
              STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Flash_Buff,2);
              POINT_COLOR=RED ;
              LCD_Clear(WHITE);   
              LCD_ShowString(40,160,200,16,24,"WRITE SUCCESS");  //д����λ��
             
            FRAME_INIT();
            Frame_flag=0; 
          }     
			     break;
          
	    case KEY0_l:
                NVIC_SystemReset();     
				break;
            
        case KEY1_s:           //�¼�
                if(Assist!=0){ 
                      Assist--;
                   if(Assist<1){
                           Assist=0;
                      }    
                  } 
				break;            
        case KEY1_l:
               Assist=0;
        
//               BLDC_Stop();
//               Flash_Buff[0]=Assist;
//               Flash_Buff[1]=Mileage;    
//               STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)Flash_Buff,2);
//               POINT_COLOR=RED ;
//               LCD_Clear(WHITE);   
//               LCD_ShowString(40,160,200,16,24,"WRITE SUCCESS");  //д����λ��
//               delay_ms(1000);
//               LCD_Clear(WHITE); 
//               FRAME_INIT();
			   break;
            
      	 case KEY2_s:   // �ϼ�
             if(state==CART){
                BLDC_Stop();
                state=START;
              }
              Assist++;
              if(Assist>4){
              Assist=5;
                   }            
                 break;
                   
       	 case KEY2_l:     //�����ϼ�  �����Ƴ�ģʽ
                 state=CART; 	
                break;
            
			default:  break;
		} 
}
   
/*****************��ª�棬��Ҫ����д��������*********************/
void USART_Reci(void)
 {
     if(USART1_RX_STA)			//���յ�һ��������
   {
      if(USART1_RX_BUF[0]=='s')
        {  
        Mileage_Single=Mileage-Mileage_e0;
        Speed_aver=(float)(Mileage_Single/TIMER)*6; //��λת�� 
        Frequent_aver=frequent_sum/TIMER; 
        uprintf(USART1,"s%.1fs\n",Speed_aver);      
        }	
      else if(USART1_RX_BUF[0]=='m')
        {  
        //  uprintf(USART1,"me%.1fme\n",Mileage_Single*0.1);  
        uprintf(USART1,"m%.1fm\n",Mileage*0.1);       				
        }	
      else if(USART1_RX_BUF[0]=='f')
        {  
        uprintf(USART1,"f%d\nf",frequent_sum/8); 
      //  uprintf(USART1,"f1%d\nf1",Frequent_aver/8);      				
        }	
        else if(USART1_RX_BUF[0]=='a')
        {  
       Assist++;
      //  uprintf(USART1,"f1%d\nf1",Frequent_aver/8);      				
        } else if(USART1_RX_BUF[0]=='b')
        {  
       Assist--;
        uprintf(USART1,"f1%d\n",state);      				
        }
        
        
      USART1_RX_STA=0;	
   }	
}	
 

/***********************************************
/////�ж����ȼ�     ɲ��EXTI5-9  00    ���ܴ���  04    ���� 02    ̤Ƶ 03
***********************************************/
void NVIC_Config(void)
{
	 NVIC_InitTypeDef NVIC_InitStructure; 					 //��λNVIC�Ĵ���ΪĬ��ֵ	
     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);           //�����ж����ȼ���

     NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;	  // ɲ��
  	 NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;					
  	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
     NVIC_Init(&NVIC_InitStructure); 
	
      NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;		   //�����ж�
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;    
      NVIC_InitStructure.NVIC_IRQChannelSubPriority =1;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
	
     NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;   // 10ms �ж�
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	 NVIC_Init(&NVIC_InitStructure);	
      
//     NVIC_InitStructure.NVIC_IRQChannel =  ADC1_2_IRQn;
//	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
//	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
//	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	 NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���

     NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			 // ̤Ƶ�ж�
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;	 
  	 NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;					
  	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
     NVIC_Init(&NVIC_InitStructure);   
      
     NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;			// ���������ж�
     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1;	 
  	 NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;					
  	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		
     NVIC_Init(&NVIC_InitStructure); 
          
      NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =1; 
      NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);
      
//      NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_IRQn;  //��TIM1�����ж�
//      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//      NVIC_Init(&NVIC_InitStructure);	
    
	   NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;    // 30s ��ʱ�ж�
	   NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;
	   NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		
	   NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			
	   NVIC_Init(&NVIC_InitStructure);		
   
}


/**************************************************
//// SPI�Ǳ���ʾ����
***************************************************/
 
//�ֲ�ˢ��Ļ   Res_Lcd��TIM2 �������ÿ2s �ֲ�ˢ��һ��
void Ref_LCD(void)
{    
   if(Ref_Lcd_flag>50)
    {   
     if(Frame_flag==0){
            if(Exti_speed_1==0){
               mes_show(TIMER,(float)Mileage_e0*0.1,Assist,0,frequent_1*8);
             }else{
               mes_show(TIMER,(float)Mileage_e0*0.1,Assist,(float)(720*1.0/Exti_speed_1),frequent_1*8);    
             }
        }        
        Ref_Lcd_flag=0;
     }
} 


void mes_show(u16 a,float b,u8 c,float d,u8 e)
{       u8 h=0;
        char a1[5];
        char b1[8];
        char c1[5];
        char d1[5];
        char e1[5];
        char f1[5];
        
         h=a/60;
        a=a-h*60;
       sprintf(a1,"%.1d",a); 
       sprintf(b1,"%.1f",b);
       sprintf(c1,"%d",c);
       sprintf(d1,"%.1f",d);
       sprintf(e1,"%d",e);
       sprintf(f1,"%d",h);    
    if(a<=9){
       LCD_ShowString(65,222,200,16,24,"0");	//TIME
       LCD_ShowString(77,222,200,16,24,a1);	//TIME
       }
     else if(a>9&&a<=59){
       LCD_ShowString(65,222,200,16,24,a1);	//TIME
     }    
     
    if(h<=9){
     LCD_ShowString(32,222,200,16,24,"0");	//TIME
     LCD_ShowString(44,222,200,16,24,f1);	//TIME
    }else if(h>9&&h<=59){
       LCD_ShowString(32,222,200,16,24,f1);	//TIME
     } 
       
     LCD_ShowString(25,287,200,16,24,b1); //MILE
	 LCD_ShowString(20,90,200,16,96,c1);	//ASSIST
	 LCD_ShowString(110,100,200,16,48,d1); //SPEED
     LCD_ShowString(140,230,200,16,48,e1); //FRE
	
//     if(I_Thrut[3]<=160){
//     
//     }else if(I_Thrut[3]>=195){
//       LCD_Fill(21,19,26,27,BLACK);    //���ͼ�� 
//     }else if(I_Thrut[3]<=180){
//      LCD_Fill(21,19,26,27,BLACK);    //���ͼ��  
//      LCD_Fill(28,19,33,27,BLACK);    //���ͼ��
//     }
          
//     LCD_Fill(21,19,26,27,BLACK);    //���ͼ��
//     LCD_Fill(28,19,33,27,BLACK);    //���ͼ��
//     LCD_Fill(35,19,40,27,BLACK);    //���ͼ��
//     LCD_Fill(42,19,47,27,BLACK);    //���ͼ��    
}
  

void mes_show1(float a,float b,u8 c,float d,u16 e,u16 f)
{       u8 h=0;
        char a1[5];
        char b1[8];
        char c1[5];
        char d1[5];
        char e1[5];
        char f1[5];
        char g1[5];
        h=c/60;
        c=c-h*60;
       sprintf(a1,"%.1f",a); 
       sprintf(b1,"%.1f",b);
       sprintf(c1,"%d",c);
       sprintf(d1,"%.1f",d);
       sprintf(e1,"%d",e);
       sprintf(f1,"%d",f);
       sprintf(g1,"%d",h);
     if(c<=9){
       LCD_ShowString(137,128,200,16,24,"0");	//TIME
       LCD_ShowString(149,128,200,16,24,c1);	//TIME
       }
     else if(a>9&&a<=59){
       LCD_ShowString(137,128,200,16,24,c1);	//TIME
     }  
    
    if(h<=9){
     LCD_ShowString(105,128,200,16,24,"0");	//TIME
     LCD_ShowString(117,128,200,16,24,g1);	//TIME
    }else if(h>9&&h<=59){
       LCD_ShowString(105,128,200,16,24,g1);	//TIME
     } 
      
     LCD_ShowString(130,68,200,16,24,a1);    //Mile_Sum
     LCD_ShowString(130,98,200,16,24,b1);   //Mile_sin
   //  LCD_ShowString(120,128,200,16,24,c1);  //Timer
     LCD_ShowString(120,160,200,16,24,d1);  //Speed
     LCD_ShowString(130,188,200,16,24,e1);  //Fre_Sum
     LCD_ShowString(130,218,200,16,24,f1);  //Fre_Aver
}  
  

void FRAME_INIT(void){
    LCD_Clear(WHITE);
    POINT_COLOR=BLACK ; 
    LCD_DrawLine(2,2,2,318);      //���ұ߽�   
    LCD_DrawLine(238,2,238,318);      
    LCD_DrawLine(2,2,238,2);   // ���±߽�
    LCD_DrawLine(2,318,238,318); 
	
   // LCD_DrawLine(0,70,238,70); 
	LCD_DrawLine(10,40,230,40);  //��һ
    //LCD_DrawLine(80,220,238,220);
	LCD_DrawLine(10,190,230,190); // ���
    //LCD_DrawLine(80,70,80,318);   
	LCD_DrawLine(87,45,87,185);   // ��һ  
	LCD_DrawLine(120,200,120,310);   // ����
	LCD_DrawLine(10,255,118,255); // ����
	
	LCD_DrawLine(17,16,52,16);
	LCD_DrawLine(17,16,17,31);
	LCD_DrawLine(17,31,52,31);
	LCD_DrawLine(52,16,52,21);
	LCD_DrawLine(52,21,55,21);
	LCD_DrawLine(55,21,55,26);
	LCD_DrawLine(52,26,55,26);
	LCD_DrawLine(52,26,52,31);

	LCD_ShowString(10,50,200,24,24,"ASSIST");
	
	LCD_ShowString(100,50,200,24,24,"SPEED");
	
	LCD_ShowString(180,155,200,24,24,"KM/h");
	
	LCD_ShowString(11,200,200,16,16,"TIME");
    
	LCD_ShowString(55,221,200,16,24,":");
    
	LCD_ShowString(11,265,200,16,16,"MILE");
	LCD_ShowString(85,295,200,16,16,"KM");
	
	LCD_ShowString(135,200,200,16,24,"FRE");
	LCD_ShowString(185,275,200,16,24,"RPM");
}


void FRAME_INIT1(void){

    LCD_Clear(WHITE);
    
    POINT_COLOR=BLACK ; 
    LCD_DrawLine(2,2,2,318);      //���ұ߽�   
    LCD_DrawLine(238,2,238,318);      
    LCD_DrawLine(2,2,238,2);   // ���±߽�
    LCD_DrawLine(2,318,238,318); 
    
     LCD_DrawLine(20,65,220,65);  // ��һ��
     LCD_DrawLine(20,95,220,95); 
     LCD_DrawLine(20,125,220,125);  
     LCD_DrawLine(20,155,220,155);  
     LCD_DrawLine(20,185,220,185);  
     LCD_DrawLine(20,215,220,215);  
     LCD_DrawLine(20,245,220,245); 
     LCD_DrawLine(20,275,220,275); 
    
     LCD_DrawLine(20,65,20,245);  // ���� 
     LCD_DrawLine(220,65,220,245);     
     
    LCD_ShowString(25,68,200,16,24,"Mile_Sum");  
    LCD_ShowString(25,98,200,16,24,"Mile_Sin");  
    LCD_ShowString(25,128,200,16,24,"Timer");
    LCD_ShowString(127,128,200,16,24,":");	//TIME    
    LCD_ShowString(25,158,200,16,24,"Speed"); 
    LCD_ShowString(25,188,200,16,24,"Fre_Sum");
    LCD_ShowString(25,218,200,16,24,"Fre_Aver");
    
    LCD_ShowString(200,72,200,16,16,"Km");  
    LCD_ShowString(200,102,200,16,16,"Km");  
    LCD_ShowString(190,132,200,16,16,"Min");  
    LCD_ShowString(180,162,200,16,16,"Km/h"); 
    LCD_ShowString(190,192,200,16,16,"RPM");
    LCD_ShowString(190,222,200,16,16,"RPM"); 
}


