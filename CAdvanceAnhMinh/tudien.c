#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<termios.h>
#include<unistd.h>
#include<time.h>
#include"btree.h"
#include"soundex.h"

#define M 50
#define L 4
#define BACKSP_LINE "\b \b"
static struct termios orig_termios;
static int ttyfd = STDIN_FILENO;

typedef char str[M];

typedef struct
{
  str list[10];
  int count;
}
  mean;

typedef struct
{
  str word[1000];
  int count;
}
  soundex;


// BEGIN AUTO COMPLETED

int strhead(const char *a,const char *b,int x)	/*kiểm tra x chữ cái đầu của xâu có giống nhau ko */
{
  int i;
  for(i=0;i<x;i++)
    if((a[i])!=b[i]) return 0;
  return 1*(x>0);
}

void fatal(char *message)
{
  fprintf(stderr,"fatal error: %s\n",message);
  exit(1);
}

int tty_reset(void)
{
  /* flush and reset */
  if (tcsetattr(ttyfd,TCSAFLUSH,&orig_termios) < 0) return -1;
  return 0;
}

/* exit handler for tty reset */
void tty_atexit(void)  /* NOTE: If the program terminates due to a signal   */
{                      /* this code will not run.  This is for exit()'s     */
  tty_reset();        /* only.  For resetting the terminal after a signal, */
}                      /* a signal handler which calls tty_reset is needed. */

/* reset tty - useful also for restoring the terminal when this process
   wishes to temporarily relinquish the tty
*/


/* Cài đặt ubuntu ở chế độ raw */
void tty_raw(void)
{
  struct termios raw;
  
  raw = orig_termios;  /* copy original and then modify below */

  /* input modes - clear indicated ones giving: no break, no CR to NL,
     no parity check, no strip char, no start/stop output (sic) control */
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  
  /* output modes - clear giving: no post processing such as NL to CR+NL */
  raw.c_oflag &= ~(OPOST);
  
  /* control modes - set 8 bit chars */
  raw.c_cflag |= (CS8);
  
  /* local modes - clear giving: echoing off, canonical off (no erase with
     backspace, ^U,...),  no extended functions, no signal chars (^Z,^C) */
  raw.c_lflag &= ~(ECHO |ICANON | IEXTEN | ISIG);
  
  /* control chars - set return condition: min number of bytes and timer */
  raw.c_cc[VMIN] = 5; raw.c_cc[VTIME] = 8; /* after 5 bytes or .8 seconds
					      after first byte seen      */
  raw.c_cc[VMIN] = 0; raw.c_cc[VTIME] = 0; /* immediate - anything       */
  raw.c_cc[VMIN] = 2; raw.c_cc[VTIME] = 0; /* after two bytes, no timer  */
  raw.c_cc[VMIN] = 0; raw.c_cc[VTIME] = 8; /* after a byte or .8 seconds */
  
  /* put terminal in raw mode after flushing */
  if(tcsetattr(ttyfd,TCSAFLUSH,&raw)<0) fatal("can't set raw mode");
}


/* Read and write from tty - this is just toy code!!
   Prints T on timeout, quits on q input, prints Z if z input, goes up
   if u input, prints * for any other input character
*/

int screenio(BTA* btfile,char *key)//hàm xử lý kí tự nhập từ bộ đệm gồm cả dấu tab
{
  BTint x;
  int bytesread,i=0,j,n;
  key[i]='\0';
  char c_in,back_space[]=BACKSP_LINE,sugg[M];
  for(;;)
    {
      bytesread=read(ttyfd,&c_in,1); /* Đọc 1 byte ki tự */
      if(bytesread<0) fatal("read error");
      if(bytesread==0) c_in=0;   /* Ko đọc đc byte nào thì bỏ qua */
      switch(c_in)         /* Khi đọc đc 1 byte */
	{
	case '\r' : printf("\n\r");  /* Khi gặp dấu ENTER hoặc ESC thì kết thúc */
	case 27 : return 0;
	case 127:					/*Xử lý khi bấm phím BACKSPACE */
	  i=strlen(key);
	  if(i>0)
	    {
	      key[--i]='\0';
	      bfndky(btfile,key,&x);
	      write(STDOUT_FILENO,back_space,3);
	    }
	  break;
	case '\t':	      /* Gợi ý và in ra từ  khi bấm phím TAB */
	  j=strlen(key);
	  while(j-->0) write(STDOUT_FILENO,back_space,3);
	  n=bnxtky(btfile,sugg,&x);
	  if(n==0&&strhead(key,sugg,i)!=0)
	    strcpy(key,sugg);
	  else
	    {
	      key[i]='\0';
	      bfndky(btfile,key,&x);
	    }
	  write(STDOUT_FILENO,key,strlen(key));
	  break;
	default:
	  if(' '<=c_in&&c_in<='~')
	    {		/* In ra các kí tự nhận vào như bt */
	      i=strlen(key);
	      key[i++]=tolower(c_in);
	      key[i]='\0';
	      bfndky(btfile,key,&x);
	      write(STDOUT_FILENO, &c_in, 1);
	    }
	}
    }
}



