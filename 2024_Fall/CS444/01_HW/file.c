#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


#define MB (size_t)(1024*1024)
#define GB (size_t)(1024*MB)

void setBuffer(char *buffer){
  for (int i = 0; i < MB; i++)
    buffer[i] = (char)(i%256);

}

int main() {
  printf("Simulating buffer to buffer transfer of 10GB\n");
  char *buffer;
  char *dest;

  buffer = (char*)malloc(MB);
  dest = (char*)malloc(MB);

  setBuffer(buffer);
  printf("Buffer allocated 1 MB\nStarting transfer process\n");
  clock_t timer = clock();
  int mb_transfered;
  for (int i = 0; i < 5000; i++){
    memcpy(dest, buffer, MB);
    memcpy(buffer, dest, MB);
    mb_transfered+=2;

    printf("Transfered: %dMB\n",mb_transfered);
  }

  clock_t elapsed = (clock() - timer) / CLOCKS_PER_SEC;
  printf("Time elapsed for transfering data: %f\n", (double)((10*GB / elapsed) / GB * 8));
  free(buffer);
  free(dest);
}
