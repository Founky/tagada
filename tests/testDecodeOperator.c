#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/Automated.h>
#include <CUnit/CUCurses.h>   /* only on systems having curses */

#include "tests.h"

#include "core.h"
#include "ram.h"
#include "instructions.h"

#define MAGIC 0x10c

/* Declare the private functions to test */
uint16_t decodeAValue(uint16_t instruction);

int initSuiteOperator(void) {
  /* Set the registers */
  regs.A = MAGIC;
  regs.B = 0x00;
  ram[regs.B] = MAGIC;
  regs.PC = 0x01;
  ram[regs.PC + 1] = MAGIC;
  ram[MAGIC] = MAGIC;
  return 0;
}
int cleanSuiteOperator(void) { return 0; }

void testDecodeAValueRegister(void) {
  uint16_t i = SET_OP_A(0x00); /* Set a operand */
  CU_ASSERT_EQUAL(decodeAValue(i), MAGIC);
}

void testDecodeAValueRegisterPointer(void) {
  uint16_t i = SET_OP_A(0x09); /* Set a operand */
  CU_ASSERT_EQUAL(decodeAValue(i), MAGIC);
}

void testDecodeAValueRegisterWordPointer(void) {
  uint16_t i = SET_OP_A(0x11); /* Set a operand */
  CU_ASSERT_EQUAL(decodeAValue(i), MAGIC);
}

void testDecodeAValuePOP(void) {
  uint16_t i = SET_OP_A(0x18); /* Set a operand */
  regs.SP = 0x10;
  ram[regs.SP] = MAGIC;
  CU_ASSERT_EQUAL(decodeAValue(i), MAGIC);
  CU_ASSERT_EQUAL(regs.SP, 0x11);
}

void testDecodeAValuePEEK(void) {
  uint16_t i = SET_OP_A(0x19); /* Set a operand */
  regs.SP = 0x10;
  ram[regs.SP] = MAGIC;
  CU_ASSERT_EQUAL(decodeAValue(i), MAGIC);
  CU_ASSERT_EQUAL(regs.SP, 0x10);
}

extern void initTestInstruction(void) __attribute__ ((constructor));

void init() {

  CU_pSuite pSuite = NULL;

  /* add a suite to the registry */
  pSuite = CU_add_suite("Decode operators", initSuiteOperator, cleanSuiteOperator);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    exit(CU_get_error());
  }

  /* add the tests to the suite */
  if ((NULL == CU_add_test(pSuite, "Decode A value register", testDecodeAValueRegister)) ||
      (NULL == CU_add_test(pSuite, "Decode A value pointer word register", testDecodeAValueRegisterWordPointer)) ||
      (NULL == CU_add_test(pSuite, "Decode A value POP", testDecodeAValuePOP)) ||
      (NULL == CU_add_test(pSuite, "Decode A value PEEK", testDecodeAValuePEEK)) ||
      (NULL == CU_add_test(pSuite, "Decode A value pointer register", testDecodeAValueRegisterPointer)))
  {
    CU_cleanup_registry();
    exit(CU_get_error());
  }
}

void initTestInstruction() {
  addInit(init);
}
