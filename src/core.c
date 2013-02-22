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

void run() {
  uint16_t instruction = 0x0;
  uint8_t opCode = 0x0;
  while (1) {
    instruction = ram[regs.PC];
    opCode = OC(instruction);
    switch (opCode) {
      case 0x00 : 
        /* special instruction */
        break;
      case 0x01 :
        /* Set b, a */
        set(instruction);
        break;
      case 0x02 : 
        /* Add b, a */
        add(instruction);
        break;
      case 0x03 :
        /* Sub b, a */
        sub(instruction);
        break;
      case 0x04 :
        /* Mul b, a */
        mul(instruction);
        break;
      case 0x05 :
        /* Mli b, a */
        mli(instruction);
        break;
      case 0x06 :
        /* Div b, a */
        div(instruction);
        break;
      case 0x07 :
        /* Dvi b, a */
        dvi(instruction);
        break;
      case 0x08 :
        /* Mod b, a*/
        mod(instruction);
        break;
      case 0x09 :
        /* Mdi b, a*/
        mdi(instruction);
        break;
      case 0x0A :
        /* And b, a */
        and(instruction);
        break;
      case 0x0B :
        /* Bor b, a */
        bor(instruction);
        break;
      case 0x0C :
        /* Xor b, a */
        xor(instruction);
        break;
      case 0x0D :
        /* Shr b, a */
        shr(instruction);
        break;
      case 0x0E :
        /* Asr b, a */
        asr(instruction);
        break;
      case 0x0F :
        /* Shl b, a */
        shl(instruction);
        break;
      case 0x10 :
        /* Ifb b, a */
        ifb(instruction);
        break;
      case 0x11 :
        /* Ifc b, a */
        ifc(instruction);
        break;
      case 0x12 :
        /* Ife b, a */
        ife(instruction);
        break;
      case 0x13 :
        /* Ifn b, a */
        ifn(instruction);
        break;
      case 0x14 :
        /* Ifg b, a */
        ifg(instruction);
        break;
      case 0x15 :
        /* Ifa b, a */
        ifa(instruction);
        break;
      case 0x16 :
        /* Ifl b, a */
        ifl(instruction);
        break;
      case 0x17 :
        /* Ifu b, a */
        ifu(instruction);
        break;
      case 0x18 :
        break;
      case 0x19 :
        break;
      case 0x1A :
        /* Adx b, a */
        adx(instruction);
        break;
      case 0x1B :
        /* Sbx b, a */
        sbx(instruction);
        break;
      case 0x1C :
        break;
      case 0x1D :
        break;
      case 0x1E :
        /* Sdi b, a */
        sdi(instruction);
        break;
      case 0x1F :
        /* Std b, a */
        std(instruction);
        break;
    }
  }
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

uint8_t *decodeAAddress(uint16_t instruction) {
  return (uint8_t *)&ram;
}

uint8_t *decodeBAddress(uint16_t instruction) {
  return (uint8_t *)&ram;
}

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
  /* TODO: Implement mod for signed numbers */
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
