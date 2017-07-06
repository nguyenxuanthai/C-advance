#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jrb.h"
#include "jval.h"

void del() {
  while(getchar() != '\n');
}


void menu();
int pick(void (*menu)(void), int number);
int compare_s(Jval a, Jval b);
JRB add_entry(JRB root, char *phonenumber, char *name, int (*compare)(Jval, Jval));
void print_phonebook(JRB root);
void del_phonenumber(JRB root, char *key, int (*compare)(Jval, Jval));
char *entry_phonenumber();



int main() {
  int choose = 0;
  JRB root;
  FILE *fin;
  char num[12], name[30];
  root = make_jrb();
  fin = fopen("phonebook.txt", "r");
  if (fin == NULL) {
    printf("Phone book rong\n");
  } else {
    while (!feof(fin)) {
      fscanf(fin, "%[^\t]\t%s\n", name, num); //del();
      add_entry(root, num, name, compare_s);
    }
    fclose(fin);
    print_phonebook(root);
    printf("___________________________________________________________\n\n");
  }

  while(choose != 5) {
    
    choose = pick(menu, 5);
    
    switch(choose) {
      
    case 1: {
      char *num1, name1[30];
      Jval key1;
      
      printf("\nNhap thong tin so dien thoai va nguoi dung:\n");
      num1 = entry_phonenumber();
      key1 = new_jval_s(num1);
      printf("Nhap vao ten nguoi su dung: ");
      scanf("%[^\n]", name1); del();
      if (jrb_find_gen(root, key1, compare_s) != NULL) {
	printf("\nDa ton tai so dien thoai %s trong phone book.\n", num1);
	break;
      }
      
      add_entry(root, num1, name1, compare_s);
      free(num1);
      printf("\nThem thanh cong.\n\n\n");
      break;
    }

    case 2: {
      JRB cur;
      Jval key1;
      char name1[30];
      printf("\nChinh sua ten nguoi su dung cua so dien thoai yeu cau.\n");
      if (root->parent==root) {
	printf("\nPhone book rong. Khong the thuc hien chuc nang nay.\n");
	break;
      }
      key1.s = strdup(entry_phonenumber());
      cur = jrb_find_gen(root, key1, compare_s);
      if (cur == NULL) {
	printf("\nKhong ton tai so dien thoai %s trong danh ba.\n", key1.s);
	break;
      }
      printf("Nhap vao ten nguoi dung can doi cho so dien thoai %s : ", key1.s);
      scanf("%[^\n]", name1); del();
      if (strcmp(cur->val.s, name1)==0)
	printf("Ban da khong thay doi ten nguoi so huu so dien thoai %s.\n\n\n",key1.s);
      else {
	cur->val.s = strdup(name1);
	printf("Thay doi thanh cong.\n\n\n");
      }
      break;
    }

    case 3: {
      char key[15];
      printf("\nXoa so dien thoai yeu cau.\n");
      if(root->parent == root) {
	printf("\nPhone book rong. Khong the thuc hien chuc nang nay.\n");
	break;
      }
      del_phonenumber(root, entry_phonenumber(), compare_s);
      break;
    }

    case 4: {
      print_phonebook(root);
      break;
    }

    case 5: break;
      
    }
    
  }


  JRB cur;
  fin = fopen("phonebook.txt", "w");
  jrb_traverse(cur,root)
    fprintf(fin ,"%s\t%s\n", cur->val.s, cur->key.s);
  fclose(fin);
  jrb_free_tree(root);
  return 0;
}


void menu() {
  printf("\t\tPhone Book\n\n");
  printf("\t1. Them so dien thoai\n");
  printf("\t2. Chinh sua so dien thoai\n");
  printf("\t3. Xoa so dien thoai\n");
  printf("\t4. In danh ba dien thoai\n");
  printf("\t5. Thoat\n\n");
  printf("Chon chuc nang so: ");
}

int pick(void (*menu)(void), int number) {// number = so chuc nang ma menu co
  int chon;
  do {
    menu();
    while(scanf("%d", &chon) != 1) {
      del();
      printf("Lua chon phai la so tu 1 den %d\n", number);
      menu();
    }
    del();
    if (chon < 1 || chon > number) {
      printf("Chuong trinh chi co cac tinh nang tu 1 den %d\n", number);
    }
  } while(chon < 1 || chon > number);
  return chon;
}


int compare_s(Jval a, Jval b) {
  return strcmp(a.s, b.s);
}

JRB add_entry(JRB root, char *phonenumber, char *name, int (*compare)(Jval, Jval)) {
  Jval key;
  Jval value;
  value.s = strdup(name);
  key.s = strdup(phonenumber);
  jrb_insert_gen(root, key, value, compare);
  return root;
}


void print_phonebook(JRB root) {
  JRB cur;
  if (root->parent == root) {
    printf("Phonebook rong\n");
    return;
  }
  printf("\tDu lieu trong PhoneBook la: \n\n");
  printf("\t\t%-30s%-15s\n", "Name", "Phone Number");
  jrb_traverse(cur, root) {
    printf("\t\t%-30s%-15s\n", cur->val.s, cur->key.s);
  }
  printf("\n");
}


void del_phonenumber(JRB root, char *key, int (*compare)(Jval, Jval)) {
  JRB cur;
  Jval k;
  k.s = strdup(key);
  cur = jrb_find_gen(root, k, compare);
  if (cur == NULL) {
    printf("Khong co so dien thoai nay trong phone book\n");
  } else {
    jrb_delete_node(cur);
    printf("Xoa thanh cong\n\n\n");
  }
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
