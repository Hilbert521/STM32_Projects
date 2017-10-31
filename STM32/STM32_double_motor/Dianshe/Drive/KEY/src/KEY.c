#include "main.h"

void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTCʱ��
 
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13;//PA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //PA0���ó����룬Ĭ������	  
	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOA.0
} 

u16 KEY_Time=0;
vu16 KEY_UP[3]={1,1,1};

u8 KEY_Scan1(u8 mode)
{	 
	//static u8 key_up=1;   //�������ɿ���־
	//if(mode)key_up=1;  //֧������		  
	if( KEY_UP[0]&& KEY_UP[1]&& KEY_UP[2]&&(KEY0==0||KEY1==0||KEY2==0))
	{
		delay_us(500);//ȥ���� 
		if(KEY0==0)  
        {   KEY_UP[0]=0;   
        }
		else if(KEY1==0)
        {   KEY_UP[1]=0;       
        }   
        else if(KEY2==0)
        {   KEY_UP[2]=0;       
        }
	}
    else if(KEY0==1&&KEY1==1&&KEY2==1)   //KEY0_s
       {
     if( KEY_UP[0]==0){
           if((KEY_Time<=100)&&(KEY_Time>=0))				
             {
                 KEY_UP[0]=1; 
                 KEY_Time=0; 
                 return KEY0_s;      
             }
          else if (KEY_Time>100)	
            {	
               KEY_UP[0]=1;  
               KEY_Time=0;
               return KEY0_l;
           } 
       }
               
       if( KEY_UP[1]==0){
           if((KEY_Time<=100)&&(KEY_Time>=0))				
             {
                 KEY_UP[1]=1; 
                 KEY_Time=0; 
                 return KEY1_s;      
             }
          else if (KEY_Time>100)	
            {	
               KEY_UP[1]=1;  
               KEY_Time=0;
               return KEY1_l;
           }  
       }
     
     if( KEY_UP[2]==0){
       if((KEY_Time<=100)&&(KEY_Time>=0))				
         {
              KEY_UP[2]=1;
             KEY_Time=0; 
             return KEY2_s;      
         }
      else if (KEY_Time>100)	
        {	  
            KEY_UP[2]=1;
            KEY_Time=0;
           return KEY2_l;
       }   
     }
        KEY_UP[0]=1;
        KEY_UP[1]=1;
        KEY_UP[2]=1;     
   }
       
	return 0;// �ް�������
}

