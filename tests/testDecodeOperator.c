#include <stdio.h>
#include <stdlib.h>

#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/Automated.h>
#include <CUnit/CUCurses.h>   /* only on systems having curses */

#include "tests.h"

int init_suite_success(void) { return 0; }
int clean_suite_success(void) { return 0; }

void test_success1(void) {
   CU_ASSERT(1);
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
  pSuite = CU_add_suite("Suite_success", init_suite_success, clean_suite_success);
  if (NULL == pSuite) {
     CU_cleanup_registry();
       exit(CU_get_error());
  }

  /* add the tests to the suite */
  if ((NULL == CU_add_test(pSuite, "successful_test_1", test_success1)) ||
      (NULL == CU_add_test(pSuite, "successful_test_2", test_success2)) ||
      (NULL == CU_add_test(pSuite, "successful_test_3", test_success3)))
  {
     CU_cleanup_registry();
     exit(CU_get_error());
  }
}

void initTestInstruction() {
  addInit(init);
}
