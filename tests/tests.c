#include <stdio.h>
#include <stdlib.h>

#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <CUnit/Automated.h>
#include <CUnit/CUCurses.h>   /* only on systems having curses */

#include "tests.h"

int nbInits = 0;

void (*inits[INIT_SIZE])(void);

void addInit(void (*init)(void)) {
  inits[nbInits] = init;
  nbInits++;
}

int main() {
  /* initialize the CUnit test registry */
  if (CUE_SUCCESS != CU_initialize_registry()) {
    exit(CU_get_error());
  }

  /* Run all the inits */
  int i;
  for (i = 0; i < nbInits; i++) {
    inits[i]();
  }
  
  /* Run all tests using the curses interface */
  /* (only on systems having curses) */
  CU_curses_run_tests();

  /* Clean up registry and return */
  CU_cleanup_registry();

  return CU_get_error();
}
