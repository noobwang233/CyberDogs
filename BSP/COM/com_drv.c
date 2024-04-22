#include "com_drv.h"	  
////////////////////////////////////////////////////////////////////////////////// 	 

#include "FreeRTOS.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32开发板
//串口1初始化		   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/8/18
//版本：V1.5
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved
//********************************************************************************
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
//V1.5修改说明
//1,增加了对UCOSII的支持
////////////////////////////////////////////////////////////////////////////////// 	  
 

//////////////////////////////////////////////////////////////////
extern struct com_type** com_cfgs;
static uint8_t com_idex = 0;

//重定义fputc函数 
/* retarget the C library printf function to the USART */
int _write (int fd, char *pBuffer, int size)  
{  
    for (int i = 0; i < size; i++)  
    {  
		USART_SendData(com_cfgs[com_idex]->usart, (uint8_t)pBuffer[i]);
		while(RESET == USART_GetFlagStatus(com_cfgs[com_idex]->usart, USART_FLAG_TC));		
    }  
    return size;  
}

//串口1中断服务程序
//注意,读取USARTx->SR能避免莫名其妙的错误   	
uint8_t USART_RX_BUF[USART_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
//接收状态
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
uint16_t USART_RX_STA=0;       //接收状态标记	  

void com_init(uint8_t index, uint32_t bound)
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    com_idex = index;
    /* enable GPIO clock */
    RCC_APB2PeriphClockCmd(com_cfgs[com_idex]->gpio_clk, ENABLE);

    /* enable USART clock */
    RCC_APB2PeriphClockCmd(com_cfgs[com_idex]->usart_clk, ENABLE);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
    /* connect port to USARTx_Tx */
    GPIO_InitStructure.GPIO_Pin = com_cfgs[com_idex]->tx_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(com_cfgs[com_idex]->gpio_port, &GPIO_InitStructure);

    /* connect port to USARTx_Rx */
    GPIO_InitStructure.GPIO_Pin = com_cfgs[com_idex]->rx_pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(com_cfgs[com_idex]->gpio_port, &GPIO_InitStructure);

    /* USART configure */
    com_cfgs[com_idex]->usart_cfg.USART_BaudRate = bound;
    USART_Init(com_cfgs[com_idex]->usart, &com_cfgs[com_idex]->usart_cfg); //初始化串口

    //Usart1 NVIC 配置
    NVIC_Init(&com_cfgs[com_idex]->com_nvic);	//根据指定的参数初始化VIC寄存器
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断

    USART_Cmd(com_cfgs[com_idex]->usart, ENABLE);
}


void USART1_IRQHandler(void)                	//串口1中断服务程序
	{
	uint8_t Res;
// #if SYSTEM_SUPPORT_OS 		//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
// 	OSIntEnter();
// #endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
    {
        Res =USART_ReceiveData(USART1);	//读取接收到的数据
        
        if((USART_RX_STA&0x8000)==0)//接收未完成
        {
            if(USART_RX_STA&0x4000)//接收到了0x0d
            {
                if(Res!=0x0a)USART_RX_STA=0;//接收错误,重新开始
                else USART_RX_STA|=0x8000;	//接收完成了 
            }
            else //还没收到0X0D
            {	
                if(Res==0x0d)
                    USART_RX_STA|=0x4000;
                else
                {
                    USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
                    USART_RX_STA++;
                    if(USART_RX_STA>(USART_REC_LEN-1))
                        USART_RX_STA=0;//接收数据错误,重新开始接收	  
                }
            }
        }
    }
  // #if SYSTEM_SUPPORT_OS 	//如果SYSTEM_SUPPORT_OS为真，则需要支持OS.
  // 	OSIntExit();  											 
  // #endif
}

