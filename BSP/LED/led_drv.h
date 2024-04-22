#ifndef __LED_DRV_H
#define __LED_DRV_H

#include "stm32f10x.h"
#include <stdint.h>

/* eval board low layer led */
struct led_init_type
{
    GPIO_InitTypeDef    gpio_type;
    GPIO_TypeDef *      gpio_port;
    uint32_t            gpio_clk;
};


/* configure led GPIO */
void led_init(uint8_t lednum);
/* turn on selected led */
void led_on(uint8_t lednum);
/* turn off selected led */
void led_off(uint8_t lednum);
/* toggle the selected led */
void led_toggle(uint8_t lednum);

#endif

