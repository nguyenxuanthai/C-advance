#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "btree.h"

void del() {
  while(getchar() != '\n');
}
void menu();
int pick(void (*menu)(void), int number);
char *entry_name();
char *entry_phonenumber();
BTA *add_entry(BTA *root);
void search_name(BTA *root);
void search_phonenumber(BTA *root);
void del_number(BTA *root);
void edit_phonenumber(BTA *root);
void print_tree(BTA *root);
void ghi_file(BTA *root, char file_name[]);
//////////////////////////////////////////////////////////////////////

int main() {
  int choose;
  BTA *root;
  char name[30], number[15];
  char file_name[] = "phonebook.txt";
  FILE *f;
  btinit();
  root = btcrt("phone", 0, FALSE);
  f = fopen(file_name, "r");
  if(f == NULL) {
    printf("\n  File %s khong ton tai.\n",file_name);
  }
  else {
    while(!feof(f)) {
      fscanf(f, "%[^\t]\t%s\n", name, number);
      btins(root, number, name, strlen(name)*sizeof(char));
    }
    fclose(f);
  }
  while(choose != 7) {
    choose = pick(menu, 7);
    switch(choose) {
    case 1:
      add_entry(root);
      printf("\n\n");
      break;
    case 2:
      search_phonenumber(root);
      printf("\n\n");
      break;
    case 3:
      search_name(root);
      printf("\n\n");
      break;
    case 4:
      edit_phonenumber(root);
      printf("\n\n");    
      break;
    case 5:
      del_number(root);
      printf("\n\n");
      break;
    case 6:
      printf("\nDanh sach cac so dien thoai trong phone book.\n\n");
      print_tree(root);
      printf("\n\n");
      break;
    case 7:
      break;
    }
  }
  ghi_file(root, file_name);
  btcls(root);
  return 0;
}

//////////////////////////////////////////////////////////////////////
void menu() {
  printf("\t\tPhone Book\n\n");
  printf("\t1. Them so dien thoai\n");
  printf("\t2. Tim kiem theo so dien thoai\n");
  printf("\t3. Tim kiem theo ten nguoi dung\n");
  printf("\t4. Chinh sua so dien thoai\n");
  printf("\t5. Xoa so dien thoai\n");
  printf("\t6. In danh ba dien thoai\n");
  printf("\t7. Thoat\n\n");
  printf("Chon chuc nang so: ");
}

int pick(void (*menu)(void), int number) {// number = so chuc nang ma menu co
  int chon;
  do {
    menu();
    while(scanf("%d", &chon) != 1) {
      while(getchar() != '\n');
      printf("Lua chon phai la so tu 1 den %d\n", number);
      menu();
    }
    while(getchar() != '\n');
    if (chon < 1 || chon > number) {
      printf("Chuong trinh chi co cac tinh nang tu 1 den %d\n", number);
    }
  } while(chon < 1 || chon > number);
  return chon;
}


char *entry_name() {
  char *s;
  s = (char*)malloc(sizeof(char) * 30);
  scanf("%[^\n]", s); del();
  return s;
}

char *entry_phonenumber() {
  char *num;
  int check = 0;
  num = (char*)malloc(sizeof(char) * 12);
  do {
    check = 0;
    printf("Nhap vao so dien thoai: ");
    scanf("%[^\n]", num); del();
    if (strlen(num) < 10 || strlen(num) > 11) {
      printf("So khong hop le. Vui long nhap lai\n");
      check = 1;
    } else 
      if (num[0] != '0') {
	printf("So khong hop le, Vui long nhap lai\n");
	check = 1;
      } else
	if (strcmp(num, "0100000000") < 0 || strcmp(num, "09999999999") > 0) {
	  printf("So khong hop le. Vui long nhap lai\n");
	  check = 1;
	}
  } while (check == 1);
  return num;
}


BTA *add_entry(BTA *root) {
  char *name, name1[30];
  char *number;
  int rsize;
  printf("Nhap thong tin so dien thoai muon them: \n");
  number = entry_phonenumber();
  printf("Nhap ten: ");
  name = entry_name();
  if (btsel(root, number, name1, 30, &rsize) == 0) {
    printf("So dien thoai %s da ton tai\n", number);
    free(name);
    free(number);
    return root;
  }
  btins(root, number, name, strlen(name)*sizeof(char));
  printf("Them so dien thoai thanh cong\n");
  free(name);
  free(number);
  return root;
}

void search_name(BTA *root) {
  char number[15];
  char *name, name1[30];
  int rsize, k = 0;
  BTint i;
  printf("Tim kiem so dien thoai theo ten nguoi dung.\n");
  printf("Nhap vao ten can tim kiem : ");
  name = entry_name();
  printf("Danh sach nhung so dien thoai cua %s la :\n", name);
  btpos(root, ZSTART);
  while(bnxtky(root, number, &i) == 0)
    {
      btsel(root, number, name1, 30, &rsize);
      if(strcmp(name, name1) == 0)
	{
	  k = 1;
	  printf("%s\t%s\n", name1, number);
	}
    }
  if(k == 0)
    printf("Khong co so dien thoai nao thuoc %s.\n",name);
}


void search_phonenumber(BTA *root) {
  char *number;	
  char *name;
  int rsize;
  name = (char*)malloc(30*sizeof(char));
  printf("Tim kiem nguoi dung theo so dien thoai.\n");
  number = entry_phonenumber();
  if(btsel(root, number, name, 30, &rsize) != 0)
    printf("Khong ton tai so dien thoai can tim trong phone book.\n");
  else {
    printf("Chu so huu so dien thoai %s la %s.\n", number, name);
  }
  printf("-----------------------------------------------------------\n");
  free(number);
  free(name);
  return;
}


void edit_phonenumber(BTA *root) {
  char *number;
  char *name, name1[30];
  int rsize;
  printf("Chinh sua thong tin cua mot so dien thoai.\n");
  number = entry_phonenumber();
  printf("Nhap vao ten chu so huu moi : ");
  name = entry_name();
  if(btsel(root, number, name1, 30, &rsize) != 0)
    printf("Khong ton tai so dien thoai can chinh sua trong phone book.\n");
  else {
    if(strcmp(name1, name) == 0) {
      printf("Khong thay doi ten nguoi dung so dien thoai %s.\n", number);
      free(number);
      free(name);
      return;
    }
    btupd(root, number, name, 30);
    printf("Chinh sua thanh cong.\n");
    free(number);
    free(name);
  }
}

void del_number(BTA *root) {
  char *number;
  printf("Xoa mot so dien thoai khoi phone book.\n");
  number = entry_phonenumber();
  if(btdel(root, number) != 0)
    printf("Khong ton tai so dien thoai %s trong phone book.\n", number);
  else {
    printf("Xoa thanh cong.\n");
  }
  free(number);
}

void print_tree(BTA *root) {
  BTint i;
  char number[15];
  char name[30];
  int rsize;
  btpos(root, ZSTART);
  while(bnxtky(root, number, &i) == 0) {
    btsel(root, number, name, 30, &rsize);
    printf("%-30s%-15s\n", name, number);
  }
}


void ghi_file(BTA *root, char file_name[]) {
  BTint i;
  char number[15];
  char name[30];
  int rsize;
  FILE *f;
  f = fopen(file_name, "w");
  if(f == NULL) {
    printf("Khong the mo file %s.\n", file_name);
    return;
  }
  btpos(root, ZSTART);
  while(bnxtky(root, number, &i) == 0) {
    btsel(root, number, name, 30, &rsize);
    fprintf(f, "%s\t%s\n", name, number);
  }
  fclose(f);
}
