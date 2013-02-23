#include <stdint.h>

#include "instructions.h"
#include "core.h"
#include "ram.h"



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
/* returns the appropriate value depending on the a value of the instruction */
uint16_t decodeAValue(uint16_t instruction) {
  uint8_t v = OP_A(instruction);
  /* Registers */
  if (v < 0x08) {
    return *((uint16_t *)&regs + v);
    /* Ram[reg] */
  } else if (v < 0x10) {
    return ram[*((uint16_t *)&regs + v - 0x08)];
    /* Ram[reg + nextword] */
  } else if (v < 0x18) {
    return ram[*((uint16_t *)&regs + v - 0x10) + ram[regs.PC + 0x01]];
    /* Pops the stack and return the head as value */
  } else if (v == 0x18) {
    return ram[regs.SP--];
    /* Return [SP] */
  } else if (v == 0x19) {
    return ram[regs.SP];
    /* Return the thing below dude */
  } else if (v == 0x1a) {
    return ram[regs.SP + ram[regs.PC + 1]];
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
    return ram[ram[regs.PC + 0x01]];
    // nextword
  } else if (v == 0x1f) {
    return ram[regs.PC + 0x01];
    // Small literal value from -1 to 0x1e (30)
  } else if (v <= 0x3f) {
    return 0x3f - v - 1;
  }
  return 0;
}

/* returns the appropriate value depending on the b value of the instruction */
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
    return ram[*((uint16_t *)&regs + v - 0x10) + ram[regs.PC + 0x01]];
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
    return ram[ram[regs.PC + 0x01]];
    // nextword
  } else if (v == 0x1f) {
    return ram[regs.PC + 0x01];
  }
  return 0;
}

/* returns the appropriate address depending on the b value of the instruction */
uint8_t *decodeBAddress(uint16_t instruction) {
  return (uint8_t *)&ram;
}


/* instructions routines */ 
/* sets b to a */
void set(uint16_t instruction) {
  *decodeBAddress(instruction) = decodeAValue(instruction);
}

/* sets b to b + a, EX to 1 if overflow, else 0 */
void add(uint16_t instruction) {
  uint16_t opA = decodeAValue(instruction);
  uint16_t opB = decodeBValue(instruction);
  *decodeBAddress(instruction) = opB + opA;
  if (opB + opA > 0xffff) {
    regs.EX = 0x0001;
  } else {
    regs.EX = 0x0000;
  }
}

/* sets b to b - a, EX to 0xffff if underflow, else 0 */
void sub(uint16_t instruction) {
  uint16_t opA = decodeAValue(instruction);
  uint16_t opB = decodeBValue(instruction);
  *decodeBAddress(instruction) = opB - opA;
  if (opA > opB) {
    regs.EX = 0xffff;
  } else {
    regs.EX = 0x0000;
  }
}

/* sets b to a * b (unsigned). EX is set to 16 upper bits of the multiplication */
void mul(uint16_t instruction) {
  uint32_t opA = (uint32_t)decodeAValue(instruction);
  uint32_t opB = (uint32_t)decodeBValue(instruction);
  *decodeBAddress(instruction) = (uint16_t)(opB * opA);
  regs.EX = (uint16_t)((opB * opA) >> 16);
}

/* sets b to a * b (signed). EX is set to 16 upper bits of the multiplication */
void mli(uint16_t instruction) {
  int32_t opA = (int32_t)decodeAValue(instruction);
  int32_t opB = (int32_t)decodeBValue(instruction);
  *decodeBAddress(instruction) = (int16_t)(opB * opA);
  regs.EX = (uint16_t)((opB * opA) >> 16);
}

/* sets b to b / a (unsigned). Sets b to 0 if a is 0 
 * EX is set to 16 lower bits of the division */
void div(uint16_t instruction) {
  uint32_t opA = (uint32_t)decodeAValue(instruction);
  uint32_t opB = (uint32_t)decodeBValue(instruction);
  if (opA == 0) {
    *decodeBAddress(instruction) = 0;
    regs.EX = 0;
  } else {
    *decodeBAddress(instruction) = (uint16_t)(opA / opB);
    regs.EX = (uint16_t)((opA / opB) & 0x0000ffff);
  }
}

/* sets b to b / a (signed). Sets b to 0 if a is 0 
 * EX is set to 16 lower bits of the division */
void dvi(uint16_t instruction) {
  int32_t opA = (int32_t)decodeAValue(instruction);
  int32_t opB = (int32_t)decodeBValue(instruction);
  if (opA == 0) {
    *decodeBAddress(instruction) = 0;
    regs.EX = 0;
  } else {
    *decodeBAddress(instruction) = (int16_t)(opA / opB);
    regs.EX = (uint16_t)((opA / opB) & 0x0000ffff);
  }
}

/* sets b to b%a (unsigned). if a==0, sets b to 0 instead */
void mod(uint16_t instruction) {
  uint16_t opA = decodeAValue(instruction);
  uint16_t opB = decodeBValue(instruction);
  if (opA == 0) {
    *decodeBAddress(instruction) = 0;
  } else {
    *decodeBAddress(instruction) = opB % opA;
  }
}

/* sets b to b%a (signed). if a==0, sets b to 0 instead */
void mdi(uint16_t instruction) {
  int16_t opA = decodeAValue(instruction);
  int16_t opB = decodeBValue(instruction);
  if (opA == 0) {
    *decodeBAddress(instruction) = 0;
  } else {
    *decodeBAddress(instruction) = (uint16_t)(opB % opA);
  }
}

/* sets b to b&a */
void and(uint16_t instruction) {
  *decodeBAddress(instruction) = decodeBValue(instruction) & decodeAValue(instruction);
}

/* sets b to b|a */
void bor(uint16_t instruction) {
  *decodeBAddress(instruction) = decodeBValue(instruction) | decodeAValue(instruction);
}

/* sets b to b^a */
void xor(uint16_t instruction) {
  *decodeBAddress(instruction) = decodeBValue(instruction) ^ decodeAValue(instruction);
}

void shr(uint16_t instruction) {
}

void asr(uint16_t instruction) {
}

void shl(uint16_t instruction) {
}

void ifb(uint16_t instruction) {
}

void ifc(uint16_t instruction) {
}

void ife(uint16_t instruction) {
}

void ifn(uint16_t instruction) {
}

void ifg(uint16_t instruction) {
}

void ifa(uint16_t instruction) {
}

void ifl(uint16_t instruction) {
}

void ifu(uint16_t instruction) {
}

void adx(uint16_t instruction) {
}

void sbx(uint16_t instruction) {
}

void sdi(uint16_t instruction) {
}

void std(uint16_t instruction) {
}

void jsr(uint16_t instruction) {
}

void softInteruption(uint16_t instruction) {
}

void iag(uint16_t instruction) {
}

void ias(uint16_t instruction) {
}

void rfi(uint16_t instruction) {
}

void iaq(uint16_t instruction) {
}

void hwn(uint16_t instruction) {
}

void hwq(uint16_t instruction) {
}

void hwi(uint16_t instruction) {
}
