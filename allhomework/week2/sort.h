#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>


void qsort_2_ways(void *a,int left,int right,int size,int(*compare)(void const*,void const*));
void qsort_3_ways(void *a,int left,int right,int size,int(*compare)(void const*,void const*));
void exchx(void *a,int i,int j,int size);
//  Dinh nghia ham.

void exchx(void *a,int i,int j,int size)
{
  void *temp=malloc(size);
  memcpy(temp,(char*)a+i*size,size);
  memcpy((char*)a+i*size,(char*)a+size*j,size);
  memcpy((char*)a+size*j,temp,size);
  free(temp);
}
void qsort_2_ways(void *a,int left,int right,int size,int(*compare)(void const*,void const*))
{
  int i=left,j=right;
  if(left>=right)
    return;
  void *tmp=malloc(size);
  memcpy(tmp,(char*)a+((left+right)/2)*size,size);
  while(i<=j)
    {
      while(compare((char*)a+(i)*size,tmp)<0)
	i++;
      while(compare((char*)a+(j)*size,tmp)>0)
	{
	  if(j==left)
	    break;
	  j--;
	}
      if(i<=j)
	{
	  exchx(a,i,j,size);
	  i++;
	  j--;
	}
    }
  free(tmp);
  qsort_2_ways(a,left,j,size,compare);
  qsort_2_ways(a,i,right,size,compare);
}

void qsort_3_ways(void *a,int left,int right,int size,int(*compare)(void const*,void const*))
{
  int k;
  int i=left-1,j=right;
  int p=left-1,q=right;
  if(right<=left)
    return;
  while(1)
    {
      while(compare((char*)a+(++i)*size,(char*)a+right*size)<0);
      while(compare((char*)a+right*size,(char*)a+(--j)*size)<0) 
      	if(j==left) 
	  break;
      if(i>=j) 
	break;
      exchx(a,i,j,size);
      if(compare((char*)a+i*size,(char*)a+right*size)==0) 
	exchx(a,++p,i,size);
      if(compare((char*)a+j*size,(char*)a+right*size)==0) 
	exchx(a,--q,j,size);
    }
  exchx(a,i,right,size);
  j=i-1;
  i=i+1;
  for(k=left;k<=p;k++) 
    exchx(a,k,j--,size);
  for(k=right-1;k>=q;k--) 
    exchx(a,k,i++,size);
  qsort_3_ways(a,left,j,size,compare);
  qsort_3_ways(a,i,right,size,compare);
}
