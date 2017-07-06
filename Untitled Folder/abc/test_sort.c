#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include"sort.h"
typedef struct
{
  int t;// Thoi gian thoai.
  int gio;// Gio bat dau cuoc goi.
  int phut;// Phut bat dau cuoc goi.
  int giay;// Giay bat dau cuoc goi.
  long int caller;// so dien thoai nguoi nhan.
  long int called;// so dien thoai nguoi goi.
}
  call;

call *nhap(call *e,int left,int right);
void copydata(void *a,void *e,int sophantu,int size);
void printdata(call *e,int sophantu);
void fprintdata(call *e,int sophantu,FILE *f);
void menu_main();
void menu_thuat_toan();
int luachon(void(*menu)(void ),int soluachon);
int compare_during(const void *a,const void *b);
int compare_call(const void *a,const void *b);
int compare_during_time(const void *a,const void *b);
int compare(const void *a,const void *b);

int search(void *a,void *key,int *chiso,int sophantu,int size);
void thoigianchay(void *a,int sophantu,int size,int(*compare_t)(void const *,void const *));
int loaibo(void *a,int sophantu,int size,int(*sosanh)(void const *,void const *));


//// Dinh nghia.
call *nhap(call *e,int left,int right)
{
  int i,c;
  srand(time(NULL));
  for(i=left;i<=right;i++)
    {
      c=rand()%2;
      e[i].gio=rand()%24;
      e[i].phut=rand()%60;
      e[i].giay=rand()%60;
      e[i].t=rand()%3600+1;
      if(c==1)
	e[i].called=(rand()%100000000)+100000000;
      else
	e[i].called=(rand()%100000000)+900000000;
      c=rand()%2;
      if(c==1)
	e[i].caller=(rand()%100000000)+100000000;
      else
	e[i].caller=(rand()&100000000)+900000000;
    }
  return e;
}

void copydata(void *a,void *e,int sophantu,int size)
{
  memcpy((char*)a,(char*)e,sophantu*size);
}
void printdata(call *e,int sophantu)
{
  int i;
  for(i=0;i<sophantu;i++) 
    {
      printf("0%-15ld0%-15ld",e[i].caller,e[i].called);
      if(e[i].gio<10)
	printf("0");
      printf("%d:",e[i].gio);
      if(e[i].phut<10)
	printf("0");
      printf("%d:",e[i].phut);
      if(e[i].giay<10)
	printf("0");
      printf("%d",e[i].giay);
      printf("%10d\n",e[i].t);
    }
}
void fprintdata(call *e,int sophantu,FILE *f)
{ 
  int i;
  for(i=0;i<sophantu;i++)
    {
      fprintf(f,"0%-15ld0%-15ld",e[i].caller,e[i].called);
      if(e[i].gio<10)
	fprintf(f,"0");
      fprintf(f,"%d:",e[i].gio);
      if(e[i].phut<10)
	fprintf(f,"0");
      fprintf(f,"%d:",e[i].phut);
      if(e[i].giay<10)
	fprintf(f,"0");
      fprintf(f,"%d",e[i].giay);
      fprintf(f,"%10d\n",e[i].t);
    }
}

void menu_main()
{
  printf("\n");
  printf("========================================================\n");
  printf("=============== BAI TAP WEEK 1 C ADVANCE ===============\n");
  printf("========================================================\n\n");
  // printf("!!! Hướng dẫn sử dụng:\n Sau khi nhập lựa chọn, để so sánh được thời gian tính của các thuật toán sắp xếp ( ở đây chỉ xét 4 thuật toán cơ bản) hãy chọn đủ cả 4 thuật toán rồi mở file tương ứng để xem kết quả.\n Sau khi chọn đủ cả 4 thuật toán trong một case ( trừ case 4) thì xem file kết quả trước khi chọn case tiếp theo. Chúc bạn may mắn ! ( vì code quá chày cối).\n");
  printf("  1. Khoi tao du lieu.\n");
  printf("  2. Case study 1.\n");
  printf("  3. Case study 2.\n");
  printf("  4. Bai tap 1.\n");
  printf("  5. Bai tap 2.\n");
  printf("  6. Thoat.\n\n");
  printf("=====> Nhap lua chon cua ban : ");
}
int luachon(void (*menu)(void ),int soluachon)
{
  int luachon;
  do
    {
      menu();
      while(scanf("%d",&luachon)!=1)
	{
	  while(getchar()!='\n');
	  printf("  !!! Nhap sai! Hay nhap vao so tu 1 den %d!\n",soluachon);
	  menu();
	}
      while(getchar()!='\n');
      if(luachon<1||luachon>soluachon)
	printf("  !!! Nhap sai! Hay nhap vao so tu 1 den %d!\n",soluachon);
    }
  while(luachon<1||luachon>soluachon);
  return luachon;
}

