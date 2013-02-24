#ifndef __INSTRUCTIONS_H__
#define __INSTRUCTIONS_H__

/* this file contains instruction routines */

#include <stdint.h>

/* instruction format : aaaaaabbbbbooooo */
#define OP_A(X) ((X & 0b1111110000000000) >> 10) /* operande a */ 
#define OP_B(X) ((X & 0b0000001111100000) >>  5) /* operande b */
#define OC(X)   ((X & 0b0000000000011111) >>  0) /* opCode */

#define SET_OP_A(X) (X << 10) /* operande a */ 
#define SET_OP_B(X) (X << 5) /* operande b */
#define SET_OC(X)   (X <<  0) /* opCode */

/* intructions routines */
void setInst(uint16_t instruction);
void addInst(uint16_t instruction);
void subInst(uint16_t instruction);
void mulInst(uint16_t instruction);
void mliInst(uint16_t instruction);
void divInst(uint16_t instruction);
void dviInst(uint16_t instruction);
void modInst(uint16_t instruction);
void mdiInst(uint16_t instruction);
void andInst(uint16_t instruction);
void borInst(uint16_t instruction);
void xorInst(uint16_t instruction);
void shrInst(uint16_t instruction);
void asrInst(uint16_t instruction);
void shlInst(uint16_t instruction);
void ifbInst(uint16_t instruction);
void ifcInst(uint16_t instruction);
void ifeInst(uint16_t instruction);
void ifnInst(uint16_t instruction);
void ifgInst(uint16_t instruction);
void ifaInst(uint16_t instruction);
void iflInst(uint16_t instruction);
void ifuInst(uint16_t instruction);
void adxInst(uint16_t instruction);
void sbxInst(uint16_t instruction);
void sdiInst(uint16_t instruction);
void stdInst(uint16_t instruction);
void jsrInst(uint16_t instruction);
void intInst(uint16_t instruction);
void iagInst(uint16_t instruction);
void iasInst(uint16_t instruction);
void rfiInst(uint16_t instruction);
void iaqInst(uint16_t instruction);
void hwnInst(uint16_t instruction);
void hwqInst(uint16_t instruction);
void hwiInst(uint16_t instruction);

#endif
