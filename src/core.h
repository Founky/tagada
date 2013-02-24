#ifndef __CORE_H__
#define __CORE_H__

/* this file contains the DCPU16 core description and associated routines */

#include <stdint.h>

struct registerFile {
  uint16_t A; /* General registers */
  uint16_t B;
  uint16_t C;
  uint16_t X;
  uint16_t Y;
  uint16_t Z;
  uint16_t I;
  uint16_t J;
  uint16_t PC; /* Program counter */
  uint16_t SP; /* Stack pointer */
  uint16_t EX; /* Extra/excess register */
  uint16_t IA; /* Interrupt address */
};

extern struct registerFile regs;
extern uint8_t intQueing;

/* initialize DCPU16 core */
void initCore();
/* fetch next instruction, decode and execute */
void run();
#endif
