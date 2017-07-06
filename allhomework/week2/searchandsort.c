#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

typedef union {
  int i;
  long l;
  float f;
  double d;
  void *v;
  char *s;
  char c;
  unsigned char uc;
  short sh;
  unsigned short ush;
  unsigned int ui;
  int *iarray;
  float *farray;
  char carray[30];
  unsigned char ucarray[30];
}
  jval;

jval new_jval_i(int);
jval new_jval_l(long);
jval new_jval_f(float);
jval new_jval_d(double);
jval new_jval_v(/* void */);
jval new_jval_s(char *);
jval new_jval_c(char);
jval new_jval_uc(unsigned char);
jval new_jval_sh(short);
jval new_jval_ush(unsigned short);
jval new_jval_ui(unsigned int);
jval new_jval_iarray(int *);
jval new_jval_farray(float *);
jval new_jval_carray_nt(char *);  /* Carray is null terminated */
jval new_jval_carray_nnt(char *);  /* Carray is not null terminated */
       /* For ucarray -- use carray, because it uses memcpy */

jval JNULL;

int jval_i(jval);
long jval_l(jval);
float jval_f(jval);
double jval_d(jval);
void *jval_v(jval);
char *jval_s(jval);
char jval_c(jval);
unsigned char jval_uc(jval);
short jval_sh(jval);
unsigned short jval_ush(jval);
unsigned int jval_ui(jval);
int *jval_iarray(jval);
float *jval_farray(jval);
char *jval_carray(jval);


jval JNULL;

jval new_jval_i(int i) {
  jval j;
  j.i=i;
  return j;
}
  
jval new_jval_l(long int l) {
  jval j;
  j.l=l;
  return j;
}
  
jval new_jval_f(float f) {
  jval j;
  j.f=f;
  return j;
}
  
jval new_jval_d(double d) {
  jval j;
  j.d=d;
  return j;
}
  
jval new_jval_v(void *v) {
  jval j;
  j.v=v;
  return j;
}
  
jval new_jval_s(char *s) {
  jval j;
  j.s=s;
  return j;
}
  
jval new_jval_c(char c) {
  jval j;
  j.c=c;
  return j;
}
  
jval new_jval_uc(unsigned char uc) {
  jval j;
  j.uc=uc;
  return j;
}
  
jval new_jval_sh(short sh) {
  jval j;
  j.sh=sh;
  return j;
}
  
jval new_jval_ush(unsigned short ush) {
  jval j;
  j.ush=ush;
  return j;
}
  
jval new_jval_ui(unsigned int i) {
  jval j;
  j.i=i;
  return j;
}
  
jval new_jval_iarray(int *i) {
  jval j;
  j.iarray=i;
  return j;
}
  
jval new_jval_farray(float *f) {
  jval j;
  j.farray=f;
  return j;
}
  
jval new_jval_carray_nt(char *carray) {
  jval j;
  int i;

  for(i=0;i<30&&carray[i]!='\0';i++)
    {
      j.carray[i]=carray[i];
    }
  if(i<30) j.carray[i]=carray[i];
  return j;
}
  
jval new_jval_carray_nnt(char *carray) {
  jval j;

  memcpy(j.carray,carray,30);
  return j;
}

int jval_i(jval j) {
  return j.i;
}

long jval_l(jval j) {
  return j.l;
}

float jval_f(jval j) {
  return j.f;
}

double jval_d(jval j) {
  return j.d;
}

void *jval_v(jval j) {
  return j.v;
}

char *jval_s(jval j) {
  return j.s;
}

char jval_c(jval j) {
  return j.c;
}

unsigned char jval_uc(jval j) {
  return j.uc;
}

short jval_sh(jval j) {
  return j.sh;
}

unsigned short jval_ush(jval j) {
  return j.ush;
}

unsigned int jval_ui(jval j) {
  return j.ui;
}

int *jval_iarray(jval j) {
  int *i;
  i=j.iarray;
  return i;
}

float *jval_farray(jval j) {
  float *i;
  i=j.farray;
  return i;
}

