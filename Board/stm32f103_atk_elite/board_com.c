#include "stm32f10x.h"
#include "board_com.h"

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

static struct com_type* com_cfg[COMn] = {&com0};
struct com_type** com_cfgs = com_cfg;