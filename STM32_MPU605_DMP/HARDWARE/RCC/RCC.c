#include "rcc.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_flash.h"


/******
*AHB    1��Ƶ   72M    ���72M
*APB1   2��Ƶ   36M    ���36M
*APB2   1��Ƶ   72M    ���72M
*/

void RCC_Configuration(void)
{ 
    ErrorStatus HSEStartUpStatus;
  /*������RCC�Ĵ�������Ϊȱʡֵ*/
  RCC_DeInit();
 
  /*�����ⲿ���پ���HSE��*/
  RCC_HSEConfig(RCC_HSE_ON);   //RCC_HSE_ON����HSE�����(ON)
 
  /*�ȴ�HSE����*/
  HSEStartUpStatus = RCC_WaitForHSEStartUp();
 
  if(HSEStartUpStatus == SUCCESS)        //SUCCESS��HSE�����ȶ��Ҿ���
  {
    /*����AHBʱ�ӣ�HCLK��*/ 
    RCC_HCLKConfig(RCC_SYSCLK_Div1);  //RCC_SYSCLK_Div1����AHBʱ��= ϵͳʱ��
 
    /* ���ø���AHBʱ�ӣ�PCLK2��*/ 
    RCC_PCLK2Config(RCC_HCLK_Div1);   //RCC_HCLK_Div1����APB2ʱ��= HCLK
 
    /*���õ���AHBʱ�ӣ�PCLK1��*/    
RCC_PCLK1Config(RCC_HCLK_Div2);   //RCC_HCLK_Div2����APB1ʱ��= HCLK / 2
 
    /*����FLASH�洢����ʱʱ��������*/
    FLASH_SetLatency(FLASH_Latency_2);    //FLASH_Latency_2  2��ʱ����
   
 /*ѡ��FLASHԤȡָ�����ģʽ*/  
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);       // Ԥȡָ����ʹ��
 
    /*����PLLʱ��Դ����Ƶϵ��*/ 
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);     
// PLL������ʱ��= HSEʱ��Ƶ�ʣ�RCC_PLLMul_9����PLL����ʱ��x 9
   
  /*ʹ��PLL */
    RCC_PLLCmd(ENABLE); 
 
    /*���ָ����RCC��־λ(PLL׼���ñ�־)�������*/   
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)      
       {
       }
 
    /*����ϵͳʱ�ӣ�SYSCLK��*/ 
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK); 
//RCC_SYSCLKSource_PLLCLK����ѡ��PLL��Ϊϵͳʱ��
 
    /* PLL��������ϵͳʱ�ӵ�ʱ��Դ*/
    while(RCC_GetSYSCLKSource() != 0x08)        //0x08��PLL��Ϊϵͳʱ��
       { 
       }
     
   }
}
void Rcc_init(){
    
    SystemInit();  
	RCC_Configuration();
    
	RCC_APB2PeriphClockCmd(   RCC_APB2Periph_GPIOA 
							| RCC_APB2Periph_GPIOB
			 	 	 	 	| RCC_APB2Periph_GPIOC 
                         //   | RCC_APB2Periph_GPIOD 
						//	| RCC_APB2Periph_AFIO
							| RCC_APB2Periph_TIM1
						//	| RCC_APB2Periph_ADC1
                            | RCC_APB2Periph_USART1
 							, ENABLE );

 	RCC_APB1PeriphClockCmd( RCC_APB1Periph_TIM3
							| RCC_APB1Periph_TIM4
                            | RCC_APB1Periph_TIM2
							, ENABLE);
    
}
