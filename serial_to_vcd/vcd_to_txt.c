#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char * argv[]){
  int pid;
  float idle, busy, user;
  
  FILE* fid;
  fpos_t pos;
  int pos_init = 0;

  while (1)  {
    // try to open the file
    if ((fid = fopen("t.txt","rw+")) == NULL) {
      perror("Cant open file\n");
      sleep(1); // sleep for a little while, and try again
      continue; 
    }
    
    // reset position in file (if initialized)
    if (pos_init) 
      fsetpos (fid, &pos);
    
    // read as many line as possible
    while(1) {
      printf("e");
      char c = getchar();

      printf("%c\n", c);
      
      fgetpos (fid, &pos); // remember current position 
      pos_init = 1; // position has been initialized 
    } 
    printf("out\n");
    fclose(fid);
  }
}
