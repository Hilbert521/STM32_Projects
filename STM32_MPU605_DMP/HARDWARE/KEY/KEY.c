#include "stm32f10x.h"
#include "delay.h"
#include "key.h"
#include "USART2.h"
/*******************************************************
*******�������������Ӧ��ż�����
*******************************************************/
void KEY_Init(void) //��ʼ���������Ҫʹ�õ�GPIO�ڡ�
{

 	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       //����PB8��PB11Ϊ�������롢����
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;       //����PB12��PB15Ϊ�������롣
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_Init(GPIOB,&GPIO_InitStructure);		
}

extern u8 RUN_MODE;

int KEY_Scan(void) //ʵ�־�����̡�����ֵΪ���������ļ�ֵ���˼�ֵ���û��Լ����塣
{
	u8 KeyVal=0;	 //keyValΪ��󷵻صļ�ֵ��
    
	GPIO_Write(GPIOB,(GPIOB->ODR & 0xf0ff | 0x0f00)); //����PB8��PB11ȫ������ߡ�
	
	// ����
	if((GPIOB->IDR & 0xf000)==0x0000)  //���PB12��PB15ȫΪ0����û�м����¡���ʱ������ֵΪ-1.
    {   return -1;    }
	else
	{	
	    delay_ms(5);    //��ʱ5msȥ������
	  if((GPIOB->IDR & 0xf000)==0x0000)//�����ʱ5ms��PB12��PB15��ȫΪ0���򣬸ղ����ŵĵ�λ�仯�Ƕ���������.
        return -1;   }
    
  
  GPIO_Write(GPIOB,(GPIOB->ODR & 0xf0ff | 0x0100));	//��PB11��PB8��������Ƶ�0001.

		switch(GPIOB->IDR & 0xf000)//��PB12��PB15��ֵ�����жϣ��������ͬ�ļ�ֵ��
			{
				case 0x1000: KeyVal=1; RUN_MODE=1;   uprintf(USART1," ����Ϊ��һ��\r\n");  break;
				case 0x2000: KeyVal=2; RUN_MODE=2;   uprintf(USART1," ����Ϊ�ڶ���!\r\n");  break;
				case 0x4000: KeyVal=3; RUN_MODE=3;   uprintf(USART1," ����Ϊ������!\r\n");	break;
				case 0x8000: KeyVal=10;	break;
			}
	   
	GPIO_Write(GPIOB,(GPIOB->ODR & 0xf0ff | 0x0200));	//��PB11��PB8��������Ƶ�0010.
		switch(GPIOB->IDR & 0xf000)		        //��PB12��PB15��ֵ�����жϣ��������ͬ�ļ�ֵ��
		{
			case 0x1000: KeyVal=4; RUN_MODE=4;  uprintf(USART1," ����Ϊ������!\r\n");	break;
			case 0x2000: KeyVal=5; RUN_MODE=5;  uprintf(USART1," ����Ϊ������!\r\n");	break;
			case 0x4000: KeyVal=6; RUN_MODE=6;  uprintf(USART1," ����Ϊ������!\r\n"); break;
			case 0x8000: KeyVal=11;	break;
		}

	GPIO_Write(GPIOB,(GPIOB->ODR & 0xf0ff | 0x0400));	//��PB11��PB8��������Ƶ�0100.
		switch(GPIOB->IDR & 0xf000)		        //��PB12��PB15��ֵ�����жϣ��������ͬ�ļ�ֵ��
		{
			case 0x1000: KeyVal=7;	break;
			case 0x2000: KeyVal=8;	break;
			case 0x4000: KeyVal=9;	break;
			case 0x8000: KeyVal=12;	break;
		}
 
	 GPIO_Write(GPIOB,(GPIOB->ODR & 0xf0ff | 0x0800));	//��PB11��PB8��������Ƶ�1000.
		switch(GPIOB->IDR & 0xf000)		        //��PB12��PB15��ֵ�����жϣ��������ͬ�ļ�ֵ��
		{
			case 0x1000: KeyVal=14;	break;
			case 0x2000: KeyVal=0;	break;
			case 0x4000: KeyVal=15;	break;
			case 0x8000: KeyVal=13;	break;
		}			
	return KeyVal;		
}

