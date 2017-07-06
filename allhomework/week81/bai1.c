#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "graph.h"


int main() {
  graph g;
  JRB cur, cur1;
  g = creat_graph();
  add_ver(g, 0, new_jval_s("V0"));
  add_ver(g, 1, new_jval_s("V1"));
  add_ver(g, 2, new_jval_s("V2"));
  add_ver(g, 3, new_jval_s("V3"));
  add_edge(g, 0, 1, new_jval_i(1));
  add_edge(g, 1, 2, new_jval_i(1));
  add_edge(g, 2, 0, new_jval_i(1));
  add_edge(g, 1, 3, new_jval_i(1));

  jrb_traverse(cur, g.vers)
    printf("%d\t%s\n", cur->key, jval_s(cur->val));
  jrb_traverse(cur1, g.edges)
    {
      JRB adj, cur2;
  
      adj = (JRB)jval_v(cur1->val);
      jrb_traverse(cur2, adj)
	printf("%d\t%d\n", jval_i(cur2->key), jval_i(cur2->val));
    }

  ////////////////////
  int i;
  i = check_cycle(g);
  if (i == 1) {
    printf("Do thi nay co chu trinh\n");
  }
  return 0;
}
