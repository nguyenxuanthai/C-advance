#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sort.h"
#define max 10

int int_compare(void const* x, void const *y) {
  int m, n;
  m = *((int*)x);
  n = *((int*)y);
  if ( m == n ) return 0;
  return m > n ? 1: -1;
}

int main() {
  int i;
  clock_t start, end;
  int a1[max], a2[max], a3[max];
  srand(time(NULL));
  for(i = 0; i < max; i++)
    {
      a1[i] = rand()%10;
      a2[i] = a1[i];
      a3[i] = a1[i];
      //      printf("a1 %2d ", a1[i]);
    }
  //    printf("\n\n");

  start = clock();
  qsort(a3, max, sizeof(int), int_compare);
  end = clock();
  /*   for (i = 0; i < max; i++) {
    printf("%2d\n", a3[i]);
    }*/
  printf("qsort in stdlib.h run in %f seconds.\n", (double)(end - start)/CLOCKS_PER_SEC);
  
  start = clock();
  qsort_2_ways(a1, 0, max - 1);
  end = clock();
  printf("qsort 2 ways run in %.9f seconds.\n", (double)(end - start)/CLOCKS_PER_SEC);
  /*     for (i = 0; i < max; i++) {
    printf("%2d\n", a1[i]);
    }*/

  start = clock();
  qsort_3_ways(a2, 0, max - 1);
  end = clock();
  printf("qsort 3 ways run in %f seconds.\n", (double)(end - start)/CLOCKS_PER_SEC);
}
