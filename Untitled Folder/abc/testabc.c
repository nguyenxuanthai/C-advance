#include <stdio.h>
#include "jrb_graph2.h"

#define NUMBER_VERS 15
int main(int argc, char const *argv[])
{
	graph g;
	g = create_graph();

	if(argc <2 || argc > 5){
		printf("Dau` vao` sai\n");
		return 0;
	}

	//xu ly tham so ./p -h
	if(strcmp(argv[1],"-h") == 0){
		printf("C-advance\n");
	}

	//xu ly tham so: ./p -v demo.txt
	if(strcmp(argv[1], "-v") == 0){
		IS is;
		is = new_inputstruct(argv[2]);
		if(is == NULL){
			printf("Failed to file\n");
			exit(0);
		}

		while(get_line(is) >= 0){
			if(is->NF == 2){
				add_ver(g, atoi(is->fields[0]), new_jval_s(is->fields[1]));
				printf("%s %s\n",is->fields[0], is->fields[1]);

			}

			if(is->NF == 3){
				add_edge(g,atoi(is->fields[0]), atoi(is->fields[1]), new_jval_i(atoi(is->fields[2])));
				add_edge(g,atoi(is->fields[1]), atoi(is->fields[0]), new_jval_i(atoi(is->fields[2])));

			}
		}
			jettison_inputstruct(is);
			drop_graph(g);
	}
	return 0;
}