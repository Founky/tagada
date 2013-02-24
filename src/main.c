#include <string.h>
#include <stdint.h>

#include "core.h"
#include "ram.h"

void init() {
  memset(ram, 0, RAM_SIZE);
  initCore();
}

int main() {
  init();
  run();
  return 0;
}
