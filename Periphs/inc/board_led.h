#ifndef _BOARD_LED_H
#define _BOARD_LED_H

#ifdef cplusplus
 extern "C" {
#endif

#include "led_drv.h"
/* exported types */
#define LEDn                             3U
typedef enum 
{
    LED0,           //PB5
    LED1,           //PE5
    BEEP            //PB8 BEEP
} led_typedef_enum;

#ifdef cplusplus
}
#endif

#endif /* GD32F303C_START_H */
