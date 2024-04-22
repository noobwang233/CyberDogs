#include "led_drv.h"

extern struct led_init_type** led_cfgs;
/*!
    \brief      configure led GPIO
    \param[in]  led_num: specify the led to be configured
      \arg        LED0
    \param[out] none
    \retval     none
*/
void  led_init (uint8_t led_num)
{
    /* enable the led clock */
    RCC_APB2PeriphClockCmd(led_cfgs[led_num]->gpio_clk, ENABLE);
    /* configure led GPIO port */ 
    GPIO_Init(led_cfgs[led_num]->gpio_port, &led_cfgs[led_num]->gpio_type);

    GPIO_ResetBits(led_cfgs[led_num]->gpio_port, led_cfgs[led_num]->gpio_type.GPIO_Pin);
}

/*!
    \brief      turn on selected led
    \param[in]  led_num: specify the led to be turned on
      \arg        LED0
    \param[out] none
    \retval     none
*/
void led_on(uint8_t led_num)
{
    GPIO_SetBits(led_cfgs[led_num]->gpio_port, led_cfgs[led_num]->gpio_type.GPIO_Pin);
}

/*!
    \brief      turn off selected led
    \param[in]  led_num: specify the led to be turned off
      \arg        LED0
    \param[out] none
    \retval     none
*/
void led_off(uint8_t led_num)
{
    GPIO_ResetBits(led_cfgs[led_num]->gpio_port, led_cfgs[led_num]->gpio_type.GPIO_Pin);
}

/*!
    \brief      toggle selected led
    \param[in]  led_num: specify the led to be toggled
      \arg        LED0
    \param[out] none
    \retval     none
*/
void led_toggle(uint8_t led_num)
{
    uint8_t led_state = GPIO_ReadOutputDataBit(led_cfgs[led_num]->gpio_port, led_cfgs[led_num]->gpio_type.GPIO_Pin);
    GPIO_WriteBit(led_cfgs[led_num]->gpio_port, led_cfgs[led_num]->gpio_type.GPIO_Pin, !led_state);
}