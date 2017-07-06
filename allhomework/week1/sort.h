#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

void swap(int *a, int *b);
void qsort_2_ways(int arr[], int left, int right);


//source code

void swap(int *a, int *b) {
  int tmp;
  tmp = *a;
  *a = *b;
  *b = tmp;
}

void qsort_2_ways(int arr[], int left, int right) {
  int i = left, j = right;
  int tmp;
  int pivot = arr[(left + right) / 2];
 
  /* partition */
  while (i <= j) {
    while (arr[i] < pivot) //arr[i] > pivot, neu can sap xep theo thu tu khong tang.
      i++;
    while (arr[j] > pivot) //arr[j] < pivot, neu can sap xep theo thu tu khong tang.
      j--;
    if (i <= j) {
      tmp = arr[i];
      arr[i] = arr[j];
      arr[j] = tmp;
      i++;
      j--;
    }
  };
 
  /* recursion */
  if (left < j)
    qsort_2_ways(arr, left, j);
  if (i < right)
    qsort_2_ways(arr, i, right);
}


void qsort_3_ways(int a[], int left, int right)
{
  if(left >= right)
    return;
  int i = left , c = right - 1, k;
  int j = right - 1, d = left;
  while(1)
    {
      while(a[i] < a[right]) //a[i]>a[r], neu can sap xep theo thu tu khong tang.
	i++;
      while(a[j] > a[right]) //a[j]<a[r], neu can sap xep theo thu tu khong tang.
	j--;
      if(i >= j)
	break;
      swap(&a[i], &a[j]);
      if(a[i] == a[right])
	{
	  swap(&a[d], &a[i]);
	  d++;
	  i++;
	}
      if(a[j] == a[right])
	{
	  swap(&a[c], &a[j]);
	  c--;
	  j--;
	}
    }
  for(k = left; k<d; k++)
    {
      swap(&a[k], &a[j]);
      j--;
    }
  for(k = right; k>c; k--)
    {
      swap(&a[k], &a[i]);
      i++;
    }
   qsort_3_ways(a, left, j);
   qsort_3_ways(a, i, right);
}
