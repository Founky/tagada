#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

/* this file contains instruction routines */

#include <stdint.h>

/* instruction format : aaaaaabbbbbooooo */
#define OP_A(X) ((X & 0b1111110000000000) >> 10) /* operande a */ 
#define OP_B(X) ((X & 0b0000001111100000) >>  5) /* operande b */
#define OC(X)   ((X & 0b0000000000011111) >>  0) /* opCode */

/* intructions routines */
void set(uint16_t instruction);
void add(uint16_t instruction);
void sub(uint16_t instruction);
void mul(uint16_t instruction);
void mli(uint16_t instruction);
void div(uint16_t instruction);
void dvi(uint16_t instruction);
void mod(uint16_t instruction);
void mdi(uint16_t instruction);
void and(uint16_t instruction);
void bor(uint16_t instruction);
void xor(uint16_t instruction);
void shr(uint16_t instruction);
void asr(uint16_t instruction);
void shl(uint16_t instruction);
void ifb(uint16_t instruction);
void ifc(uint16_t instruction);
void ife(uint16_t instruction);
void ifn(uint16_t instruction);
void ifg(uint16_t instruction);
void ifa(uint16_t instruction);
void ifl(uint16_t instruction);
void ifu(uint16_t instruction);
void adx(uint16_t instruction);
void sbx(uint16_t instruction);
void sdi(uint16_t instruction);
void std(uint16_t instruction);
void jsr(uint16_t instruction);
void softInteruption(uint16_t instruction);
void iag(uint16_t instruction);
void ias(uint16_t instruction);
void rfi(uint16_t instruction);
void iaq(uint16_t instruction);
void hwn(uint16_t instruction);
void hwq(uint16_t instruction);
void hwi(uint16_t instruction);

#endif
