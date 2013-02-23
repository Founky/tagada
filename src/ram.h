#ifndef __RAM_H__
#define __RAM_H__

/* this file contrains the ram description */

#include <stdint.h>

#define RAM_SIZE 0x10000 /* in words of 16 bits */

extern uint16_t ram[RAM_SIZE];

#endif
