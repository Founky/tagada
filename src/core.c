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

void set(uint16_t instruction) {
}
void add(uint16_t instruction) {
}
void sub(uint16_t instruction) {
}
void mul(uint16_t instruction) {
}
void mli(uint16_t instruction) {
}
void div(uint16_t instruction) {
}
void dvi(uint16_t instruction) {
}
void mod(uint16_t instruction) {
}
void mdi(uint16_t instruction) {
}
void and(uint16_t instruction) {
}
void bor(uint16_t instruction) {
}
void xor(uint16_t instruction) {
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
