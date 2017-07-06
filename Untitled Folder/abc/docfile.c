#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(){
	FILE *fp;
	char kitu[50];
	char *bump;

   fp = fopen("products.txt","r");
   if(fp == NULL){
   	printf("Khong mo duoc file product.txt\n");
   	exit(0);
   }   

   else
   {
   	while(fgets(kitu, 50, fp) != NULL){
   		bump = strtok(kitu," ");
   		while(bump != NULL){
   			printf("%d\n",atoi(bump));
   			bump = strtok(NULL," ");
   		}
   	}
}
}