#include <stdio.h>
#include "jrb_graph2.h"

typedef struct product{
  int masp;
  char name[100];
}product;
int menu(){
  int n;
  printf("\n=======MENU==========\n\n");
  printf("1. In danh sach san pham.\n");
  printf("2. In danh sach giao dich.\n");
  printf("3. Hien thi muc do lien quang giua 2 san pham.\n");
  printf("4. In danh sach cac san pham lien quan.\n");
  printf("5. Hien thi moi lien he giua 2 san pham\n");
  printf("6. Thoat!\n\n");
  printf("\tBan chon: ");
  scanf("%d%*c",&n);
  while(n>6||n<1){
    printf("\tBan da nhap sai!!!\n");
    printf("Xin nhap lai(1->6): ");
    scanf("%d%*c",&n);
  }
  return n;
}

int main(){
  FILE *f1;
  FILE *f2;
  int m;
  IS is1;
  IS is2;
  graph g;
  JRB tmp;
  do{
    m=menu();
    switch(m){
    case 1:
      if((is1=new_inputstruct("products.txt"))==NULL){
        printf("Khong the doc file products.txt");
        return 0;
      }
      g=create_graph();
      while(get_line(is1)>=0){
        add_ver(g,atoi(is1->fields[0]),new_jval_s(strdup(is1->fields[1])));
      }
      jrb_traverse(tmp,g.vers){
        printf("Ma san pham: %d\n",jval_i(tmp->key));
        printf("Ten san pham: %s\n",jval_s(tmp->val));
        printf("\n");
      }
      break;
    case 2:
      if((is2=new_inputstruct("orderhistory.txt"))==NULL){
        printf("Khong the doc file orderhistory.txt");
        return 0;
      }
      while(get_line(is2)>=0){
        for(int i=0;i<is2->NF;i++){
          for(int j=i+1;j<is2->NF;j++){
            add_edge(g,i,j,new_jval_i(get_weight_i(g,j,i)+1));
          }
        }
        for(int i=is2->NF-1;i>=0;i--){
          for(int j=i-1;j>=0;j--){
            add_edge(g,j,i,new_jval_i(get_weight_i(g,i,j)+1));
          }
        }
        for(int i=0;i<is2->NF;i++){
          printf("%s ",get_val(g.vers,atoi(is2->fields[i])));
        }
        printf("\n");
      }
      /* while(get_line(is2)>=0){ */
      /*   add_edges(g,atoi(is2->fields[0]),atoi(is2->fields[1]), */
      /*            new_jval_i(atoi(is2->fields[2]))); */
      /*   add_edges(g,atoi(is2->fields[1]),atoi(is2->fields[0]), */
      /*            new_jval_i(atoi(is2->fields[2])));   */
      /* } */

      break;
    case 3:

      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
    }
  }while(m!=6);
}