int tab_complt(BTA *btfile,char *key)    /* Hàm này kích hoạt terminal ở chế độ raw rồi xử lý kí tự*/
{
  /* check that input is from a tty */
  if (! isatty(ttyfd)) fatal("not on a tty");
  
  /* store current tty settings in orig_termios */
  if (tcgetattr(ttyfd,&orig_termios) < 0) fatal("can't get tty settings");

  /* register the tty reset with the exit handler */
  if (atexit(tty_atexit) != 0) fatal("atexit: can't register tty reset");

  tty_raw();      /* put tty in raw mode */
  screenio(btfile,key);
  tty_atexit();   /* run application code */
  return 0;       /* tty_atexit will restore terminal */
}
// END AUTO COMPLETE


int luachon(void (*menu)(void ),int soluachon)
{
  int luachon;
  do
    {
      menu();
      while(scanf("%d",&luachon)!=1)
	{
	  while(getchar()!='\n');
	  printf("\n  !!! Nhap sai! Hay nhap vao so tu 1 den %d!\n",soluachon);
	  menu();
	}
      while(getchar()!='\n');
      if(luachon<1||luachon>soluachon)
	printf("\n  !!! Nhap sai! Hay nhap vao so tu 1 den %d!\n",soluachon);
    }
  while(luachon<1||luachon>soluachon);
  return luachon;
}

void menu_tudien()
{
  printf("\n===========================================================\n");
  printf("========== CHUONG TRINH QUAN LY TU DIEN DIEN TU ===========\n");
  printf("===========================================================\n\n");
  printf("\t1. Them tu moi.\n");
  printf("\t2. Tim kiem nghia cua tu.\n");
  printf("\t3. Xoa mot tu.\n");
  printf("\t4. Thoat.\n\n");
  printf("=====> Nhap lua chon cua ban : ");
}


char *nhap_xau()
{
  char *s;
  s=(char*)malloc(50*sizeof(char));
  scanf("%[^\n]%*c",s);
  return s;
}

int add_dic(BTA *root,char *w,char *m)
{
  mean *s;
  int rsize,k=0,i;
  s=(mean*)malloc(sizeof(mean));
  if(btsel(root,w,(char *)s,sizeof(mean),&rsize)==0)
    {
      for(i=0;i<s->count;i++)
	if(strcmp(s->list[i],m)==0)
	  k=1;
      if(k==1)
	return 0;
      else
	{
	  strcpy(s->list[s->count],m);
	  s->count++;
	  btupd(root,w,(char *)s,sizeof(mean));
	}
    }
  else
    {
      strcpy(s->list[0],m);
      s->count=1;
      btins(root,w,(char *)s,sizeof(mean));
    }
  free(s);
  return 1;
}

int add_sou(BTA *root,char *w)
{
  soundex *se;
  char s[10];
  int rsize,k=0,i;
  se=(soundex*)malloc(sizeof(soundex));
  soundEx(s,w,50,1);
  if(btsel(root,s,(char *)se,sizeof(soundex),&rsize)==0)
    {
      for(i=0;i<se->count;i++)
	if(strcmp(se->word[i],w)==0)
	  k=1;
      if(k==1)
	return 0;
      else
	{
	  strcpy(se->word[se->count],w);
	  se->count++;
	  btupd(root,s,(char *)se,sizeof(soundex));
	}
    }
  else
    {
      strcpy(se->word[0],w);
      se->count=1;
      btins(root,s,(char *)se,sizeof(soundex));
    }
  free(se);
  return 1;
}

int add_word(BTA *Dic,BTA *Sou,char *w,char *m)
{
  add_sou(Sou,w);
  if(add_dic(Dic,w,m)==1)
    return 1;
  else
    return 0;
}

mean *search_mean(BTA *root,char *w)
{
  mean *s;
  int rsize;
  s=(mean*)malloc(sizeof(mean));
  if(btsel(root,w,(char *)s,sizeof(mean),&rsize)==0)
    return s;
  else
    {
      free(s);
      return NULL;
    }
}

