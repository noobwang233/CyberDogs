#include "stm32f10x.h"
#include "board_key.h"

/* private variables */
static struct key_init_type key0 = {
    {
        GPIO_Pin_4, 
        0, 
        GPIO_Mode_IPU
    }, 
    GPIOE, 
    RCC_APB2Periph_GPIOE, 
    {
        {
            EXTI_Line4,
            EXTI_Mode_Interrupt,
            EXTI_Trigger_Falling,
            ENABLE
        },
        GPIO_PortSourceGPIOE, 
        GPIO_PinSource4, 
    },
    {
        EXTI4_IRQn,
        0x02,
        0x03,
        ENABLE
    },
    LOW_VALID
};


static struct key_init_type key1 = {
    {
        GPIO_Pin_3, 
        0, 
        GPIO_Mode_IPU
    }, 
    GPIOE, 
    RCC_APB2Periph_GPIOE, 
    {
        {
            EXTI_Line3,
            EXTI_Mode_Interrupt,
            EXTI_Trigger_Falling,
            ENABLE
        },
        GPIO_PortSourceGPIOE, 
        GPIO_PinSource3, 
    },
    {
        EXTI3_IRQn,
        0x02,
        0x02,
        ENABLE
    },
    LOW_VALID
};


static struct key_init_type key_up = {
    {
        GPIO_Pin_0, 
        0, 
        GPIO_Mode_IPD
    }, 
    GPIOA, 
    RCC_APB2Periph_GPIOA, 
    {
        {
            EXTI_Line0,
            EXTI_Mode_Interrupt,
            EXTI_Trigger_Rising,
            ENABLE
        },
        GPIO_PortSourceGPIOA, 
        GPIO_PinSource0, 
    },
    {
        EXTI0_IRQn,
        0x02,
        0x02,
        ENABLE
    },
    HIGH_VALID
};

static struct key_init_type* key_cfg[KEYn] = {&key0, &key1, &key_up};
struct key_init_type** key_cfgs = key_cfg;