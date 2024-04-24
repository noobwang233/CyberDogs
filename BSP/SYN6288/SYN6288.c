#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "stm32f10x.h"
#include "SYN6288.h"
#include "board_com.h"


#define GB2312_CODE     0
#define GBK_CODE        1
#define BIG5_CODE       2
#define UNICODE_CODE    3

#define MAKE_CMD_ARG(Music,code)   (Music << 3 | code)

extern struct com_type** com_cfgs;
/**************芯片设置命令*********************/
struct SYN_Data  SYN_StopCom ={0xFD,{0X00,0X02},0X02,NULL,NULL,0XFD};//停止合成
struct SYN_Data  SYN_SuspendCom={0XFD,{0X00,0X02},0X03,NULL,NULL,0XFC};//暂停合成
struct SYN_Data  SYN_RecoverCom={0XFD,{0X00,0X02},0X04,NULL,NULL,0XFB};//恢复合成
struct SYN_Data  SYN_ChackCom={0XFD,{0X00,0X02},0X21,NULL,NULL,0XDE};//状态查询
struct SYN_Data  SYN_PowerDownCom={0XFD,{0X00,0X02},0X88,NULL,NULL,0X77};//进入POWER DOWN 状态命令
struct SYN_Data  SYN_SetBoundCom ={0xFD,{0X00,0X02},0X31,NULL,NULL,0X00};//设置波特率
/***********************************************/

uint8_t calculateXOR(uint8_t* data, uint8_t length) {
    uint8_t xorValue = 0;
    for(uint8_t i = 0; i < length; i++) {
        xorValue ^= data[i];
    }
    return xorValue;
}

//其实是USART2_Send_Byte
void SYN6288_Send_Byte( unsigned char byte )
{
		USART_SendData(com_cfgs[SYN6288]->usart, (uint8_t)byte);
		while(RESET == USART_GetFlagStatus(com_cfgs[SYN6288]->usart, USART_FLAG_TC));	
}


void Speech_SYN_Data(struct SYN_Data data)
{
    SYN6288_Send_Byte(data.cmd);
    SYN6288_Send_Byte(data.len[0]);
    SYN6288_Send_Byte(data.len[1]);
    SYN6288_Send_Byte(data.cmd);
    if(data.cmd_arg != NULL)
        SYN6288_Send_Byte(*data.cmd_arg);
    if(data.text != NULL)
    {
        char * p;
        for(p = data.text; *p != '\0'; p++)
            SYN6288_Send_Byte(*p);
    }
    SYN6288_Send_Byte(data.XOR);
}

/***********************************************************
* 名    称：  YS-SYN6288 文本合成函数
* 功    能：  发送合成文本到SYN6288芯片进行合成播放
* 入口参数：Music(背景音乐选择):0无背景音乐。1-15：相关背景音乐
            *HZdata:文本指针变量 
* 出口参数：
* 说    明： 本函数只用于文本合成，具备背景音乐选择。默认波特率9600bps。					 
* 调用方法：例： SYN_FrameInfo（0，“乐声电子科技”）；
**********************************************************/
void SYN_Speech(uint8_t Music,char *HZdata)
{
/****************需要发送的文本**********************************/
        unsigned  char  HZ_Length;  
        uint8_t     cmd_arg;
        struct SYN_Data data = {.head= 0xFD};
        data.cmd = 0x01u;//构造命令字：合成播放命令
        cmd_arg = MAKE_CMD_ARG(Music,UNICODE_CODE) ;  //构造命令参数：背景音乐设定
        data.cmd_arg = &cmd_arg;
        HZ_Length =strlen((const char *)HZdata); 			//需要发送文本的长度
        if(HZ_Length > 200)
            HZ_Length = 200;
        data.len[1] = HZ_Length + 3;
        data.XOR = calculateXOR((uint8_t *)&data, 4);
        data.XOR = calculateXOR(data.cmd_arg, 1);
        data.text = HZdata;
        data.XOR = calculateXOR((uint8_t *)data.text, HZ_Length);
		Speech_SYN_Data(data);
}


/***********************************************************
* 名    称： void  main(void)
* 功    能： 主函数	程序入口
* 入口参数： *Info_data:固定的配置信息变量 
* 出口参数：
* 说    明：本函数用于配置，停止合成、暂停合成等设置 ，默认波特率9600bps。					 
* 调用方法：通过调用已经定义的相关数组进行配置。 
**********************************************************/
void YS_SYN_Set(struct SYN_Data *data)
{
    Speech_SYN_Data(*data);
}

void SYN_SetBound(uint32_t bound)
{
/****************需要发送的文本**********************************/
        uint8_t cmd_arg;
        struct SYN_Data data = SYN_SetBoundCom;
        data.cmd_arg = NULL;
        if(bound == 9600)
            cmd_arg = 0;
        else if (bound == 19200)
            cmd_arg = 1;
        else
            cmd_arg = 2;
        data.cmd_arg = &cmd_arg;
        data.XOR = calculateXOR((uint8_t *)&data, 4);
        data.XOR = calculateXOR(data.cmd_arg, 1);
        Speech_SYN_Data(data);
}