void print_mean(mean *m,char *w)
{
  int i;
  if(m==NULL)
    {
      printf("\n  Khong ton tai tu %s trong tu dien.\n",w);
      return;
    }
  printf("\n=====> Nghia cua tu %s la : ",w);
  for(i=0;i<m->count-1;i++)
    printf("%s, ",m->list[i]);
  printf("%s.\n",m->list[i]);
  return;
}

soundex *search_soundex(BTA *root,char *w)
{
  soundex *se;
  int rsize,i;
  char s[10];
  soundEx(s,w,50,1);
  se=(soundex*)malloc(sizeof(soundex));
  if(btsel(root,s,(char *)se,sizeof(soundex),&rsize)==0)
    return se;
  else
    {
      free(se);
      return NULL;
    }
}

void print_soundex(soundex *se,char *w)
{
  int i;
  if(se==NULL)
    printf("\n  Khong co tu nao co cach phat am gan giong voi tu %s.\n",w);
  else
    {
      printf("\n  Cac tu co cach phat am gan giong voi tu %s la :\n",w);
      for(i=0;i<se->count;i++)
	printf("\t%d. %s.\n",i+1,se->word[i]);
    }
  return;
}



int menu_soundex(soundex *se,char *w)
{
  int n;
  n=0;
  while(n<1||n>se->count)
    {
      print_soundex(se,w);
      printf("\n  Nhap vao so thu tu tuong ung voi tu can tra : ");
      while(scanf("%d",&n)!=1)
	{
	  while(getchar()!='\n');
	  printf("\n  !!! Nhap sai! Hay nhap vao so tu 1 den %d!\n",se->count);
	  print_soundex(se,w);
	  printf("\n  Nhap vao so thu tu tuong ung voi tu can tra : ");
	}
      while(getchar()!='\n');
      if(n<1||n>se->count)
	printf("\n  !!! Nhap sai! Hay nhap vao so tu 1 den %d!\n",se->count);
    }
  return n;
}

void soundex_search(BTA *Dic,BTA *Sou,char *w)
{
  int rsize,n,i=0;
  BTint in;
  soundex *se;
  mean *m;
  se=search_soundex(Sou,w);
  if(se==NULL)
    printf("\n  Khong co tu nao co cach phat am gan giong voi tu %s trong tu dien.\n",w);
  else
    {
      n=menu_soundex(se,w);
      m=search_mean(Dic,se->word[n-1]);
      print_mean(m,se->word[n-1]);
      free(m);
      free(se);
    }
  return;
}

void find_mean(BTA *Dic,BTA *Sou)
{
  char *tu,s[M],c,c1,pri[]="\n  Nhap vao tu can tra : ";
  int k1=0,i=0,rsize;
  mean *m;
  tu=(char*)malloc(M*sizeof(char));
  write(STDOUT_FILENO,pri,strlen(pri));


  tab_complt(Dic,s);
      strcpy(tu,s);
      m=search_mean(Dic,tu);
      if(m==NULL)
	{
	  printf("\n  Khong co tu %s trong tu dien.\n",tu);
	  printf("\n  Ban co muon xem cac tu co cach phat am gan giong voi tu %s khong?\n\n  Nhap 'y' neu dong y, nhap 'n' neu khong : ",tu);
	  c1=getchar();
	  while(c1!='y'&&c1!='n')
	    {
	      printf("\n  Hay nhap vao ki tu 'y' hoac 'n'.\n");
	      c1=getchar();
	    }
	  if(c1=='y')
	    soundex_search(Dic,Sou,tu);
	}
      else
	print_mean(m,tu);
      // }
      printf("-----------------------------------------------------------\n");
      return;
}

int del_word(BTA *Dic,BTA *Sou)
{
  char *tu;
  int rsize;
  soundex *s;
  printf("\n  Nhap vao tu can xoa : ");
  tu=nhap_xau();
  if(btdel(Dic,tu)!=0)
    {
      printf("\n  Khong ton tai tu %s trong tu dien.\n",tu);
      free(tu);   
      return 0;
    }
  else
    {
      s=search_soundex(Sou,tu);
      int i=0,j;
      char sou[10];
      soundEx(sou,tu,50,1);
      while(i<s->count)
	if(strcmp(s->word[i],tu)==0)
	  break;
	else
	  i++;
      for(j=i;j<s->count;j++)
	strcpy(s->word[j],s->word[j+1]);
      s->count--;
      btupd(Sou,sou,(char *)s,sizeof(soundex));
      printf("\n=====> Xoa thanh cong.\n");
      free(s);
    }
  free(tu);
  return 1;
}

