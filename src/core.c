#include <string.h>
#include <stdint.h>

#include "core.h"

void initCore() {
  memset(&regs, 0, 8 * sizeof(uint16_t));
}
