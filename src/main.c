#include <string.h>
#include <stdint.h>

#include "core.h"
#include "ram.h"

void init(char * filePath) {
  initRam(filePath);
  initCore();
}

int main(int argc, char * argv[]) {
  if (argc == 2) {
    init(argv[1]);
  } else {
    init(NULL);
  }
  run();
  return 0;
}
