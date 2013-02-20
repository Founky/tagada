#ifndef __CORE_H__
#define __CORE_H__

#include <stdint.h>

/* instruction format : aaaaaabbbbbooooo */
#define OP_A(X) ((X & 0b1111110000000000) >> 10) /* operande a */ 
#define OP_B(X) ((X & 0b0000001111111111) >>  5) /* operande b */
#define OC(X)   ((X & 0b0000000000011111) >>  0) /* opCode */


void initCore();


struct {
  uint16_t A;
  uint16_t B;
  uint16_t C;
  uint16_t X;
  uint16_t Y;
  uint16_t Z;
  uint16_t I;
  uint16_t J;
} regs;

#endif
