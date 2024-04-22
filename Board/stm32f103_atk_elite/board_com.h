#ifndef _BOARD_COM_H
#define _BOARD_COM_H

#ifdef cplusplus
 extern "C" {
#endif

#include "com_drv.h"

#define COMn                             1U
#define COM_IDEX                         0u
typedef enum 
{
    COM_0 = 0,          //USART1
} com_index;

/* exported types */

#ifdef cplusplus
}
#endif

#endif /* GD32F303C_START_H */
