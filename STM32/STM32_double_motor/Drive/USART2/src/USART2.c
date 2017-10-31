#include "string.h"
#include "stdio.h"
#include "USART2.h"
#include "BLDC.h"
#include "PID.h"
#include "stdarg.h"
#include "misc.h"
#include "stm32f10x.h"
#include "stdlib.h"
#include "string.h"
#include "stdarg.h"
#include "USART_Function.h"

static char cmd_line[MAX_CMD_LINE_LENGTH + 1];
static char *cmd_argv[MAX_ARGC];

//���ڽ��ջ����� 	
u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//���ջ���,���USART2_MAX_RECV_LEN���ֽ�.
u16 USART1_RX_STA=0;  


static cmd_struct cmd_tbl[] = {
    /*
     * ������������Ҫ�ڴ˼��ϣ�
     * CMD_ADD("������","����ʹ�÷�������Ϊ�ո�,�����ܲ���˫���ţ�",��Ӧ�����ִ�к�����)
     * ע�����һ������Ҫ���ţ�ǰ��Ķ���Ҫ����
     */
    CMD_ADD("help","Print all command and usage",set_speed_p),
		
	//���stop
	
};


void USART1_Config(void)
{        int i;    
    
    USART_InitTypeDef USART_InitStruct;
    GPIO_InitTypeDef	GPIO_InitStruct;
    USART_ClockInitTypeDef USART_ClockInitStruct;
    
     GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
     GPIO_Init(GPIOB, &GPIO_InitStruct);

     GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
     GPIO_Init(GPIOB,&GPIO_InitStruct);   
 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);
    GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);      //��ӳ��PB6/7
      
 	USART_InitStruct.USART_BaudRate            = 115200;	  					
	USART_InitStruct.USART_StopBits            = USART_StopBits_1;	 
	USART_InitStruct.USART_WordLength          = USART_WordLength_8b; 
	USART_InitStruct.USART_Parity              = USART_Parity_No;
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStruct.USART_Mode                = USART_Mode_Rx | USART_Mode_Tx;

    USART_ClockInitStruct.USART_Clock=USART_Clock_Disable;
    USART_ClockInitStruct.USART_CPHA=USART_CPHA_2Edge;
    USART_ClockInitStruct.USART_CPOL=USART_CPOL_Low;
    USART_ClockInitStruct.USART_LastBit=USART_LastBit_Disable;
    USART_ClockInit(USART1,&USART_ClockInitStruct);

	USART_Init(USART1, &USART_InitStruct);
  	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);

    for(i = 0;i < MAX_ARGC;i++){
        cmd_argv[i] = (char *)malloc(MAX_CMD_ARG_LENGTH + 1);
    }
    

}


void USART1_IRQHandler(void){

    static u32 cmd_line_index = 0,cmd_line_length = 0;
    
    int cmd_argc,i;
    int erro_n;
    u8 c_recv;

    if(USART_GetITStatus(USART1,USART_IT_RXNE) != RESET){     
        
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
        
        c_recv = USART_ReceiveData(USART1);	// ����������c_recv
        
        if(c_recv == '\n'){  //�������  һ��ָ��
			uprintf(USART1,"done");
            if(cmd_line_index == 0){   // ��ӡ Done
                return;
            }
            
            cmd_line[cmd_line_length++] = (char)c_recv;   // ���� \n
            
            erro_n = cmd_parse(cmd_line,&cmd_argc,cmd_argv);  //��������  argc �洢�����ĸ���   set p 50  ��������
                                                                    // argv Ϊÿһ���������ַ������飩�׵�ַ
            if(erro_n < 0){           //���ؽ�����Ϣ
                //��ӡ����ִ�д�����Ϣ
                if(erro_n == -3){
                cmd_line_index = 0;
                cmd_line_length = 0;
                memset(cmd_line,0,MAX_CMD_LINE_LENGTH);
                return;
                }else if(erro_n == -2){
                    uprintf(USART1,"\nthe param is too long");
                }else if(erro_n == -1){
                    uprintf(USART1,"\ntoo many param");
                }
                cmd_line_index = 0;
                cmd_line_length = 0;
                memset(cmd_line,0,MAX_CMD_LINE_LENGTH + 1);
                return;
            }
                 
            erro_n = cmd_exec(cmd_argc,cmd_argv);   //�������   ִ������
            
            if(erro_n < 0){
                //��ӡ����ִ�д�����Ϣ
                if(erro_n == -2){
                  	  uprintf(USART1,"\r\nnot find commmand:%s\n",cmd_argv[0]);
                }
                cmd_line_index = 0;
                cmd_line_length = 0;
                memset(cmd_line,0,MAX_CMD_LINE_LENGTH + 1);
                return;
            }
            cmd_line_index = 0;
            cmd_line_length = 0;
            memset(cmd_line,0,MAX_CMD_LINE_LENGTH + 1);
        }    
        
        else{ // û�н���������
        
            if(cmd_line_index == MAX_CMD_LINE_LENGTH){  // 32
                //��ӡ������̫������Ϣ
                cmd_line_index = 0;
                cmd_line_length = 0;
                return;
            }
            
            for(i = 0;i < cmd_line_length - cmd_line_index;i++){
                cmd_line[cmd_line_length - i] = cmd_line[cmd_line_length - i -1];
            }
            
            cmd_line[cmd_line_index] = (char)c_recv;   // c_recv = USART_ReceiveData(USART1);	// ����������c_recv
			cmd_line_index++;
            cmd_line_length++;
        }			
    }
}

