#ifndef BLDC_H
#define BLDC_H

#define	MANPOWER	0
#define	START	1
#define	RUN	    2
#define	STOP    3
#define FAULT   4


// IR2136S  ʹ�ܶ˿�   PB12
#define IR2136_EN_SET  GPIO_SetBits(GPIOB, GPIO_Pin_12)    //PB12
#define IR2136_EN_RESET GPIO_ResetBits(GPIOB, GPIO_Pin_12)   //PB12  

extern void BLDC_GPIOConfig(void);
extern void BLDC_TIM1Config(void);
extern void BLDC_TIM3Config(void);
extern void huanxiang(void);
extern void BLDC_Start(void);
extern void BLDC_Start1(void);
extern void BLDC_Stop(void);
#endif
