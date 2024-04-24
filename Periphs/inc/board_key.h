#ifndef _BOARD_KEY_H
#define _BOARD_KEY_H

#ifdef cplusplus
 extern "C" {
#endif

#include "key_drv.h"

/* exported types */
#define KEYn                             3U
typedef enum 
{
    KEY_0 = 0,          //PE4
    KEY_1,              //PE3
    KEY_UP              //PA0  WK_UP
} key_index;

#ifdef cplusplus
}
#endif

#endif /* GD32F303C_START_H */
