#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<string.h>
#define MAX 3000

void swap(int *a,int *b);
//void merge(int numbers[], int temp[], int left, int mid, int right);//tron
void merge_sort(int a[], int tmp[], int dau, int cuoi);// Sắp xếp trộn.

void insertion_sort(int a[],long int sophantu); // Sắp xếp chèn.
void selection_sort(int a[],long int sophantu); // Sắp xếp lựa chọn.
void quicksort(int a[],long int l, long int r); // Sắp xếp nhanh.
void sort(int a[],long int l,long int r); // quick sort 3 duong.
void ShakerSort(int a[],int n);// Sắp xếp sàng.
void ShellSort(int a[],int n);// Sắp xếp vỏ sò.
void Heapsort(int a[],int n);// Sắp xếp vun đống.
void radixsort(int a[],int n);// Sắp xếp theo cơ số.
//void RQSWithRange (int a[],int l,int r);
void randomizedquicksort(int a[],int *n);

// Định nghĩa hàn.
void swap(int *a,int *b)
{
  int tmp;
  tmp=*a;
  *a=*b;
  *b=tmp;
}

// sap xep tron merge_sort(.....);
// numbers[] la mang can sap xep.left, right la chi so phan tu dau tien va cuoi cung cua mang.
void merge(int numbers[], int temp[], int left, int mid, int right)// tron
{
  int i, left_end, num_elements, tmp_pos;
  left_end = mid - 1; // phan tu cuoi cua nua trai 
  tmp_pos = left;
  num_elements = right - left + 1;
  // [left ....left_end]   [    .... right]
  while ((left <= left_end) && (mid <= right))
  {
    if (numbers[left]<=numbers[mid])//numbers[left]>numbers[mid].
      {
	temp[tmp_pos] = numbers[left];
	tmp_pos = tmp_pos + 1;
	left = left +1;
      }
    else
      {
	temp[tmp_pos] = numbers[mid];
	tmp_pos = tmp_pos + 1;
	mid = mid + 1;
      }
  }
  while (left <= left_end) // sao chep nua trai 
    { 
      temp[tmp_pos] = numbers[left];
      left = left + 1;
      tmp_pos = tmp_pos + 1;
    }
  while (mid <= right) //sao chep nua phai 
    {
      temp[tmp_pos] = numbers[mid];
      mid = mid + 1;
      tmp_pos = tmp_pos + 1;
    }
  for (i=0;i<=num_elements;i++) // sao chep 
    {
      numbers[right] = temp[right];
      right = right - 1;
    }
}
//ham sap xep tron.
void merge_sort(int numbers[], int temp[], int left, int right)
{
  int mid; // mid la vi tri giua cua mang numbers khi bi chia doi 
  if (right>left)
    {
      mid=(right + left)/2;
      merge_sort(numbers,temp,left,mid); // De quy sap xep tron nua trai
      merge_sort(numbers,temp,mid+1,right); // De quy sap xep tron nua phai 
      merge(numbers,temp,left,mid+1,right);
    }
}

