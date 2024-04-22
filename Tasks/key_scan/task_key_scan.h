#ifndef __KEY_SCAN_TASK_H
#define __KEY_SCAN_TASK_H	 

#include <stdint.h>
#include "FreeRTOS.h"
#include "task.h"
typedef enum
{
    RELEASE_STATE,        //空闲
    SHORT_PRESS_STATE,    //短按
    LONG_PRESS_STATE,     //长按
    DOUBLE_PRESS_STATE,   //双击
} KeyState;

struct key_task_type
{
    uint8_t key_index;
    char * key_name;
    KeyState key_state;
    TaskHandle_t key_handle;
};
BaseType_t key_task_init(uint8_t key_num, uint16_t stack_size);

#endif
