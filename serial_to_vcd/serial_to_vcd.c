#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BYTETOBINARY(byte)  \
  (byte & 0x80 ? 1 : 0),    \
    (byte & 0x40 ? 1 : 0),  \
    (byte & 0x20 ? 1 : 0),  \
    (byte & 0x10 ? 1 : 0),  \
    (byte & 0x08 ? 1 : 0),  \
    (byte & 0x04 ? 1 : 0),  \
    (byte & 0x02 ? 1 : 0),  \
    (byte & 0x01 ? 1 : 0) 

int main(int argc, char * argv[]){
  printf("$var wire 8 ' data $end\n");
  printf("$enddefinitions $end\n");
  printf("$dumpvars\n");
  printf("bxxxxxxxx '\n");
  printf("$end \n");
  struct timespec start, change;
  clock_gettime(CLOCK_MONOTONIC, &start);
  long time = 0;
  while (1){
    char c = getchar();
    clock_gettime(CLOCK_MONOTONIC, &change);
    time = (change.tv_nsec - start.tv_nsec)/1000;
    printf("# %ld\n", time);
    printf("b%d%d%d%d%d%d%d%d '\n", c & 0x80 ? 1 : 0, c & 0x40 ? 1 : 0, c & 0x20 ? 1 : 0, c & 0x10 ? 1 : 0, c & 0x08 ? 1 : 0, c & 0x04 ? 1 : 0, c & 0x02 ? 1 : 0, c & 0x01 ? 1 : 0); 
  }
  
  return 0;
}
