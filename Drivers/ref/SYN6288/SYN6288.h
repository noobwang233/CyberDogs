#ifndef SYN6288_H
#define SYN6288_H
#include <stdint.h>

#define SYN6288                         COM_1

struct SYN_Data {
	const uint8_t 	head;
	uint8_t 		len[2];
    uint8_t         cmd;
    uint8_t*     cmd_arg;
    char*       text;
    uint8_t         XOR;
};


void SYN_Speech(uint8_t Music,char *HZdata);
void YS_SYN_Set(struct SYN_Data *data);
void SYN_SetBound(uint32_t bound);

#endif
