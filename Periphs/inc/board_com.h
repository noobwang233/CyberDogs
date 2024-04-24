#ifndef _BOARD_COM_H
#define _BOARD_COM_H

#ifdef cplusplus
 extern "C" {
#endif

#include "com_drv.h"
#include "main.h"


#define COMn                             2U

typedef enum 
{
    COM_0 = 0,          //USART1
    COM_1 = 0,          //USART2
} com_index;

/* exported types */

#ifdef cplusplus
}
#endif

#endif /* GD32F303C_START_H */
