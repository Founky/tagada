#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "core.h"
#include "ram.h"
#include "instructions.h"


struct registerFile regs;
uint8_t intQueing;

void initCore() {
  /* note : when set to 0,
   * SP virtually points to ram[RAM_SIZE] */
  memset(&regs, 0, 12 * sizeof(uint16_t));
  intQueing = 0;
}

void specialInstruction(uint16_t instruction);

void run() {
  uint16_t instruction = 0x0;
  uint8_t opCode = 0x0;
  while (1) {
    instruction = ram[regs.PC];
    opCode = OC(instruction);
    switch (opCode) {
      case 0x00 : 
        /* special instruction */
        specialInstruction(instruction);
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
      case 0x0a :
        /* And b, a */
        and(instruction);
        break;
      case 0x0b :
        /* Bor b, a */
        bor(instruction);
        break;
      case 0x0c :
        /* Xor b, a */
        xor(instruction);
        break;
      case 0x0d :
        /* Shr b, a */
        shr(instruction);
        break;
      case 0x0e :
        /* Asr b, a */
        asr(instruction);
        break;
      case 0x0f :
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
      case 0x1a :
        /* Adx b, a */
        adx(instruction);
        break;
      case 0x1b :
        /* Sbx b, a */
        sbx(instruction);
        break;
      case 0x1c :
        break;
      case 0x1d :
        break;
      case 0x1e :
        /* Sdi b, a */
        sdi(instruction);
        break;
      case 0x1f :
        /* Std b, a */
        std(instruction);
        break;
      default :
        printf("Unknown opCode : %x\n", opCode);
        break;
    }
    regs.PC++;
  }
}

/* calls the appropriate special instruction routine */
void specialInstruction(uint16_t instruction) {
  /* special instruction format : aaaaaaooooo00000 */
  uint8_t opCode = OP_B(instruction);
  switch (opCode) {
    case 0x01 :
      /* Jsr a */
      jsr(instruction);
      break;
    case 0x08 :
      /* Int a */
      softInteruption(instruction);
      break;
    case 0x09 :
      /* Iag a */
      iag(instruction);
      break;
    case 0x0a :
      /* ias a */
      ias(instruction);
      break;
    case 0x0b :
      /* rfi a */
      rfi(instruction);
      break;
    case 0x0c :
      /* iaq a */
      iaq(instruction);
      break;
    case 0x10 :
      /* hwn a */
      hwn(instruction);
      break;
    case 0x11 :
      /* hwq a */
      hwq(instruction);
      break;
    case 0x12 :
      /* hwi a */
      hwi(instruction);
      break;
    default :
      printf("Unknown opCode : %x\n", opCode);
      break;
  }
}
