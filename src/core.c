#include <string.h>
#include <stdint.h>

#include "core.h"

struct {
  uint16_t A; /* General registers */
  uint16_t B;
  uint16_t C;
  uint16_t X;
  uint16_t Y;
  uint16_t Z;
  uint16_t I;
  uint16_t J;
  uint16_t PC; /* Program counter */
  uint16_t SP; /* Stack pointer */
  uint16_t EX; /* Extra/excess register */
  uint16_t IA; /* Interrupt address */
} regs;

void initCore() {
  memset(&regs, 0, 12 * sizeof(uint16_t));
}
