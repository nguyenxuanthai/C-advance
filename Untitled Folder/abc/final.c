#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "jrb_graph2.h"

void del(){
	while(getchar() != '\n');
}

void menu() {
  printf("\n\tMENU\n");
  printf("1. In danh sach san pham.\n");
  printf("2. In danh sach giao dich.\n");
  printf("3. Hien thi muc do lien quan giua 2 san pham.\n");
  printf("4. In danh sach san pham lien quan.\n");
  printf("5. Hien thi moi lien he giua 2 san pham.\n");
  printf("6. Thoat.\n");
  printf("Nhap lua chon cua ban: ");
}

int main(){
	graph g;
	g = create_graph();
	IS is;
	int i;
	int choose = 0;
	do{
		menu();
		scanf("%d", &choose);
		del(); 
		while(choose < 1 || choose > 6){
			printf("Khong co chuc nang nay. Vui long nhap lai tu 1 den 6: \n");
			printf("Lua chon cua ban la:");
			scanf("%d",choose);
			del();
		}
		switch(choose){
			case 1:

			is = new_inputstruct("products.txt");
			if(is == NULL){
				printf("Khong the mo file products.txt de doc.\n");
				exit(0);
			} 
			while(get_line(is) >= 0){
				if(is->NF == 2){
				add_ver(g, atoi(is->fields[0]), new_jval_s(is->fields[1]));
				printf("Ma san pham: %d \n",is->fields[0]);
				printf("Ten san pham: %s \n", is->fields[1]);
			}
		}
			
			
			break;
			

			case 2: break;
			
			
			case 3: break;
			

			case 4: break;
			

			case 5: break;
			

			case 6: break;
		}
	} while (choose != 6);
	return 0;
}