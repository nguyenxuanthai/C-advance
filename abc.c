#include <stdio.h>
#include "jrb_graph2.h"

int main(int argc, char *argv[])
{
  graph g;
  g = create_graph();
  
  if (argc < 2 || argc > 5) {
    printf("Dau vao chua dung vui long nhap lai\n");
    return 0;
  }

  //test1  
  if (strcmp(argv[1], "-h") == 0) {
    printf("C-advance, hk 20152\n");
  }

  
  //test2  
  if (strcmp(argv[1], "-v") == 0) {
    IS is;
    int i;
    is = new_inputstruct(argv[2]);
    if (is == NULL) {
      printf("Khong the mo file %s de doc\n", argv[2]);
      exit(0);
    }

    while (get_line(is) >= 0) {
      if (is->NF == 2) {
	add_ver(g, atoi(is->fields[0]), new_jval_s(is->fields[1]));
	printf("%s %s\n", is->fields[0], is->fields[1]);
      }
      if (is->NF == 3) {
	add_edge(g, atoi(is->fields[0]), atoi(is->fields[1]), new_jval_i(atoi(is->fields[2])));
	add_edge(g, atoi(is->fields[1]), atoi(is->fields[0]), new_jval_i(atoi(is->fields[2])));
      }
    }
    jettison_inputstruct(is);
    drop_graph(g);
    //    wei = get_weight_i(g, 1, 3);
    //    printf("%d\n", wei);
  }


  //test3  
  if (strcmp(argv[1], "-w") == 0) {
    IS is;
    int i, wei;
    is = new_inputstruct(argv[2]);
    if (is == NULL) {
      printf("Khong the mo file %s de doc\n", argv[2]);
      exit(0);
    }

    while (get_line(is) >= 0) {
      if (is->NF == 2) {
	add_ver(g, atoi(is->fields[0]), new_jval_s(is->fields[1]));
	//	printf("%s %s\n", is->fields[0], is->fields[1]);
      }
      if (is->NF == 3) {
	add_edge(g, atoi(is->fields[0]), atoi(is->fields[1]), new_jval_i(atoi(is->fields[2])));
	add_edge(g, atoi(is->fields[1]), atoi(is->fields[0]), new_jval_i(atoi(is->fields[2])));
      }
    }
    jettison_inputstruct(is);
    wei = get_weight_i(g, atoi(argv[3]), atoi(argv[4]));
    printf("%d\n", wei);
    drop_graph(g);
  }


  //test4
  if (strcmp(argv[1], "-p") == 0) {
    IS is;
    int i, wei, ver[10], dem;
    JRB cur;
    is = new_inputstruct(argv[2]);
    if (is == NULL) {
      printf("Khong the mo file %s de doc\n", argv[2]);
      exit(0);
    }

    while (get_line(is) >= 0) {
      if (is->NF == 2) {
	add_ver(g, atoi(is->fields[0]), new_jval_s(strdup(is->fields[1])));
	//	printf("%s\n", is->fields[1]);
      }
      if (is->NF == 3) {
	add_edge(g, atoi(is->fields[0]), atoi(is->fields[1]), new_jval_i(atoi(is->fields[2])));
	add_edge(g, atoi(is->fields[1]), atoi(is->fields[0]), new_jval_i(atoi(is->fields[2])));
      }
    }
    jettison_inputstruct(is);
    
    dem = way_weight_min_i(g, atoi(argv[3]), atoi(argv[4]), &wei, ver);
    printf("%d\n", wei);
    for (i = 0; i < dem; i++) {
      //      printf("%d\n", ver[i]);
      cur = jrb_find_int(g.vers, ver[i]);
      //      printf("%d\n", cur->key);
      printf("%s\n", cur->val);
    }
    drop_graph(g);
  }


  //test5
  if (strcmp(argv[1], "-n") == 0) {
    IS is;
    int i, wei,ver[10], dem;
    JRB cur;
    is = new_inputstruct(argv[2]);
    if (is == NULL) {
      printf("Khong the mo file %s de doc\n", argv[2]);
      exit(0);
    }

    while (get_line(is) >= 0) {
      if (is->NF == 2) {
	add_ver(g, atoi(is->fields[0]), new_jval_s(strdup(is->fields[1])));
	//	printf("%s\n", is->fields[1]);
      }
      if (is->NF == 3) {
	add_edge(g, atoi(is->fields[0]), atoi(is->fields[1]), new_jval_i(atoi(is->fields[2])));
	add_edge(g, atoi(is->fields[1]), atoi(is->fields[0]), new_jval_i(atoi(is->fields[2])));
      }
    }
    jettison_inputstruct(is);

    dem = indegree(g, atoi(argv[3]), ver);
    printf("%d\n", dem);
    for (i = 0; i < dem; i++) {
      cur = jrb_find_int(g.vers, ver[i]);
      printf("%d %s\n", cur->key, cur->val);
    }
    drop_graph(g);
  }


  //test6
  if (strcmp(argv[1], "-s") == 0) {
    IS is;
    int i, wei,ver[10], dem;
    JRB cur;
    is = new_inputstruct(argv[2]);
    if (is == NULL) {
      printf("Khong the mo file %s de doc\n", argv[2]);
      exit(0);
    }

    while (get_line(is) >= 0) {
      if (is->NF == 2) {
	add_ver(g, atoi(is->fields[0]), new_jval_s(strdup(is->fields[1])));
	//	printf("%s\n", is->fields[1]);
      }
      if (is->NF == 3) {
	add_edge(g, atoi(is->fields[0]), atoi(is->fields[1]), new_jval_i(atoi(is->fields[2])));
	add_edge(g, atoi(is->fields[1]), atoi(is->fields[0]), new_jval_i(atoi(is->fields[2])));
      }
    }
    jettison_inputstruct(is);

    
  }


  
  return 0;
}
