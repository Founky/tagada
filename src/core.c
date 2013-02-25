#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

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
    printf("----- NEW INSTRUCTION -----\n");
    printf("%04x -- o: %x, b: %x, a: %x\n", instruction, OC(instruction), OP_B(instruction) , OP_A(instruction));
    printRam(0,4);
    printRegisters();
    switch (opCode) {
      case 0x00 : 
        /* special instruction */
        specialInstruction(instruction);
        break;
      case 0x01 :
        /* Set b, a */
        setInst(instruction);
        break;
      case 0x02 : 
        /* Add b, a */
        addInst(instruction);
        break;
      case 0x03 :
        /* Sub b, a */
        subInst(instruction);
        break;
      case 0x04 :
        /* Mul b, a */
        mulInst(instruction);
        break;
      case 0x05 :
        /* Mli b, a */
        mliInst(instruction);
        break;
      case 0x06 :
        /* Div b, a */
        divInst(instruction);
        break;
      case 0x07 :
        /* Dvi b, a */
        dviInst(instruction);
        break;
      case 0x08 :
        /* Mod b, a*/
        modInst(instruction);
        break;
      case 0x09 :
        /* Mdi b, a*/
        mdiInst(instruction);
        break;
      case 0x0a :
        /* And b, a */
        andInst(instruction);
        break;
      case 0x0b :
        /* Bor b, a */
        borInst(instruction);
        break;
      case 0x0c :
        /* Xor b, a */
        xorInst(instruction);
        break;
      case 0x0d :
        /* Shr b, a */
        shrInst(instruction);
        break;
      case 0x0e :
        /* Asr b, a */
        asrInst(instruction);
        break;
      case 0x0f :
        /* Shl b, a */
        shlInst(instruction);
        break;
      case 0x10 :
        /* Ifb b, a */
        ifbInst(instruction);
        break;
      case 0x11 :
        /* Ifc b, a */
        ifcInst(instruction);
        break;
      case 0x12 :
        /* Ife b, a */
        ifeInst(instruction);
        break;
      case 0x13 :
        /* Ifn b, a */
        ifnInst(instruction);
        break;
      case 0x14 :
        /* Ifg b, a */
        ifgInst(instruction);
        break;
      case 0x15 :
        /* Ifa b, a */
        ifaInst(instruction);
        break;
      case 0x16 :
        /* Ifl b, a */
        iflInst(instruction);
        break;
      case 0x17 :
        /* Ifu b, a */
        ifuInst(instruction);
        break;
      case 0x18 :
        break;
      case 0x19 :
        break;
      case 0x1a :
        /* Adx b, a */
        adxInst(instruction);
        break;
      case 0x1b :
        /* Sbx b, a */
        sbxInst(instruction);
        break;
      case 0x1c :
        break;
      case 0x1d :
        break;
      case 0x1e :
        /* Sdi b, a */
        sdiInst(instruction);
        break;
      case 0x1f :
        /* Std b, a */
        stdInst(instruction);
        break;
      default :
        printf("Unknown opCode : %x\nExiting\n", opCode);
        exit(1);
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
      jsrInst(instruction);
      break;
    case 0x08 :
      /* Int a */
      intInst(instruction);
      break;
    case 0x09 :
      /* Iag a */
      iagInst(instruction);
      break;
    case 0x0a :
      /* ias a */
      iasInst(instruction);
      break;
    case 0x0b :
      /* rfi a */
      rfiInst(instruction);
      break;
    case 0x0c :
      /* iaq a */
      iaqInst(instruction);
      break;
    case 0x10 :
      /* hwn a */
      hwnInst(instruction);
      break;
    case 0x11 :
      /* hwq a */
      hwqInst(instruction);
      break;
    case 0x12 :
      /* hwi a */
      hwiInst(instruction);
      break;
    default :
      printf("Unknown opCode : %x\nExiting\n", opCode);
      exit(1);
      break;
  }
}

void printRegisters() {
  printf("A :%04x B :%04x C :%04x X :%04x\n", regs.A, regs.B, regs.C, regs.X);
  printf("Y :%04x Z :%04x I :%04x J :%04x\n", regs.Y, regs.Z, regs.I, regs.J);
  printf("PC:%04x SP:%04x EX:%04x IA:%04x\n", regs.PC, regs.SP, regs.EX, regs.IA);
}