char *jval_carray(jval j) {
  char *i;
  i=j.carray;
  return i;
}

int compare(jval *a,jval *b) {
  return (*(int*)a-*(int*)b);
}
int compare_i(jval *a,jval *b) {
  return (jval_i(*a)-jval_i(*b));
}

int compare_l(jval *a,jval *b) {
  return (jval_l(*a)-jval_l(*b));
}

float compare_f(jval *a,jval *b) {
  return (jval_f(*a)-jval_f(*b));
}

double compare_d(jval *a,jval *b) {
  return (jval_d(*a)-jval_d(*b));
}

int compare_v(jval *a,jval *b) {
  return (*(int*)jval_v(*a)-*(int*)jval_v(*b));
}

int compare_s(jval *a,jval *b) {
  return strcmp(jval_s(*a),jval_s(*b));
}

int compare_c(jval *a,jval *b) {
  return (jval_c(*a)-jval_c(*b));
}
void swap(jval *a,jval *b) {
  jval tmp;
  tmp=*a;
  *a=*b;
  *b=tmp;
}

void sort_gen(jval a[],int left,int right,int(*compare)(jval *,jval *)) {
  int i=left,j=right;
  if(left>=right)
    return;
  jval tmp;
  tmp=a[(left+right)/2];
  while(i<=j)
    {
      while(compare(&a[i],&tmp)<0)
	i++;
      while(compare(&a[j],&tmp)>0)
	{
	  if(j==left)
	    break;
	  j--;
	}
      if(i<=j)
	{
	  swap(&a[i],&a[j]);
	  i++;
	  j--;
	}
    }
  sort_gen(a,left,j,compare);
  sort_gen(a,i,right,compare);
}

int *search(jval a[],int sophantu,jval key,int(*compare)(jval *,jval *)) {
  int *chiso;
  int i,j=0;
  chiso=(int*)malloc(sophantu*sizeof(int));
  for(i=0;i<sophantu;i++)
    if(compare(&a[i],&key)==0)
      {
	chiso[j]=i;
	j++;
      }
  return chiso;
}



int main(int argc, char *argv[]) {
  int *tmp;
  int i, max, x, l;
  jval *a;
  if(argc != 2)
    {
      printf(" Doi so chua du. Hay nhap vao 2 doi so.\nNhap doi so thu hai la so luong phan tu theo lenh './searchandsort so_luong_phan_tu'.\n");
      return 0;
    }
  max = atoi(argv[1]);
  printf("so luong phan tu la : max = %d\n", max);
  a = (jval*)malloc(max*sizeof(jval));
  tmp = (int*)malloc(max*sizeof(int));
  srand(time(NULL));
  printf(" Du lieu ban dau:\n");
  for(i = 0; i < max; i++)
    {
      tmp[i] = rand()%10;
      a[i] = new_jval_i(tmp[i]);
      printf("%d ",tmp[i]);
    }
  printf("\n______________________________________________________________\n");
  int *chiso;
  chiso = (int*)malloc(max*sizeof(int));
  for(i = 0; i < max; i++)
    chiso[i] = 0;
  int k;
  jval e;
  printf("Nhap vao so nguyen can tim kiem: ");
  while(scanf("%d", &k) != 1)
    {
      while(getchar() != '\n');
      printf("Hay nhap vao mot so nguyen.\n");
    }
  while(getchar() != '\n');
  e = new_jval_i(k);
  chiso=search(a, max, e, compare_i);
  i = 0;
  if(chiso != NULL)
    {
      if(jval_i(a[0]) != jval_i(e) && chiso[0] == 0)
	printf("Khong co so can tim.\n");
      else
	{
	  printf(" Cac phan tu co gia tri %d la: \n", jval_i(e));
	  while(1)
	    {
	      printf("a[%d] = %d\n", chiso[i], jval_i(a[chiso[i]]));
	      i++;
	      if(chiso[i] == 0)
		break;
	    }
	}
    }
  return 0;
}