void insertion_sort(int a[],long int sophantu)
{
  long int i,j;
  int last;
  for(i=1;i<sophantu;i++)
    {
      last=a[i];
      j=i;
      while(j>=1&&a[j-1]>last)//a[j-1]<last, neu sap xep theo thu tu khong tang.
	{
	  a[j]=a[j-1];
	  j--;
	}
      a[j]=last;
    }
}
void selection_sort(int a[],long int sophantu)
{
  long int i,j,min;
  for(i=0;i<sophantu-1;i++)
    {
      min=i;
      for(j=i+1;j<sophantu;j++)
	if(a[j]<a[min])// a[j]>a[min], neu theo thu tu khong tang.
	  min=j;
      swap(&a[i],&a[min]);
    }
}
//sort 2 phan sap xep mang theo thu tu khong giam.
void quicksort(int a[],long int l, long int r)
{
  if(l>=r)
    return;
  long int i=l;
  long int j=r;
  int x=a[(l+r)/2];
  while(i<=j)
    {
      while(a[i]<x)//a[i]>x, neu can sap xep theo thu tu khong tang.
	i++;
      while(a[j]>x)//a[j]<x, neu can sap xep theo thu tu khong tang.
	j--;
      if(i<=j)
	{
	  swap(&a[i],&a[j]);
	  i++;
	  j--;
	}
    }
  quicksort(a,l,j);
  quicksort(a,i,r);
}
// sort 3 phan sap xep mang theo thu tu khong giam
void sort(int a[],long int l,long int r)
{
  if(l>=r)
    return;
  long int i=l,c=r-1,k;
  long int j=r-1,d=l;
  while(1)
    {
      while(a[i]<a[r])//a[i]>a[r], neu can sap xep theo thu tu khong tang.
	i++;
      while(a[j]>a[r])//a[j]<a[r], neu can sap xep theo thu tu khong tang.
	j--;
      if(i>=j)
	break;
      swap(&a[i],&a[j]);
      if(a[i]==a[r])
	{
	  swap(&a[d],&a[i]);
	  d++;
	  i++;
	}
      if(a[j]==a[r])
	{
	  swap(&a[c],&a[j]);
	  c--;
	  j--;
	}
    }
  for(k=l;k<d;k++)
    {
      swap(&a[k],&a[j]);
      j--;
    }
  for(k=r;k>c;k--)
    {
      swap(&a[k],&a[i]);
      i++;
    }
   sort(a,l,j);
   sort(a,i,r);
}
void ShakerSort(int a[],int n)
{
  int l = 0,j;
  int r = n - 1; // lay phan tu gan cuoi 
  int j_last = n - 1; // luu chi so lan so sanh cuoi cung
  while(l<r) // lam den khi l > r || l == r
    {
      for(j =r;j>l;j--)
	{
	  if(a[j]<a[j-1])
	    {
	      swap(&a[j-1],&a[j]);
	      j_last=j; // luu chi so cua j vao j_last
	    }
	}
      l = j_last; // lay gia tri chi so cua j_last dua vao l
      for(j=l;j<r;j++)
	{
	  if(a[j]>a[j+1])
	    {
	      swap(&a[j],&a[j+1]);
	      j_last=j; // luu gai tri chi so
	    }
	}
      r=j_last; // luu gia tri chi so ra "r" co gia tri moi 
    }
}
void ShellSort(int a[], int n)
{
  // _Y Tuong: chon k buoc xap sep <=> chia ra hai day con voi do dai h[k] (thuong chon day so nguyen to) sau do dua tren InsertionSort de sap xep.
  int i,x,step,len,pos;
  int k=3;
  int h[3]={5,3,1} ;
  for(step=0;step<k;step++)
    {
      len=h[step];
      for(i=len;i<n;i++)
        {
	  x=a[i];
	  pos=i-len;
	  while((pos>=0)&&(a[pos]>x))
            {
	      a[pos+len]=a[pos];
	      pos=pos-len;
            }
	  a[pos+len]=x;
        }
    }
}
void Heapsort(int a[], int n)//n la so luong phan tu cua mang. 
{
  int k,x,s,f,ivalue;
  for(k=1;k<n;k++)
    {
      x=a[k];
      s=k;
      f=(s-1)/2;
      while(s>0&&a[f]<x)
	{
	  a[s]=a[f];
	  s=f;
	  f=(s-1)/2;
	}
      a[s]=x;
    }
  for(k=n-1;k>0;k--)
    {
      ivalue=a[k];
      a[k]=a[0];
      f=0;
      if(k==1)
	s=-1;
      else
	s=1;
      if(k>2&&a[2]>a[1])
	s=2;
      while(s>=0&&ivalue<a[s])
	{
	  a[f]=a[s];
	  f=s;
	  s=2*f+1;
	  if(s+1<=k-1&&a[s]<a[s+1])
	    s=s+1;
	  if(s>k-1)
	    s=-1;
        }
      a[f]=ivalue;
    }
}


// can phai xem lai radix sort, chang hieu gi ca.
int getMax(int a[],int n)
{
  int max=a[0],i;
  for(i=1;i<n;i++)
    if(max<a[i])
      max=a[i];
  return max;
}
int countDigit(int n)
{
  int count=0;
  while(n)
    {
      count++;
      n=n/10;
    }
  return count;
}
int getDigit(int n,int t)// t=countDigit();
{
  int tt=1,i;
  for(i=0;i<t;i++)tt=tt*10;
  return ((n/tt)%10);
}
void send2Box(int a[],int n,int b[10][MAX],int number[10],int t)
{
  int i,tt;
  for(i=0;i<n;i++)
    {
      tt=getDigit(a[i],t);
      b[tt][number[tt]++]=a[i];
    }
}
void getValue(int a[],int b[10][MAX],int number[10])
{
  int j=0,i,k;
  for(i=0;i<10;i++)
    {
      if(number[i]!=0)
	for(k=0;k<number[i];k++)
	  a[j++]=b[i][k];
      number[i]=0;
    }
}
void radixsort(int a[],int n)
{
  int Box[10][MAX];
  int number[10],i,j;
  for(i=0;i<10;i++)
    {
      for(j=0;j<n;j++)
	Box[i][j]=0;
      number[i]=0;
    }
  int nn=countDigit(getMax(a,n));
  for(i=0;i<nn;i++)
    {
      send2Box(a,n,Box,number,i);
      getValue(a,Box,number);
    }
}

void RQSWithRange( int a[],int l,int r)
{
  if(l<r)
    {
      int m,i;
      if(l<r)
	{
	  int randNum=rand()%(r-l+1);
	  swap(&a[r],&a[l+randNum]);
	  m=l-1;
	  for(i=l;i<r;i++)
	    {
	      if(a[i]<a[r])
		{
		  swap(&a[++m],&a[i]);
		}
	    }
	  swap(&a[++m],&a[r]);
	  RQSWithRange(a,l,m-1);
	  RQSWithRange(a,m+1,r);
	}
    }
}
void randomizedquicksort(int a[],int *n)
{
  srand((unsigned)time(0));
  RQSWithRange(a,0,(*n)-1);
}
