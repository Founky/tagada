#include <string.h>
#include <stdint.h>

#include "core.h"
#include "ram.h"

struct {
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
} regs;

void initCore() {
  memset(&regs, 0, 12 * sizeof(uint16_t));
}

/**
 *--- Values: (5/6 bits) ---------------------------------------------------------
 * C | VALUE     | DESCRIPTION
 *---+-----------+----------------------------------------------------------------
 * 0 | 0x00-0x07 | register (A, B, C, X, Y, Z, I or J, in that order)
 * 0 | 0x08-0x0f | [register]
 * 1 | 0x10-0x17 | [register + next word]
 * 0 |      0x18 | (PUSH / [--SP]) if in b, or (POP / [SP++]) if in a
 * 0 |      0x19 | [SP] / PEEK
 * 1 |      0x1a | [SP + next word] / PICK n
 * 0 |      0x1b | SP
 * 0 |      0x1c | PC
 * 0 |      0x1d | EX
 * 1 |      0x1e | [next word]
 * 1 |      0x1f | next word (literal)
 * 0 | 0x20-0x3f | literal value 0xffff-0x1e (-1..30) (literal) (only for a)
 * --+-----------+----------------------------------------------------------------
 */
uint16_t decodeAValue(uint16_t instruction) {
  uint8_t v = OP_A(instruction);
  // Registers
  if (v < 0x08) {
    return *((uint16_t *)&regs + v);
  // Ram[reg]
  } else if (v < 0x10) {
    return ram[*((uint16_t *)&regs + v - 0x08)];
  // Ram[reg + nextword]
  } else if (v < 0x18) {
    return ram[*((uint16_t *)&regs + v - 0x10) + ram[regs.PC + 0x02]];
  // ???
  } else if (v == 0x18) {
  // ???
  } else if (v == 0x19) {
  // ???
  } else if (v == 0x1a) {
  // SP
  } else if (v == 0x1b) {
    return ram[regs.SP];
  // PC
  } else if (v == 0x1c) {
    return ram[regs.PC];
  // EX
  } else if (v == 0x1d) {
    return ram[regs.EX];
  // ram[next word]
  } else if (v == 0x1e) {
    return ram[ram[regs.PC + 0x02]];
  // nextword
  } else if (v == 0x1f) {
    return ram[regs.PC + 0x02];
  // ??? 
  } else if (v <= 0x3f) {
  }
  return 0;
}

uint16_t decodeBValue(uint16_t instruction) {
  uint8_t v = OP_B(instruction);
  // Registers
  if (v < 0x08) {
    return *((uint16_t *)&regs + v);
  // Ram[reg]
  } else if (v < 0x10) {
    return ram[*((uint16_t *)&regs + v - 0x08)];
  // Ram[reg + nextword]
  } else if (v < 0x18) {
    return ram[*((uint16_t *)&regs + v - 0x10) + ram[regs.PC + 0x02]];
  // ???
  } else if (v == 0x18) {
  // ???
  } else if (v == 0x19) {
  // ???
  } else if (v == 0x1a) {
  // SP
  } else if (v == 0x1b) {
    return ram[regs.SP];
  // PC
  } else if (v == 0x1c) {
    return ram[regs.PC];
  // EX
  } else if (v == 0x1d) {
    return ram[regs.EX];
  // ram[next word]
  } else if (v == 0x1e) {
    return ram[ram[regs.PC + 0x02]];
  // nextword
  } else if (v == 0x1f) {
    return ram[regs.PC + 0x02];
  }
  return 0;
}
