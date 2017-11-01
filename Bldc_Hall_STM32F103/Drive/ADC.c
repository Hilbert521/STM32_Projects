#include "ADC.h"
#include "stm32f10x.h"
#include "USART2.h"
#include "main.h"

vu16 AD_Buf[4]; //�������ADCת�������Ҳ��DMA��Ŀ���ַ
extern vu16 I_Thrut[4];  // ��ѹֵ

#define ADC1_DR_Address     ((u32)0x4001244C)

//ѭ������   

void Get_Adc_Average(void)
{ 
    u8 count; 
     ADC_SoftwareStartConvCmd(ADC1, ENABLE);
     while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC)); //�ȴ�ת������	
     for ( count=0;count<4;count++)
        {
            I_Thrut[count]=(u16)(AD_Buf[count]* 330 / 4096);
        } 
}

void  Adc_Init(void)
{ 	
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

   
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1, ENABLE );	  //ʹ��ADC1ͨ��ʱ��
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//PA0/1/2/3  ģ�������                        
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	
    
                           
     GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		
	 GPIO_Init(GPIOC, &GPIO_InitStructure);	
    
	 ADC_DeInit(ADC1);  //��λADC1

    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent; //ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
    ADC_InitStructure.ADC_ScanConvMode =ENABLE; //ģ��ת��������ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = DISABLE; //ģ��ת������������ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None; 
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; //ADC�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel =4; //˳����й���ת����ADCͨ������Ŀ
    ADC_Init(ADC1, &ADC_InitStructure); //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���  
    
        
 //����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
    //ADC1,ADCͨ��x,�������˳��ֵΪy,����ʱ��Ϊ239.5����
  
   ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_239Cycles5 );
   ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 2, ADC_SampleTime_239Cycles5 );
   ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 3, ADC_SampleTime_239Cycles5 );
   ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 4, ADC_SampleTime_239Cycles5 );   //V_BUS
    
 // ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����
    ADC_DMACmd(ADC1, ENABLE);   
    
//     ADC_InjectedSequencerLengthConfig(ADC1, 1);
//     ADC_InjectedChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_7Cycles5);//CURRENT  
//     ADC_InjectedChannelConfig(ADC1,ADC_Channel_3,2,ADC_SampleTime_7Cycles5);//CURRENT  
//     ADC_InjectedChannelConfig(ADC1,ADC_Channel_4,1,ADC_SampleTime_7Cycles5);//CURRENT  
//     ADC_ExternalTrigInjectedConvConfig(ADC1, ADC_ExternalTrigInjecConv_T1_CC4);//�������ʹ��time1��CCR4�¼�����������
//     ADC_ExternalTrigInjectedConvCmd(ADC1,ENABLE);
//     ADC_SoftwareStartInjectedConvCmd(ADC1, ENABLE);
//     
//     ADC_ITConfig(ADC1,ADC_IT_JEOC,ENABLE);//ENABLE INJECTED INTERRUPT

	ADC_Cmd(ADC1, ENABLE);	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
}	




 void DMA_Configuration(void)
{      
    DMA_InitTypeDef DMA_InitStructure;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    DMA_DeInit(DMA1_Channel1); //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr = ADC1_DR_Address; //DMA����ADC����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&AD_Buf; //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
    DMA_InitStructure.DMA_BufferSize = 4; //DMAͨ����DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; //�����ַ�Ĵ�������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; //�ڴ��ַ�Ĵ�������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //���ݿ��Ϊ16λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��Ϊ16λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; //������ѭ������ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable; //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
    DMA_Init(DMA1_Channel1, &DMA_InitStructure); //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��
    DMA_Cmd(DMA1_Channel1, ENABLE); //����DMAͨ��
}

extern u8 ERROR_flag;
 void ADC1_2_IRQHandler(void)
 {

 }






