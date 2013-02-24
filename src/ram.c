#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "ram.h"

uint16_t ram [RAM_SIZE];

void loadProgram(char * filePath) {

  FILE * pf = fopen(filePath, "rb");

  if (pf == NULL) {
    perror("can't open file ");
  }

  /* Check file size */
  fseek(pf , 0 , SEEK_END);
  uint32_t lSize = ftell(pf);
  rewind(pf);
  /* printf("Filesize: %d\n", lSize); */

  /* Allocate memory to contain the whole file */
  uint16_t * buf = (uint16_t*)malloc(sizeof(char) * lSize);
  if (buf == NULL) {
    fputs ("Memory error", stderr); 
    return;
  }

  /* Copy the file into the buf */
  size_t result = fread (buf, 1, lSize, pf);
  if (result != lSize) {
    fputs("Reading error",stderr); 
    return;
  }

  uint32_t i = 0;
  /* Copy buf to ram */
  for(i = 0; i < lSize; ++i) {
    ram[i] = buf[i];
  }

  /* Close file, free buf */
  fclose(pf);
  free(buf);

}

void printRam(uint32_t start, uint32_t end) {
  int i = 0;
  for (i = 0; i < end; i+=4) {
    printf("%04x: %04x ",i + start + 0, ram[i + start + 0]);
    printf("%04x ", ram[i + start + 1]);
    printf("%04x ", ram[i + start + 2]);
    printf("%04x ", ram[i + start + 3]);
    printf("\n");
  }
}

void initRam(char * filePath) {
  memset(ram, 0, RAM_SIZE);
  if (filePath != NULL) {
    loadProgram(filePath);
  }
}
