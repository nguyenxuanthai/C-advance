#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"jval.h"
#include"jrb.h"
#include"dllist.h"
#define MAX 1000

typedef struct
{
  JRB edges;
  JRB vers;
}
  graph;

typedef struct
{
  int ver[MAX];
  int count;
}
  way;

graph creat_graph();

void add_ver(graph g,int id,Jval name);
void add_edge(graph g,int ver1,int ver2,Jval val);
JRB find_adj(graph g,int ver); //tra ve canh co dinh ver
int has_edge(graph g,int v1,int v2); //Ham tra ve 1 neu v1<->v2, tra ve 21 neu v2->v1, tra ve 12 neu v1->v2. Nguoc lai tra ve 0.
int indegree(graph g,int ver,int *output);//Tra ve -1 neu khong co dinh ver
int outdegree(graph g,int ver,int *output);
int check_indegree(graph g,int *indegree);
int check_chu_trinh(graph g,int ver);
int check_cycle(graph g);
void drop_graph(graph g);



/////////////////////////////////////
graph creat_graph()
{
  graph g;
  g.vers=make_jrb();
  g.edges=make_jrb();
  return g;
}


void add_ver(graph g,int id,Jval name)
{
  JRB node,node1,tree,tree1;
  node=jrb_find_int(g.vers,id);
  if(node==NULL)
    jrb_insert_int(g.vers,id,name);
  node1=jrb_find_int(g.edges,id);
  if(node1==NULL)
    {
      tree1=make_jrb();
      jrb_insert_int(g.edges,id,new_jval_v(tree1));
    }
  return;
}

void add_edge(graph g,int ver1,int ver2,Jval val)
{
  JRB node1,node2;
  node1=jrb_find_int(g.edges,ver1);
  if(jrb_find_int((JRB)jval_v(node1->val),ver2)==NULL)
    jrb_insert_int((JRB)jval_v(node1->val),ver2,val);
}

JRB find_adj(graph g,int ver)
{
  JRB node;
  node=jrb_find_int(g.edges,ver);
  if(node==NULL)
    return NULL;
  return (JRB)jval_v(node->val);
}


int has_edge(graph g,int v1,int v2) {
  JRB adj,cur,adj1,cur1;
  adj=find_adj(g,v1);
  jrb_traverse(cur,adj)
    if(v2==jval_i(cur->key))
      {
	adj1=find_adj(g,v2);
	jrb_traverse(cur1,adj1)
	  if(v1==jval_i(cur1->key))
	    return 1;
	return 12;
      }
  adj1=find_adj(g,v2);
  jrb_traverse(cur1,adj1)
    if(v1==jval_i(cur1->key))
      return 21;
  return 0;
}

int indegree(graph g,int ver,int *output)
{
  JRB cur,adj,cur1;
  int total;
  total=0;
  if(jrb_find_int(g.edges,ver)==NULL)
    return 0;
  jrb_traverse(cur,g.edges)
    {
      adj=(JRB)jval_v(cur->val);
      jrb_traverse(cur1,adj)
	if(jval_i(cur1->key)==ver)
	  {
	    output[total]=jval_i(cur->key);
	    total++;
	  }
    }
  return total;
}

int outdegree(graph g,int ver,int *output)
{
  JRB adj,cur;
  int total;
  if(jrb_find_int(g.edges,ver)==NULL)
    return 0;
  adj=find_adj(g,ver);
  total=0;
  jrb_traverse(cur,adj)
    {
      output[total]=jval_i(cur->key);
      total++;              
    }
  return total;
}

int check_indegree(graph g,int *indegree)
{
  JRB cur;
  int k;
  k=0;
  jrb_traverse(cur,g.edges)
    if(indegree[jval_i(cur->key)]==0)
      k=1;
  return k;
}



int check_chu_trinh(graph g,int ver)
{
  Dllist stack, node;
  int visited[MAX];
  int output[MAX];
  int weight[MAX];
  int inde[MAX];
  int i;
  int u,n;
  JRB cur;
  stack=new_dllist();
  for(i=0;i<MAX;i++)
    {
      visited[i]=output[i]=weight[i]=0;
      inde[i]=-1;
    }
  jrb_traverse(cur,g.edges)
    inde[jval_i(cur->key)]=indegree(g,jval_i(cur->key),output);
  if(check_indegree(g,inde)==0)
    {
      // printf("\n ok.\n");
      return 1;
    }
  dll_append(stack,new_jval_i(ver));
  while(!dll_empty(stack))
    {
      node=dll_last(stack);
      u=jval_i(node->val);
      dll_delete_node(node);
      visited[u]=1;
      n=outdegree(g,u,output);
      for(i=0;i<n;i++)
	{
	  if(visited[output[i]]==0)
	    dll_append(stack,new_jval_i(output[i]));
	  if(output[i]==ver)
	    {
	      // printf("\n  %d-> %d\n",ver,u);
	      return 1;
	    }
	}
    }
  return 0;
}

int check_cycle(graph g)
{
  JRB cur;
  jrb_traverse(cur,g.edges)
    if(check_chu_trinh(g,jval_i(cur->key))==1)
      return 1;
  return 0;
}

  void drop_graph(graph g)
  {
    JRB cur,adj;
    jrb_traverse(cur,g.edges)
      {
	adj=(JRB)jval_v(cur->val);
	jrb_free_tree(adj);
      }
    jrb_free_tree(g.edges);
    jrb_free_tree(g.vers);
  }


