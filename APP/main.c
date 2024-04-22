#include "stm32f10x.h"
#include "board_periph.h"
#include <stdio.h>
#include "delay.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"

#include "task_key_scan.h"

TaskHandle_t LED0_Task_Handle = NULL; /* 任务句柄 */
TaskHandle_t LED1_Task_Handle = NULL;
TaskHandle_t BEEP_Task_Handle = NULL;
TaskHandle_t TaskCreate_Handle = NULL;
TaskHandle_t LCD_Task_Handle = NULL;
static void LED_Task(void* parameter)
{
    led_typedef_enum LED = (led_typedef_enum)parameter;
    while (1)
    {
        led_on(LED);
        vTaskDelay(pdMS_TO_TICKS(500)); /* 延时500 ms */
        led_off(LED);
        vTaskDelay(pdMS_TO_TICKS(500)); /* 延时500 ms */
    }
}

static void LCD_Task(void* parameter)
{
    uint8_t t = ' ';
    while (1)
    {
        if(t > 'Z')
            t = ' ';
        LCD_ShowChar(10,10,t,24,0);
        vTaskDelay(pdMS_TO_TICKS(1000)); /* 延时1s */
        t++;
    }
}

static void TaskCreate_Task(void* parameter)
{
    taskENTER_CRITICAL(); //进入临界区

    configASSERT(xTaskCreate(   (TaskFunction_t )LED_Task, /* 任务入口函数 */
                                (const char* )"LED0_Task",/* 任务名字 */
                                (uint16_t )128, /* 任务栈大小 */
                                (void* )LED0, /* 任务入口函数参数 */
                                (UBaseType_t )2, /* 任务的优先级 */
                                (TaskHandle_t* )&LED0_Task_Handle)/* 任务句柄指针 */);

    configASSERT(xTaskCreate(   (TaskFunction_t )LED_Task, /* 任务入口函数 */
                                (const char* )"LED1_Task",/* 任务名字 */
                                (uint16_t )128, /* 任务栈大小 */
                                (void* )LED1, /* 任务入口函数参数 */
                                (UBaseType_t )2, /* 任务的优先级 */
                                (TaskHandle_t* )&LED1_Task_Handle)/* 任务句柄指针 */);
    configASSERT(xTaskCreate(   (TaskFunction_t )LCD_Task, /* 任务入口函数 */
                                (const char* )"LCD_Task",/* 任务名字 */
                                (uint16_t )128, /* 任务栈大小 */
                                (void* )NULL, /* 任务入口函数参数 */
                                (UBaseType_t )2, /* 任务的优先级 */
                                (TaskHandle_t* )&LCD_Task_Handle)/* 任务句柄指针 */);
    // configASSERT(xTaskCreate(   (TaskFunction_t )LED_Task, /* 任务入口函数 */
    //                             (const char* )"BEEP_Task",/* 任务名字 */
    //                             (uint16_t )128, /* 任务栈大小 */
    //                             (void* )BEEP, /* 任务入口函数参数 */
    //                             (UBaseType_t )2, /* 任务的优先级 */
    //                             (TaskHandle_t* )&LED1_Task_Handle)/* 任务句柄指针 */);
    configASSERT(key_task_init(KEY_0, 256));
    configASSERT(key_task_init(KEY_1, 256));
    configASSERT(key_task_init(KEY_UP, 256));

    vTaskDelete(TaskCreate_Handle); //删除TaskCreate 任务

    taskEXIT_CRITICAL(); //退出临界区
}

int main()
{
    BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
    NVIC_SetPriorityGrouping(NVIC_PriorityGroup_4);
    delay_init();
    led_init(LED0);
    led_init(LED1);
    led_init(BEEP);
    key_init(KEY_0, KEY_MODE_GPIO);
    key_init(KEY_1, KEY_MODE_GPIO);
    key_init(KEY_UP, KEY_MODE_GPIO);
    com_init(COM_IDEX,115200);
    LCD_Init();
    xReturn = xTaskCreate(  (TaskFunction_t )TaskCreate_Task, /* 任务入口函数 */
                                (const char* )"TaskCreate_Task",/* 任务名字 */
                            (uint16_t )256, /* 任务栈大小 */
                            (void* )NULL, /* 任务入口函数参数 */
                            (UBaseType_t )3, /* 任务的优先级 */
                            (TaskHandle_t* )&TaskCreate_Handle);/* 任务句柄指针 */
    if (pdPASS == xReturn)
        vTaskStartScheduler(); /* 启动任务，开启调度 */

    while (1){}


    return 0;
}