int cmd_parse(char *cmd_line,int *argc,char *argv[]){    //cmd_line ������ (��ַ)
    char c_temp;   
    int i = 0,arg_index = 0;
    int arg_cnt = 0;
    
    c_temp = cmd_line[i++];   // c_temp ���յ�������
    
    while(c_temp != '\r'){    // û�н���
        
        if(c_temp == ' '){      // 1 �ո����������  �����ǲ���
            
            if(arg_index == 0){     //���������߲����ַ�����һ���ǿո������  ����ѭ��
                c_temp = cmd_line[i++];
                continue;         
            }
            
                     //�ո�Ϊ������������ķָ���
            if(arg_cnt == MAX_ARGC){   //���������������,�򷵻�  -1    ��������  \=  ��������
                return -1;
            }
            
            argv[arg_cnt][arg_index] = 0;
            arg_cnt++;//����
            arg_index = 0;
            c_temp = cmd_line[i++];
            continue;
        }                            //  1
                
        if(arg_index == MAX_CMD_ARG_LENGTH){   //��������������ȹ������򱨴���
            return -2;
        }
        
        argv[arg_cnt][arg_index++] = c_temp;   // ��һ���ո���  ��argv[1][arg_index++] 
                                                   // arg_cntΪ�ڼ�������   arg_indexΪĳ�������ĵڼ�λ
        c_temp = cmd_line[i++];
    }
    
      
    if(arg_cnt == 0 && arg_index == 0){  //���������߲����ǿյģ��򷵻�
        return -3;
    }
    
    //���һ�������Ľ���û���������whileѭ���н�����
    argv[arg_cnt++][arg_index] = 0;
    *argc = arg_cnt;   // argc �洢�����ĸ���   set p 50  ��������
    return 0;
}



int cmd_exec(int argc,char *argv[]){
    int cmd_index = 0;
    u32 cmd_num;
    cmd_num = sizeof(cmd_tbl)/sizeof(cmd_tbl[0]);
		
    if(argc == 0){  //��������ǿյģ��򷵻�
        return -1;
    }
    
    for(cmd_index = 0;cmd_index < cmd_num;cmd_index++){   //��������
        if(strcmp((char *)(cmd_tbl[cmd_index].cmd_name),(char *)argv[0]) == 0){  //����ҵ��������ִ���������Ӧ�ĺ���
            uprintf(USART1,"\n");
            cmd_tbl[cmd_index].cmd_func(argc,argv);
            return 0;
    }  }
    return -2;
}

void uprintf(USART_TypeDef* USARTx, char *fmt, ...)
 {
	char buffer[128];
	u8 i = 0;
     
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, 128, fmt, arg_ptr);
     
	USART_ClearFlag(USARTx,USART_FLAG_TXE);
	while ((i < 128) && buffer[i])
	{
		if(buffer[i] == '\n'){
			
			USART_SendData(USARTx,(u8)'\r');
			while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); 
			USART_SendData(USARTx,(u8)buffer[i++]);
		}else{
	    USART_SendData(USARTx, (u8) buffer[i++]);
		}
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET); 
	}
	va_end(arg_ptr);
 }
