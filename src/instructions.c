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
    /* ram[SP + nextword] */
  } else if (v == 0x1a) {
    return ram[regs.SP + ram[regs.PC + 0x01]];
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
    return 0x3f - v - 0x01;
  }
  return 0x00;
}

/* returns the appropriate value depending on the b value of the instruction */
uint16_t decodeBValue(uint16_t instruction) {
  uint8_t v = OP_B(instruction);
  /* Registers */
  if (v < 0x08) {
    return *((uint16_t *)&regs + v);
    /* Ram[reg] */
  } else if (v < 0x10) {
    return ram[*((uint16_t *)&regs + v - 0x08)];
    /* Ram[reg + nextword] */
  } else if (v < 0x18) {
    return ram[*((uint16_t *)&regs + v - 0x10) + ram[regs.PC + 0x01]];
    /* PUSH Meaningless for B value */
  } else if (v == 0x18) {
    return 0x00;
    /* Return [SP] */
  } else if (v == 0x19) {
    return ram[regs.SP];
    /* ram[SP + nextword] */
  } else if (v == 0x1a) {
    return ram[regs.SP + ram[regs.PC + 0x01]];
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

/* returns the appropriate address depending on the a value of the instruction */
uint16_t *decodeAAddress(uint16_t instruction) {
  return (uint16_t *)&ram;
}

/* returns the appropriate address depending on the b value of the instruction */
uint16_t *decodeBAddress(uint16_t instruction) {
  uint8_t v = OP_B(instruction);
  /* Registers */
  if (v < 0x08) {
    return (uint16_t *)&regs + v;
    /* Ram[reg] */
  } else if (v < 0x10) {
    return &ram[*((uint16_t *)&regs + v - 0x08)];
    /* Ram[reg + nextword] */
  } else if (v < 0x18) {
    return &ram[*((uint16_t *)&regs + v - 0x10) + ram[regs.PC + 0x01]];
    /* Push : increment SP and return &ram[SP]*/
  } else if (v == 0x18) {
    return &ram[++regs.SP];
    /* Return [SP] */
  } else if (v == 0x19) {
    return &ram[regs.SP];
    /* ram[SP + nextword] */
  } else if (v == 0x1a) {
    return &ram[regs.SP + ram[regs.PC + 0x01]];
    // SP
  } else if (v == 0x1b) {
    return &ram[regs.SP];
    // PC
  } else if (v == 0x1c) {
    return &ram[regs.PC];
    // EX
  } else if (v == 0x1d) {
    return &ram[regs.EX];
    // ram[next word]
  } else if (v == 0x1e) {
    return &ram[ram[regs.PC + 0x01]];
    // nextword
  } else if (v == 0x1f) {
    return &ram[regs.PC + 0x01];
  }
  // Never
  return (uint16_t *)&ram;
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
  *decodeBAddress(instruction) = (opB * opA) & 0x0000ffff;
  regs.EX = (uint16_t)((opB * opA) >> 16);
}

/* sets b to a * b (signed). EX is set to 16 upper bits of the multiplication */
void mli(uint16_t instruction) {
  int32_t opA = (int32_t)decodeAValue(instruction);
  int32_t opB = (int32_t)decodeBValue(instruction);
  *decodeBAddress(instruction) = (uint16_t)(opB * opA) & 0x0000ffff;
  regs.EX = (uint16_t)((opB * opA) >> 16);
}

/* divides b by a, rounds down to the nearest whole number 
 * and stores the result in b. 
 * The fractional part of the result is stored in the EX register */
void div(uint16_t instruction) {
  uint32_t opA = (uint32_t)decodeAValue(instruction);
  uint32_t opB = (uint32_t)decodeBValue(instruction);
  if (opA == 0) {
    *decodeBAddress(instruction) = 0;
    regs.EX = 0;
  } else {
    *decodeBAddress(instruction) = (uint16_t)(opB / opA);
    regs.EX = (uint16_t)(((opB << 16) / opA) & 0x0000ffff);
  }
}

/* divides b by a (signed), rounds down to the nearest whole number 
 * and stores the result in b. 
 * The fractional part of the result is stored in the EX register */
void dvi(uint16_t instruction) {
  int32_t opA = (int32_t)decodeAValue(instruction);
  int32_t opB = (int32_t)decodeBValue(instruction);
  if (opA == 0) {
    *decodeBAddress(instruction) = 0;
    regs.EX = 0;
  } else {
    *decodeBAddress(instruction) = (uint16_t)(opB / opA);
    regs.EX = (uint16_t)(((opB << 16)/ opA) & 0x0000ffff);
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

/* performs next instruction only if (b&a)!=0 */
void ifb(uint16_t instruction) {
  if (!((decodeBValue(instruction) & decodeAValue(instruction)) != 0)) {
   regs.PC += 1;
  } 
}

/* performs next instruction only if (b&a)==0 */
void ifc(uint16_t instruction) {
  if (!((decodeBValue(instruction) & decodeAValue(instruction)) == 0)) {
   regs.PC += 1;
  }
}

/* performs next instruction only if b==a */
void ife(uint16_t instruction) {
  if (!(decodeBValue(instruction) == decodeAValue(instruction))) {
   regs.PC += 1;
  }
}

/* performs next instruction only if b!=a */
void ifn(uint16_t instruction) {
  if (!(decodeBValue(instruction) != decodeAValue(instruction))) {
   regs.PC += 1;
  }
}

/* performs next instruction only if b>a */
void ifg(uint16_t instruction) {
  if (!(decodeBValue(instruction) > decodeAValue(instruction))) {
   regs.PC += 1;
  }
}

/* performs next instruction only if b>a (signed) */
void ifa(uint16_t instruction) {
  if (!((int16_t)(decodeBValue(instruction)) > (int16_t)(decodeAValue(instruction)))) {
   regs.PC += 1;
  }
}

/* performs next instruction only if b<a */
void ifl(uint16_t instruction) {
  if (!(decodeBValue(instruction) < decodeAValue(instruction))) {
   regs.PC += 1;
  }
}

/* performs next instruction only if b<a (signed) */
void ifu(uint16_t instruction) {
  if (!((int16_t)(decodeBValue(instruction)) < (int16_t)(decodeAValue(instruction)))) {
   regs.PC += 1;
  }
}

/* sets b to b+a+EX, sets EX to 0x0001 if there is an overflow, 0x0 otherwise */
void adx(uint16_t instruction) {
  uint16_t opA = decodeAValue(instruction);
  uint16_t opB = decodeBValue(instruction);
  *decodeBAddress(instruction) = opB + opA + regs.EX;
  if (opB + opA + regs.EX > 0xffff) {
    regs.EX = 0x0001;
  } else {
    regs.EX = 0x0000;
  }
}

/* sets b to b-a-EX, sets EX to 0xffff if there is an underflow, 0x0 otherwise */
void sbx(uint16_t instruction) {
  uint16_t opA = decodeAValue(instruction);
  uint16_t opB = decodeBValue(instruction);
  *decodeBAddress(instruction) = opB - opA - regs.EX;
  if (opA + regs.EX > opB) {
    regs.EX = 0xffff;
  } else {
    regs.EX = 0x0000;
  }
}

/* sets b to a, then increases I and J by 1*/
void sdi(uint16_t instruction) {
  *decodeBAddress(instruction) = decodeAValue(instruction);
  regs.I += 1;
  regs.J += 1;
}

/* sets b to a, then decreases I and J by 1*/
void std(uint16_t instruction) {
  *decodeBAddress(instruction) = decodeAValue(instruction);
  regs.I -= 1;
  regs.J -= 1;
}

/* pushes the address of the next instruction to the stack,
 * then sets PC to a */
void jsr(uint16_t instruction) {
  ram[--regs.SP] = regs.PC + 1;
  regs.PC = decodeAValue(instruction);
}

void softInteruption(uint16_t instruction) {
}

/* sets a to IA */
void iag(uint16_t instruction) {
  *decodeAAddress(instruction) = regs.IA;
}

/* sets IA to a */
void ias(uint16_t instruction) {
  regs.IA = decodeAValue(instruction);
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
