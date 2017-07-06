

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"sort.h"

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

void insertionsort(void *a,int sophantu,int size,int(*compare)(void const *,void const *))
{
  int i,j;
  void *last=malloc(size);
  for(i=1;i<sophantu;i++)
    {
      memcpy(last,(char*)a+i*size,size);
      j=i;
      while(j>=1&&compare((char*)a+(j-1)*size,last)>0)//a[j-1]<last, neu sap xep theo thu tu khong tang.
	{
	  memcpy((char*)a+j*size,(char*)a+(j-1)*size,size);
	  j--;
	}
      memcpy((char*)a+j*size,last,size);
    }
}
void selectionsort(void *a,int sophantu,int size,int(*compare)(void const *,void const *))
{
  int i,j,min;
  for(i=0;i<sophantu-1;i++)
    {
      min=i;
      for(j=i+1;j<sophantu;j++)
	if(compare((char*)a+j*size,(char*)a+min*size)<0)
	  min=j;
      exchx(a,i,min,size);
    }
}

void merge(void *a,void *temp,int left,int mid,int right,int size,int(*compare)(void const *,void const *))// tron
{
  int i,left_end,sophantu,tmp_pos;
  left_end=mid-1; // phan tu cuoi cua nua trai 
  tmp_pos=left;
  sophantu=right-left+1;
  // [left ....left_end]   [   .... right]
  while((left<=left_end)&&(mid<=right))
    {
      if(compare((char*)a+left*size,(char*)a+mid*size)<=0)
      {
	memcpy((char*)temp+tmp_pos*size,(char*)a+left*size,size);
	tmp_pos++;
	left++;
      }
    else
      {
	memcpy((char*)temp+tmp_pos*size,(char*)a+mid*size,size);
	tmp_pos++;
	mid++;
      }
    }
  while(left<=left_end) // sao chep nua trai 
    { 
      memcpy((char*)temp+tmp_pos*size,(char*)a+left*size,size);
      left++;
      tmp_pos++;
    }
  while(mid<=right) //sao chep nua phai 
    {
      memcpy((char*)temp+tmp_pos*size,(char*)a+mid*size,size);
      mid++;
      tmp_pos++;
    }
  for (i=0;i<sophantu;i++) // sao chep 
    {
      memcpy((char*)a+right*size,(char*)temp+right*size,size);
      right--;
    }
}
//ham sap xep tron.
void mergesort(void *a,void *temp,int left,int right,int size,int(*compare)(void const *,void const *))
{
  int mid; // mid la vi tri giua cua mang numbers khi bi chia doi 
  if (right>left)
    {
      mid=(right+left)/2;
      mergesort(a,temp,left,mid,size,compare); //De quy sap xep tron nua trai
      mergesort(a,temp,mid+1,right,size,compare);//De quy sap xep tron nua phai 
      merge(a,temp,left,mid+1,right,size,compare);//tron
    }
}
void shakersort(void *a,int sophantu,int size,int(*compare)(void const *,void const *))
{
  int l=0,j;
  int r=sophantu-1; // lay phan tu gan cuoi 
  int j_last=sophantu-1; // luu chi so lan so sanh cuoi cung
  while(l<r) // lam den khi l > r || l == r
    {
      for(j=r;j>l;j--)
	{
	  if(compare((char*)a+j*size,(char*)a+(j-1)*size)<0)
	    {
	      exchx(a,j,j-1,size);
	      j_last=j; // luu chi so cua j vao j_last
	    }
	}
      l=j_last; // lay gia tri chi so cua j_last dua vao l
      for(j=l;j<r;j++)
	{
	  if(compare((char*)a+j*size,(char*)a+(j+1)*size)>0)
	    {
	      exchx(a,j,j+1,size);
	      j_last=j; // luu gai tri chi so
	    }
	}
      r=j_last; // luu gia tri chi so ra "r" co gia tri moi 
    }
}

void shellsort(void *a,int sophantu,int size,int(*compare)(void const *,void const *))
{
  // _Y Tuong: chon k buoc xap sep <=> chia ra hai day con voi do dai h[k] (thuong chon day so nguyen to) sau do dua tren InsertionSort de sap xep.
  int i,step,len,pos;
  int k=3;
  int h[3]={5,3,1};
  void *tmp=malloc(size);
  for(step=0;step<k;step++)
    {
      len=h[step];
      for(i=len;i<sophantu;i++)
        {
	  memcpy(tmp,(char*)a+i*size,size);
	  pos=i-len;
	  while((pos>=0)&&(compare(tmp,(char*)a+pos*size)<0))
            {
	      memcpy((char*)a+(pos+len)*size,(char*)a+pos*size,size);
	      pos=pos-len;
            }
	  memcpy((char*)a+(pos+len)*size,tmp,size);
        }
    }
  free(tmp);
}

void heapsort(void *a,int sophantu,int size,int(*compare)(void const *,void const *))
{
  int k,s,f;
  void *tmp=malloc(size);
  for(k=1;k<sophantu;k++)
    {
      memcpy(tmp,(char*)a+k*size,size);
      s=k;
      f=(s-1)/2;
      while(s>0&&compare((char*)a+f*size,tmp)<0)
	{
	  memcpy((char*)a+s*size,(char*)a+f*size,size);
	  s=f;
	  f=(s-1)/2;
	}
      memcpy((char*)a+s*size,tmp,size);
    }
  for(k=sophantu-1;k>0;k--)
    {
      memcpy(tmp,(char*)a+k*size,size);
      memcpy((char*)a+k*size,(char*)a,size);
      f=0;
      if(k==1)
	s=-1;
      else
	s=1;
      if(k>2&&compare((char*)a+2*size,(char*)a+size)>0)
	s=2;
      while(s>=0&&compare(tmp,(char*)a+s*size)<0)
	{
	  memcpy((char*)a+f*size,(char*)a+s*size,size);
	  f=s;
	  s=2*f+1;
	  if(s+1<=k-1&&compare((char*)a+s*size,(char*)a+(s+1)*size)<0)
	    s=s+1;
	  if(s>k-1)
	    s=-1;
        }
      memcpy((char*)a+f*size,tmp,size);
    }
  free(tmp);
}