int compare_during(const void *a,const void *b)
{
  call *m,*n;
  m=(call*)a;
  n=(call*)b;
  return ((*m).t-(*n).t);
}
int compare_call(const void *a,const void *b)
{
  call *m,*n;
  m=(call*)a;
  n=(call*)b;
  if((*m).caller<(*n).caller)
    return -1;
  else
    if((*m).caller>(*n).caller)
      return 1;
    else
      if((*m).called<(*n).called)
	return -1;
      else
	if((*m).called>(*n).called)
	  return 1;
	else
	  if((*m).gio!=(*n).gio)
	    return ((*m).gio-(*n).gio);
	  else
	    if((*m).phut!=(*n).phut)
	      return ((*m).phut-(*n).phut);
	    else
	      if((*m).giay!=(*n).giay)
		return ((*m).giay-(*n).giay);
	      else
		return ((*m).t-(*n).t);
}

int compare_during_time(const void *a,const void *b)
{
  call *m,*n;
  m=(call*)a;
  n=(call*)b;
  if((*m).t!=(*n).t)
    return ((*m).t-(*n).t);
  else
    if((*m).gio!=(*n).gio)
      return ((*m).gio-(*n).gio);
    else
      if((*m).phut!=(*n).phut)
	return ((*m).phut-(*n).phut);
      else
	return ((*m).giay-(*n).giay);
}

int compare(const void *a,const void *b)
{
  return (*(int*)a-*(int*)b);
}


int search(void *a,void *key,int *chiso,int sophantu,int size)
{
  int i,j=0;
  for(i=0;i<sophantu;i++)
    if(compare((char*)a+i*size,key)==0)
      chiso[j++]=i;
  return j;
}

void menu_thuat_toan()
{
  printf("\n");
  printf("========================================================\n");
  printf("================ THUAT TOAN SAP XEP ====================\n");
  printf("========================================================\n\n");
  printf("  1. Quicksort 2 ways.\n");
  printf("  2. Insertionsort.\n");
  printf("  3. Selectionsort.\n");
  printf("  4. Quicksort 3 ways.\n");
  printf("  5. Thoat.\n\n");
  printf("======> Nhap lua chon: ");
}
void thoigianchay(void *e,int sophantu,int size,int(*compare_t)(void const *,void const *))
{
  int l=0;
  clock_t begin,end;
  FILE *f;
  char name1[4][30]={"qsort_2_ways","insertionsort","selectionsort","qsort_3_ways"};
  char name[4][30]={"qsorted_2_ways.txt","insertionsorted.txt","selectionsorted.txt","qsorted_3_ways.txt"};
  while(l!=5)
    {
      l=luachon(menu_thuat_toan,5);
      switch(l)
	{
	case 1:
	  {
	    void *a;
	    a=malloc(sophantu*size);
	    copydata(a,e,sophantu,size);
	    begin=clock();
	    qsort_2_ways(a,0,sophantu-1,size,compare_t);
	    end=clock();
	    f=fopen(name[l-1],"w");
	    if(f==NULL)
	      {
		printf(" !!!  Khong the mo file.\n");
		return;
	      }
	    fprintf(f,"Thoi gian tinh cua thuat toan %s la %lfs.\n",name1[l-1],(double)(end-begin)/CLOCKS_PER_SEC);
	    if(size!=sizeof(int))
	      fprintdata(a,sophantu,f);
	    printf("_________________________________________________________\n");
	    printf("=====> Xem ket qua tren file %s.\n\n",name[l-1]);
	    fclose(f);
	    free(a);
	    break;
	  }
	case 2:
	  {
	    void *a;
	    a=malloc(sophantu*size);
	    copydata(a,e,sophantu,size);
	    begin=clock();
	    insertionsort(a,sophantu,size,compare_t);
	    end=clock();
	    f=fopen(name[l-1],"w");
	    if(f==NULL)
	      {
		printf(" !!!  Khong the mo file.\n");
		return;
	      }
	    fprintf(f,"Thoi gian tinh cua thuat toan %s la %lfs.\n",name1[l-1],(double)(end-begin)/CLOCKS_PER_SEC);
	    if(size!=sizeof(int))
	      fprintdata(a,sophantu,f);
	    printf("_________________________________________________________\n");
	    printf("=====> Xem ket qua tren file %s.\n\n",name[l-1]);
	    fclose(f);
	    free(a);
	    break;
	  }
	case 3:
	  {
	    void *a;
	    a=malloc(sophantu*size);
	    copydata(a,e,sophantu,size);
	    begin=clock();
	    selectionsort(a,sophantu,size,compare_t);
	    end=clock();
	    f=fopen(name[l-1],"w");
	    if(f==NULL)
	      {
		printf(" !!!  Khong the mo file.\n");
		return;
	      }
	    fprintf(f,"Thoi gian tinh cua thuat toan %s la %lfs.\n",name1[l-1],(double)(end-begin)/CLOCKS_PER_SEC);
	    if(size!=sizeof(int))
	      fprintdata(a,sophantu,f);
	    printf("_________________________________________________________\n");
	    printf("=====> Xem ket qua tren file %s.\n\n",name[l-1]);
	    fclose(f);
	    free(a);
	    break;
	  }
	case 4:
	  {
	    void *a;
	    a=malloc(sophantu*size);
	    copydata(a,e,sophantu,size);
	    begin=clock();
	    qsort_3_ways(a,0,sophantu-1,size,compare_t);
	    end=clock();
	    f=fopen(name[l-1],"w");
	    if(f==NULL)
	      {
		printf(" !!!  Khong the mo file.\n");
		return;
	      }
	    fprintf(f,"Thoi gian tinh cua thuat toan %s la %lfs.\n",name1[l-1],(double)(end-begin)/CLOCKS_PER_SEC);
	    if(size!=sizeof(int))
	      fprintdata(a,sophantu,f);
	    printf("_________________________________________________________\n");
	    printf("=====> Xem ket qua tren file %s.\n\n",name[l-1]);
	    fclose(f);
	    free(a);
	    break;
	  }
	case 5:
	  {
	    printf("THANK YOU. SEE YOU AGAIN.\n");
	    break;
	  }
	}
    }
}

