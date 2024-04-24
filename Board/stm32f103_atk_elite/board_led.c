#include "stm32f10x.h"
#include "board_led.h"

/* private variables */
static struct led_init_type led0 = {GPIO_Pin_5, GPIOB, RCC_APB2Periph_GPIOB};
static struct led_init_type led1 = {GPIO_Pin_5, GPIOE, RCC_APB2Periph_GPIOE};
static struct led_init_type beep = {GPIO_Pin_8, GPIOB, RCC_APB2Periph_GPIOB};
static struct led_init_type* led_cfg[LEDn] = {&led0, &led1, &beep};
struct led_init_type** led_cfgs = led_cfg;