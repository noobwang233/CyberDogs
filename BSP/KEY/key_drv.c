#include "stm32f10x.h"
#include "key_drv.h"
#include "stdio.h"

extern struct key_init_type** key_cfgs;
/*!
    \brief      configure key
    \param[in]  key_num: specify the key to be configured
      \arg        KEY_USER: user key
      \arg        KEY...
    \param[in]  key_mode: specify button mode
      \arg        KEY_MODE_GPIO: key will be used as simple IO
      \arg        KEY_MODE_EXTI: key will be connected to EXTI line with interrupt
    \param[out] none
    \retval     none
*/
void key_init(uint8_t key_num, keymode_typedef_enum key_mode)
{
    /* enable the key clock */
    RCC_APB2PeriphClockCmd(key_cfgs[key_num]->key_clk, ENABLE);

    /* configure button pin as input */
    GPIO_Init(key_cfgs[key_num]->gpio_port, &key_cfgs[key_num]->gpio_type);

    if (key_mode == KEY_MODE_EXTI) {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
        
        GPIO_EXTILineConfig(key_cfgs[key_num]->key_exit.port_source,key_cfgs[key_num]->key_exit.pin_source);
        
        EXTI_Init(&key_cfgs[key_num]->key_exit.exit_cfg);

        NVIC_Init(&key_cfgs[key_num]->key_nvic);
    }
}

/*!
    \brief      return the selected key state
    \param[in]  key: specify the key to be checked
      \arg        KEY_USER: user key
    \param[out] none
    \retval     the key's GPIO pin value
*/
key_gpio_state key_state_get(uint8_t key_index)
{
    key_gpio_state key_state = GPIO_ReadInputDataBit(key_cfgs[key_index]->gpio_port, key_cfgs[key_index]->gpio_type.GPIO_Pin);
    if(key_cfgs[key_index]->key_valid == LOW_VALID)
        return key_state;
    else
        return !key_state;
}