#include <string.h>
#include <stdint.h>

#include "core.h"

#define RAM_SIZE 0x20000 /* in bytes */

uint8_t ram [RAM_SIZE];


void init() {
  memset(ram, 0, RAM_SIZE);
  initCore();
}

int main () {

  init();
  return 0;
}
