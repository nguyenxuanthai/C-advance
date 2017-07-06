#include <stdio.h>
#include "jval.h"

int main(int argc, char *argv[]) {
  Jval a;
  a = new_jval_i(5);
  printf("%d\n", jval_i(a));
  return 0;
}