char random_c()
{
  char ch;
  int l=rand()%26;
  ch='a'+l;
  return ch;
}

char *random_s()
{
  int dodai,i;
  char *s;
  dodai=rand()%10+1;
  s=(char*)malloc(10*sizeof(char));
  for(i=0;i<dodai;i++)
    s[i]=random_c();
  s[dodai]='\0';
  return s;
}


void doc_file(BTA *Dic,BTA *Sou,char name[])
{
  FILE *f;
  char word1[M],mean1[M];
  f=fopen(name,"r");
  if(f==NULL)
    {
      printf("\n  File %s khong ton tai.\n",name);
    }
  else
    {
      while(!feof(f))
	{
	  fscanf(f,"%[^\t]\t%[^\n]\n",word1,mean1);
	  add_word(Dic,Sou,word1,mean1);
	}
      fclose(f);
    }
}

void ghi_file(BTA *root,char name[])
{
  FILE *f;
  BTint in;
  int rsize,i;
  char *tu;
  mean *s;
  tu=(char *)malloc(M*sizeof(char));
  s=(mean*)malloc(sizeof(mean));
  f=fopen(name,"w");
  if(f==NULL)
    {
      printf("\n  Khong the mo file %s.\n",name);
      return;
    }
  btpos(root,ZSTART);
  while(bnxtky(root,tu,&in)==0)
    {
      btsel(root,tu,(char *)s,sizeof(mean),&rsize);
      for(i=0;i<s->count;i++)
	fprintf(f,"%s\t%s\n",tu,s->list[i]);
    }
  free(tu);
  free(s);
  fclose(f);
}

void ghi_file_sou(BTA *root,char name[])
{
  FILE *f;
  BTint in;
  int rsize,i;
  char *tu;
  soundex *s;
  tu=(char *)malloc(M*sizeof(char));
  s=(soundex*)malloc(sizeof(soundex));
  f=fopen(name,"w");
  if(f==NULL)
    {
      printf("\n  Khong the mo file %s.\n",name);
      return;
    }
  btpos(root,ZSTART);
  while(bnxtky(root,tu,&in)==0)
    {
      btsel(root,tu,(char *)s,sizeof(soundex),&rsize);
      for(i=0;i<s->count;i++)
	fprintf(f,"%s\t%s\n",tu,s->word[i]);
    }
  free(tu);
  free(s);
  fclose(f);
}


int main(int argc,char *argv[])
{
  int l=0,thay_doi=0;
  BTA *Dic;
  BTA *Sou;
  if(argc!=4)
    {
      printf("  Nhap doi so thu hai la ten file du lieu, doi so thu ba la ten file btree, doi so thu ba la ten file soundEx.\n");
      return 1;
    }
  btinit();
  Dic=btopn(argv[2],0,0);
  //btinit();
  Sou=btopn(argv[3],0,0);
  if(Dic==NULL||Sou==NULL)
    {
      Dic=btcrt(argv[2],0,FALSE);
      Sou=btcrt(argv[3],0,FALSE);
      doc_file(Dic,Sou,argv[1]);
    }
  while(l!=L)
    {
      l=luachon(menu_tudien,L);
      switch(l)
	{
	case 1:
	  {
	    char *word1;
	    char *mean1;
	    printf("\n  Nhap vao tu can them : ");
	    word1=nhap_xau();
	    printf("\n  Nhap vao nghia cua tu %s : ",word1);
	    mean1=nhap_xau();
	    if(add_word(Dic,Sou,word1,mean1)==0)
	      printf("\n  Tu %s va nghia da ton tai.\n",word1);
	    else
	      {
		thay_doi=1;
		printf("\n=====> Them thanh cong!\n");
	      }
	    break;
	  }
	case 2:
	  find_mean(Dic,Sou);
	  break;
	case 3:
	  if(del_word(Dic,Sou)==1)
	    thay_doi=1;
	  break;
	case 4:
	  printf("\n\t\tBYE BYE! SEE YOU AGAIN!\n\n");
	  break;
	}
    }
  if(thay_doi==1)
    {
      ghi_file(Dic,argv[1]);
      ghi_file_sou(Sou,"soundex1.txt");
    }
  btcls(Dic);
  btcls(Sou);
  return 0;
}
