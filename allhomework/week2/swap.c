#include <stdio.h>
#include <stdlib.h>

int main() {
  
}

void exch(void *buff, int size, int i, int j);
void int_exch(void *buff, int size, int i, int j) {
  int temp;
  temp = *((char *)buff + i*size);
  *((char *)buff + i*size) = *((char *)buff + j*size);
  *((char *)buff + j*size) = temp;                                                                                                                              
}
