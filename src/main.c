#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define RAM_SIZE 0x20000 /* in bytes */

uint8_t ram [RAM_SIZE];

struct {
  uint16_t A;
  uint16_t B;
  uint16_t C;
  uint16_t X;
  uint16_t Y;
  uint16_t Z;
  uint16_t I;
  uint16_t J;
} regs;

void init() {
  memset(ram, 0, RAM_SIZE);
  memset(&regs, 0, 8 * sizeof(uint16_t));
}

int main () {

  init();
  return 0;
}
