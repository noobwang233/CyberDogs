#ifndef __CAN_H
#define __CAN_H	 
#include "sys.h"	    
 //////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//CAN驱动 代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/11
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//CAN接收RX0中断使能
#define CAN_RX0_INT_ENABLE	0		//0,不使能;1,使能.								    
										 							 				    
uint8_t CAN_Mode_Init(uint8_t tsjw,uint8_t tbs2,uint8_t tbs1,uint16_t brp,uint8_t mode);//CAN初始化
 
uint8_t Can_Send_Msg(uint8_t* msg,uint8_t len);						//发送数据

uint8_t Can_Receive_Msg(uint8_t *buf);							//接收数据
#endif

















