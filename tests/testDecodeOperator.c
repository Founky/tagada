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

int init_suite_success(void) {
  /* Set the registers */
  regs.A = MAGIC;
  regs.B = 0x00;
  ram[regs.B] = MAGIC;
  return 0;
}
int clean_suite_success(void) { return 0; }

void testDecodeAValueRegister(void) {
  uint16_t i = SET_OP_A(0x00); /* Set a operand */
  decodeAValue(i);
  CU_ASSERT_EQUAL(decodeAValue(i), MAGIC);
}

void testDecodeAValueRegisterPointer(void) {
  uint16_t i = SET_OP_A(0x00); /* Set a operand */
  decodeAValue(i);
  CU_ASSERT_EQUAL(decodeAValue(i), MAGIC);
}

void test_success2(void) {
  CU_ASSERT_NOT_EQUAL(2, -1);
}

void test_success3(void) {
  CU_ASSERT_STRING_EQUAL("string #1", "string #1");
}

void test_success4(void) {
  CU_ASSERT_STRING_NOT_EQUAL("string #1", "string #2");
}

extern void initTestInstruction(void) __attribute__ ((constructor));

void init() {
  CU_pSuite pSuite = NULL;

  /* add a suite to the registry */
  pSuite = CU_add_suite("Decode operators", init_suite_success, clean_suite_success);
  if (NULL == pSuite) {
     CU_cleanup_registry();
       exit(CU_get_error());
  }

  /* add the tests to the suite */
  if ((NULL == CU_add_test(pSuite, "Decode registers", testDecodeAValueRegister)) ||
      (NULL == CU_add_test(pSuite, "successful_test_2", testDecodeAValueRegisterPointer)) ||
      (NULL == CU_add_test(pSuite, "successful_test_3", test_success3)))
  {
     CU_cleanup_registry();
     exit(CU_get_error());
  }
}

void initTestInstruction() {
  addInit(init);
}
