#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define max 1000
#define add_size 100
int count = 1;
typedef struct {
  void *key;
  void *val;
} entry;

typedef struct {
  entry *data;
  int total;
  int size;
  entry (*makenode)(void *, void *);
  int (*compare)(void *, void *);
} phonebook;

void del() {
  while(getchar() != '\n');
}

entry makephonebook(void *phone, void *name) {
  entry e;
  e.key = malloc(sizeof(int));
  memcpy(e.key, phone, sizeof(int));
  e.val = strdup((char*)name);
  return e;
}

int comparephone(void *phone1, void *phone2) {
  return strcmp((char*)phone1, (char*)phone2);
}

int comparename(void *name1, void *name2) {
  return strcmp((char*)name1, (char*)name2);
}

phonebook create(entry make_node(void *, void *), int compare(void *, void *)) {
  phonebook book;
  book.makenode = make_node;
  book.compare = compare;
  book.total = 0;
  book.size = max;
  book.data = (entry*)malloc(book.size*sizeof(entry));
  return book;
}

entry *search(phonebook *book, void *val) {
  int i;
  entry *a;
  a = (entry*)malloc(sizeof(entry));
  for (i = 0; i < (*book).total; i++) {
    if ((*book).compare((*book).data[i].val, val) == 0) {
      *a = (*book).data[i];
      return a;
    }
  }
  return NULL;
}

void addentry(phonebook *book, void *key, void *val) {
  entry *new, e;
  if ((*book).total == (*book).size) {
    new = (entry*)malloc((*book).size*sizeof(entry));
    memcpy(new, (*book).data, (*book).size*sizeof(entry));
    free((*book).data);
    (*book).data = (entry*)malloc((count*add_size+max)*sizeof(entry));
    (*book).size = count*add_size+max;
    count++;
    memcpy((*book).data, new, (*book).total*sizeof(entry));
    free(new);
  }
  e = (*book).makenode(key, val);
  (*book).data[(*book).total] = e;
  (*book).total++;
}

void print(phonebook *book) {
  int i;
  printf("%-3s%-15s%-7s\n", "No", "Phone number", "Name");
  for (i = 0; i < (*book).total; i++) {
    printf("%-3d0%-15d%-30s\n", i+1, *(int*)(*book).data[i].key, (char*)((*book).data[i].val));
  }
}


void menu() {
  printf("\n\tMENU\n");
  printf("1. Them du lieu.\n");
  printf("2. Tim so dien thoai.\n");
  printf("3. In danh sach so dien thoai.\n");
  printf("4. Thoat.\n");
  printf("Nhap lua chon cua ban: ");
}


int main() {
  phonebook book;
  int num;
  char name[30];
  int choose = 0;
  book = create(makephonebook, comparename);
  FILE *fin;
  fin = fopen("phonebook.txt", "r");
  if (fin == NULL) {
    printf("%s\n", "File khong ton tai");
  } else {
    while (!feof(fin)) {
      fscanf(fin, "%d\t%[^\n]\n", &num, name);
      addentry(&book, &num, name);
    }
    fclose(fin);
  }

  do {
    menu();
    scanf("%d", &choose); del();
    while (choose < 1 || choose > 4) {
      printf("Khong co chuc nang nay vui long nhap lai\n");
      printf("lua chon cua ban la: ");
      scanf("%d", &choose); del();
    }
    switch(choose) {
    case 1: {
      FILE *fin;
      fin = fopen("phonebook.txt", "a");
      char name[30];
      int num;

      printf("\tName: "); scanf("%[^\n]", name); del();
      printf("\tNumber: "); scanf("%d", &num); del();
      addentry(&book, &num, name);
      fprintf(fin, "%d\t%s\n", num, name);
      fclose(fin);
      break;
    }
    case 2: {
      char name[30];
      entry *e;
      printf("Nhap ten can tim: ");
      scanf("%[^\n]", name); del();
      e = search(&book, name);
      if(e == NULL)
	printf(" Khong co ten can tim.\n");
      else
	printf("%s co so dien thoai la : %d\n", name, *(int*)((*e).key));
      break;
    }
    case 3: {
      print(&book);
      break;
    }
    case 4: break;
    }
  } while (choose != 4);

  return 0;
}
