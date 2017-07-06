#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#include"sort_int.h"

//dieu kien hai mang A,B da duoc sap xep theo cung thu tu khong giam.
// mang A co m phan tu, mang B co n phan tu. Mang C la mang ket qua co (m+n) phan tu..
void gopmang(int m, int n, int A[], int B[], int C[])
{
  int i = 0, j = 0;
  int k = 0,p;
  while (i<m&&j<n)
    {
      if(A[i]<B[j])//A[i]>=B[j]
	{
	  C[k]=A[i];
	  i++;
	}
      else
	{
	  C[k] = B[j];
	  j++;
	}
      k++;
    }
  if(i<m)
    for(p=i;p<m;p++)
      {
	C[k]=A[p];
	k++;
      }
  else
    for(p=j;p<n;p++)
      {
	C[k]=B[p];
	k++;
      }
}


//loai bo cac gia tri giong nhau trong mang, tra ve so phan tu cua mang sau khi loai bo.
// yeu cau mang da duoc sap xep.
long int loaibo(int a[],long int n)
{
  long int i=0,j;
  //sapxepchen(a,n);
  while(1)
    {
      if(i>=n)
	break;
      if(a[i]==a[i+1])
	{
	  for(j=i+1;j<n;j++)
	    a[j]=a[j+1];
	  n--;
	}
      else
	i++;
    }
  return n;
}
//ham tra ve chi so cua phan tu dung truoc trong hai phan tu co hieu nho nhat.
// yeu cau mang da duoc sap xep theo thu tu ko giam. 
long int chiso(int a[],long int n)
{
  long int i,k;
  int min;
  min=a[1]-a[0];
  for(i=0;i<n-1;i++)
    if(min>(a[i+1]-a[i]))
      {
	min=a[i+1]-a[i];
	k=i;
      }
  return k;
}
// ham dua ra bo ba cap so cong trong mang.
// yeu cau mang da duoc sap xep theo thu tu khong giam.
void capsocong(int a[],long int n)
{
  long int i,j,k;
  int key;
  for(i=0;i<n-2;i++)
    for(j=i+1;j<n-1;j++)
      {
	key=2*a[j]-a[i];
	for(k=j+1;k<n;k++)
	  if(key==a[k])
	    printf("Bo ba cap so cong: %d %d %d.\n",a[i],a[j],a[k]);
      }
}
main()
{
  clock_t t1, t2;
  int a[MAX];
  long int i,j;
  srand(time(NULL));
  for(i=0;i<MAX;i++)
    {
      a[i]=rand()%1000;
      printf("%2d ",a[i]);
    }
  printf("\n\n");
  t1=clock();
  // insertion_sort(a,MAX);
  // selection_sort(a,MAX);
  // merge_sort(a,temp,0,MAX-1);
  // randomizedquicksort(a,&n);
  // ShakerSort(a,MAX);
  // sort(a,0,MAX-1);
  // quicksort(a,0,MAX-1);
  // Heapsort(a,MAX);
  // ShellSort(a,MAX);
  radixsort(a,MAX);
  t2=clock();
  printf("\nThoi gian sap xep la : %10.4lf s.\n",(double)(t2-t1)/CLOCKS_PER_SEC);
  printf("\n  Mang sau khi sap xep :\n");
  for(i=0;i<MAX;i++)
    printf("%2d ",a[i]);
  printf("\n");
  /* int k=chiso(a,MAX);
     printf("hai phan tu gan nhau nhat la: %d %d.\n",a[k],a[k+1]);
     printf("\nMang sau khi loai bo cac phan tu giong nhau:\n");
     int max=loaibo(a,MAX);
     for(i=0;i<max;i++)
     printf("%2d ",a[i]);
     printf("\n");
  */
  return 0;
}
