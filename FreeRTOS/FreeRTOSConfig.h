#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

//针对不同的编译器调用不同的stdint.h 文件
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
#include <stdint.h>
extern uint32_t SystemCoreClock;
#endif

//断言
#define configASSERT_DEFINED                             1
extern int	printf (const char *__restrict, ...);
#define vAssertCalled(char,int) printf("Error:%s,%d\r\n",char,int)
#define configASSERT( x )    if( ( x ) == 0 ) vAssertCalled(__FILE__,__LINE__)
#define configQUEUE_REGISTRY_SIZE                        20

#define configUSE_PREEMPTION                             1
#define configUSE_TIME_SLICING                           1
/* 某些运行FreeRTOS 的硬件有两种方法选择下一个要执行的任务：
 * 通用方法和特定于硬件的方法（以下简称“特殊方法”）。
 *
 * 通用方法：
 * 1.configUSE_PORT_OPTIMISED_TASK_SELECTION 为 0 或者硬件不支持这种特殊方法。
 * 2.可以用于所有FreeRTOS 支持的硬件
 * 3.完全用C 实现，效率略低于特殊方法。
 * 4.不强制要求限制最大可用优先级数目
 * 特殊方法：
 * 1.必须将configUSE_PORT_OPTIMISED_TASK_SELECTION 设置为1。
 * 2.依赖一个或多个特定架构的汇编指令（一般是类似计算前导零[CLZ]指令）。
 * 3.比通用方法更高效
 * 4.一般强制限定最大可用优先级数目为32
 一般是硬件计算前导零指令，如果所使用的，MCU 没有这些硬件指令的话此宏应该设置为0！
 */
#define configUSE_PORT_OPTIMISED_TASK_SELECTION          1

#define configUSE_IDLE_HOOK                              0
#define configUSE_TICK_HOOK                              0
#define configUSE_DAEMON_TASK_STARTUP_HOOK               0
#define configUSE_MALLOC_FAILED_HOOK                     0
#define configCHECK_FOR_STACK_OVERFLOW                   0

#define configGENERATE_RUN_TIME_STATS                    0

#define configCPU_CLOCK_HZ                               (SystemCoreClock)
#define configTICK_RATE_HZ                               ( ( TickType_t ) 1000 )

//可使用的最大优先级
#define configMAX_PRIORITIES                             (5)

//空闲任务使用的堆栈大小
#define configMINIMAL_STACK_SIZE ((unsigned short)128)

//任务名字字符串长度
#define configMAX_TASK_NAME_LEN                          ( 10 )
//系统节拍计数器变量数据类型，1 表示为16 位无符号整形，0 表示为32 位无符号整形
#define configUSE_16_BIT_TICKS 0

#define configTOTAL_HEAP_SIZE                            ( ( size_t ) ( 36*1024 ) )

#define configUSE_TRACE_FACILITY                         1
//空闲任务放弃CPU 使用权给其他同优先级的用户任务
#define configIDLE_SHOULD_YIELD                          1

//启用队列
#define configUSE_QUEUE_SETS                             1
#define configUSE_TASK_NOTIFICATIONS                     1

#define configTIMER_QUEUE_LENGTH                         10
#define configTIMER_TASK_PRIORITY                        ( configMAX_PRIORITIES - 1 )
#define configUSE_COUNTING_SEMAPHORES                    1
#define configSUPPORT_DYNAMIC_ALLOCATION                 1
#define configSUPPORT_STATIC_ALLOCATION                  0
#define configNUM_TX_DESCRIPTORS                        15
#define configSTREAM_BUFFER_TRIGGER_LEVEL_TEST_MARGIN    2

/* Set the following definitions to 1 to include the API function, or zero
 * to exclude the API function. */


#define configUSE_MUTEXES                         1
#define configUSE_RECURSIVE_MUTEXES               1
#define configUSE_TIMERS                          1
#define configUSE_COUNTING_SEMAPHORES             1
#define configTIMER_TASK_STACK_DEPTH              ( configMINIMAL_STACK_SIZE * 2 )

#define INCLUDE_vTaskPrioritySet                  1
#define INCLUDE_uxTaskPriorityGet                 1
#define INCLUDE_vTaskDelete                       1
#define INCLUDE_vTaskCleanUpResources             0
#define INCLUDE_vTaskSuspend                      1
#define INCLUDE_vTaskDelayUntil                   1
#define INCLUDE_vTaskDelay                        1
#define INCLUDE_uxTaskGetStackHighWaterMark       1
#define INCLUDE_uxTaskGetStackHighWaterMark2      1
#define INCLUDE_xTaskGetSchedulerState            1
#define INCLUDE_xTimerGetTimerDaemonTaskHandle    1
#define INCLUDE_xTaskGetIdleTaskHandle            1
#define INCLUDE_xTaskGetHandle                    1
#define INCLUDE_eTaskGetState                     1
#define INCLUDE_xSemaphoreGetMutexHolder          1
#define INCLUDE_xTimerPendFunctionCall            1
#define INCLUDE_xTaskAbortDelay                   1

#ifdef __NVIC_PRIO_BITS
	#define configPRIO_BITS								__NVIC_PRIO_BITS
#else
	#define configPRIO_BITS								4	 /* 8 priority levels. */
#endif
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0x15
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5
#define configKERNEL_INTERRUPT_PRIORITY					( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << ( 8 - configPRIO_BITS ) )
#define configMAX_SYSCALL_INTERRUPT_PRIORITY			( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << ( 8 - configPRIO_BITS ) )

#define RTE_CMSIS_RTOS_RTX
#define xPortPendSVHandler PendSV_Handler
#define vPortSVCHandler SVC_Handler
// #define xPortSysTickHandler SysTick_Handler

#endif /* FREERTOS_CONFIG_H */
