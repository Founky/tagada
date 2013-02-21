#ifndef __CORE_H__
#define __CORE_H__

#include <stdint.h>

/* instruction format : aaaaaabbbbbooooo */
#define OP_A(X) ((X & 0b1111110000000000) >> 10) /* operande a */ 
#define OP_B(X) ((X & 0b0000001111111111) >>  5) /* operande b */
#define OC(X)   ((X & 0b0000000000011111) >>  0) /* opCode */

uint16_t decodeAValue(uint16_t instruction);
uint16_t decodeBValue(uint16_t instruction);
uint8_t *decodeAAddress(uint16_t instruction);
uint8_t *decodeBAddress(uint16_t instruction);

void initCore();

#endif