int loaibo(void *a,int sophantu,int size,int(*sosanh)(void const *,void const *))
{
  int i=0,j;
  qsort_2_ways(a,0,sophantu-1,size,sosanh);
  while(1)
    {
      if(i>=sophantu)
	break;
      if(sosanh((char*)a+i*size,(char*)a+(i+1)*size)==0)
	{
	  for(j=i+1;j<sophantu;j++)
	    memcpy((char*)a+j*size,(char*)a+(j+1)*size,size);
	  sophantu--;
	}
      else
	i++;
    }
}

int main()
{
  int n,max,t,*array,m,i;
  call *e,*b;
  n=0;
  array=NULL;
  e=NULL;
  b=NULL;
  while(n!=6)
    {
      n=luachon(menu_main,6);
      switch(n)
	{
	case 1:
	  {
	    FILE *f;
	    printf("   Nhap so luong phan tu cua mang so nguyen array: ");
	    scanf("%d",&m);
	    array=(int*)malloc(m*sizeof(int));
	    srand(time(NULL));
	    for(i=0;i<m;i++)
	      array[i]=rand()%10;
	    f=fopen("intdata.txt","w");
	    for(i=0;i<m;i++)
	      fprintf(f,"%d\n",array[i]);
	    fclose(f);
	    printf("\n=====> Xem ket qua tao du lieu tren file intdata.txt.\n\n");
	    printf("   Nhap so luong phan tu cua mang cau truc call: ");
	    scanf("%d",&max);
	    e=(call*)malloc(max*sizeof(call));
	    nhap(e,0,max-1);
	    f=fopen("phonedata.txt","w");
	    fprintdata(e,max,f);
	    fclose(f);
	    printf("\n=====> Xem ket qua tao du lieu tren file phonedata.txt.\n\n");
	    printf("   Nhap so luong phan tu can them:");
	    scanf("%d",&t);
	    b=(call*)malloc((max+t)*sizeof(call));
	    copydata(b,e,max,sizeof(call));
	    qsort_3_ways(b,0,max-1,sizeof(call),compare_during);
	    nhap(b,max,max+t-1);
	    printf("\n   Tao du lieu thanh cong.\n");
	    break;
	  }
	case 2:
	  {
	    printf("___________________CASE STUDY 1.__________________\n");
	    printf("\n Xử lý các bản ghi giao dịch của 1 công ty điện thoại.\n=> Lựa chọn thuật toán phù hợp : Quick sort.\n");
	    if(e==NULL)
	      {
		printf(" !!!  Chua co du lieu de xu li. Hay chon chuc nang khoi tao du lieu de tiep tuc!\n");
		break;
	      }
	    thoigianchay(e,max,sizeof(call),compare_during);
	    break;
	  }
	case 3:
	  {
	    printf("___________________CASE STUDY 2.__________________\n");
	    printf("Sắp lại 1 CSDL lớn sau khi có một vài thay đổi nhỏ.\n=> Lựa chọn thuật toán: Insertion sort.\n");
	    if(b==NULL)
	      {
		printf(" !!!  Chua co du lieu de xu li. Hay chon chuc nang khoi tao du lieu de tiep tuc!\n");
		break;
	      }
	    thoigianchay(b,max+t,sizeof(call),compare_during);
	    break;
	  }
	case 4:
	  {
	    printf("___________________BAI TAP 1.__________________\n");
	    if(array==NULL)
	      {
		printf(" !!!  Chua co du lieu de xu li. Hay chon chuc nang khoi tao du lieu de tiep tuc!\n");
		break;
	      }
	    thoigianchay(array,m,sizeof(int),compare);
	    break;
	  }
	case 5:
	  {
	    printf("___________________BAI TAP 2.__________________\n");
	    if(array==NULL)
	      {
		printf(" !!!  Chua co du lieu de xu li. Hay chon chuc nang khoi tao du lieu de tiep tuc!\n");
		break;
	      }
	    printf("De sap xep mang theo thu tu khong tang ta dao nguoc dau gia tri tra ve cua ham compare la duoc.\n");
	    printf("Ta dung ham compare1 (xem code case 5).\n");
	    int compare1(const void *a,const void *b)
	    {
	      return (*(int*)b-*(int*)a);
	    }
	    int *a,l;
	    a=(int*)malloc(m*sizeof(int));
	    memcpy((char*)a,(char*)array,m*sizeof(int));
	    if(m<=10000)
	      {
		printf("=====> Mang truoc khi sap xep la :\n");
		for(i=0;i<m;i++)
		  printf("%d ",a[i]);
	      }
	    printf("\n_____________________________________________________\n");
	    qsort(a,m,sizeof(int),compare1);
	    if(m<=10000)
	      {
		printf("=====> Mang sau khi sap xep theo thu tu khong tang la:\n");
		for(i=0;i<m;i++)
		  printf("%d ",a[i]);
	      }
	    else
	      {
		printf("=====> Xem ket qua tren file %s.\n","sortintdata.txt");
		FILE *f;
		f=fopen("sortintdata.txt","w");
		for(i=0;i<m;i++)
		  fprintf(f,"%3d",a[i]);
		fclose(f);
	      }
	    printf("\n\n");
	    l=loaibo(a,m,sizeof(int),compare);
	    printf("=====> Mang sau khi loai bo cac phan tu trung nhau la:\n");
	    for(i=0;i<l;i++)
	      printf("%d ",a[i]);
	    printf("\n");
	    free(a);
	    a=NULL;
	    break;
	  }
	case 6:
	  {
	    printf("??? CUOI CUNG CUNG XONG! LAPTOP DA BI DO! SEE YOU AGAIN!...\n");
	    break;
	  }
	}
    }
  /* radixsort(e,max,sizeof(call),compare);
  printf("______________\n");
  printdata(e,max);*/
  /* heapsort(e,max,sizeof(call))//compare);
  printf("______________\n");
  printdata(e,max);*/
  /* shellsort(e,max,sizeof(call),compare);
  printf("______________\n");
  printdata(e,max);*/
  /* shakersort(e,max,sizeof(call),compare);
  printf("______________\n");
  printdata(e,max);*/
  /* free(a);
  a=(call*)malloc(max*sizeof(call));
  mergesort(e,a,0,max-1,sizeof(call),compare);
  printf("______________\n");
  printdata(e,max);*/
  /* insertionsort(e,max,sizeof(call),compare);
  printf("______________\n");
  printdata(e,max);*/
  /* selectionsort(e,max,sizeof(call),compare);
  printf("______________\n"); 
  printdata(e,max);*/
  /* qsort_3_ways(e,0,max-1,sizeof(call),compare);
  printf("______________\n"); 
  printdata(e,max);*/
  /*qsort_2_ways(e,0,max-1,sizeof(call),compare);
  printf("______________\n");
  printdata(e,max);*/
  if(array!=NULL)
    {
      free(e);
      free(b);
      free(array);
    }
  return 0;
}
