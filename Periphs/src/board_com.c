#include "stm32f10x.h"
#include "board_com.h"
#include "stdio.h"

static struct com_type com0 = {
    USART1,
    {
        115200,
        USART_WordLength_8b,
        USART_StopBits_1,
        USART_Parity_No,
        USART_Mode_Rx | USART_Mode_Tx,
        USART_HardwareFlowControl_None
    },
    RCC_APB2Periph_USART1, 
    GPIO_Pin_9, 
    GPIO_Pin_10, 
    GPIOA, 
    RCC_APB2Periph_GPIOA,
    {
        .NVIC_IRQChannel = USART1_IRQn,
        .NVIC_IRQChannelPreemptionPriority=3,
        .NVIC_IRQChannelSubPriority = 3,
        .NVIC_IRQChannelCmd = ENABLE,
    }
};


static struct com_type com1 = {
    USART2,
    {
        9600,
        USART_WordLength_8b,
        USART_StopBits_1,
        USART_Parity_No,
        USART_Mode_Rx | USART_Mode_Tx,
        USART_HardwareFlowControl_None
    },
    RCC_APB1Periph_USART2, 
    GPIO_Pin_2,
    GPIO_Pin_3, 
    GPIOA, 
    RCC_APB2Periph_GPIOA,
    {
        .NVIC_IRQChannel = USART2_IRQn,
        .NVIC_IRQChannelPreemptionPriority=3,
        .NVIC_IRQChannelSubPriority = 3,
        .NVIC_IRQChannelCmd = ENABLE,
    }
};
static struct com_type* com_cfg[COMn] = {&com0, &com1};
struct com_type** com_cfgs = com_cfg;

//重定义fputc函数 
/* retarget the C library printf function to the USART */
int _write (int fd, char *pBuffer, int size)  
{  
    for (int i = 0; i < size; i++)  
    {  
		USART_SendData(com_cfgs[COM_IDEX]->usart, (uint8_t)pBuffer[i]);
		while(RESET == USART_GetFlagStatus(com_cfgs[COM_IDEX]->usart, USART_FLAG_TC));		
    }  
    return size;  
}