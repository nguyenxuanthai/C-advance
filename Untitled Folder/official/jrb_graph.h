
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "fields.h"
#include"jval.h"
#include"jrb.h"
#include"dllist.h"
#define MAX 1000

typedef JRB graph;

typedef struct
{
  int ver[MAX];
  int count;
}
  way;

graph create_graph();// Khởi tạo đồ thị ... Dong 73

int add_ver(graph g,int ver);// Thêm đỉnh vào đồ thị ... Dong 80
void add_edge(graph g,int ver1,int ver2,Jval val);// Thêm cạnh vào đồ thị, val là trọng số ... Dong 94

int del_ver(graph g,int ver);// Xoa dinh ... Dong 116
int del_edge(graph g,int ver1,int ver2);// Xoa cung ... Dong 136
void drop_graph(graph g);// Huy do thi ... Dong 152

void get_adj(graph g,int ver,way *adj);// adj luu cac dinh lien ke voi ver ... Dong 163
int check_adj(graph g,int ver1,int ver2);// Dong 176

int count_ver(graph g);// Trả về số đỉnh của đồ thị ... Dong 189
int count_edge(graph g);// Trả về số cạnh của đồ thị ... Dong 199
int ver_important(graph g);// Tra ve dinh ma viec xoa no se lam tang so thanh phan lien thong cua do thi ... Dong 213
int edge_important(graph g,int ver[]);// Canh ver[0] <-> ver[1] la canh ma viec xoa no se lam tang so thanh phan lien thong cua do thi ... Dong 268

int get_weight_edge_i(graph g,int ver1,int ver2);// Trả về trọng số của canh ver1 <-> ver2 ... Dong 291
float get_weight_edge_f(graph g,int ver1,int ver2);// Dong 304
int get_weight_way_i(graph g, way s);// tra ve trong so cua duong di s ... Dong 317
float get_weight_way_f(graph g, way s);// Dong 326

int get_all_ways(graph g,int start,int stop,way s[]);// tra ve so duong di tu start den stop ... Dong 336
int get_all_cycles(graph g,way s[]);// Tra ve so chu trinh trong do thi. s luu cac chu trinh ... Dong 399
int get_all_connecteds(graph g,way s[]);// Tra ve so thanh phan lien thong trong do thi ... Dong 518

int get_parent(int start,int stop,int parent[]);// Hàm trả xu ly mang parent ... Dong 538
int get_key(JRB root,char *val);// Lấy key ra từ JRB root ... Dong 556
char *get_val(JRB root,int key);// Lấy val có key ra từ root ... Dong 565

void bfs(graph g,int ver,int visited[],int *dem);// biến dem là số thành phần liên thông của đồ thị, khởi tạo dem=0, visited[i]=0, rồi mới truyền vào ... Dong 575
void BFS(graph g,int start,int stop,way *s);// Trả về một đường đi min ( theo cạnh) từ start đến stop ... Dong 604
void DFS(graph g,int start,int stop,way *s);// Giống BFS ở trên tuy nhiên chỉ đưa ra một đường đi ... Dong 648

int connecteds_max(graph g,way s[]); // Ham tra ve so thanh phan lien thong lon nhat, s luu cac dinh lien thong ... Dong 690
int connecteds_min(graph g,way s[]); // Ham tra ve so thanh phan lien thong nho nhat, s luu cac dinh lien thong ... Dong 711

void way_weight_min_i(graph g,int start,int stop,way *s);// Tra ve duong di min theo trong so ... Dong 733
void way_weight_min_f(graph g,int start,int stop,way *s);// Dong 779

void way_weight_max_i(graph g,int start,int stop,way *s);// Tra ve so dinh trong duong di co trong so lon nhat tu start den stop ... Dong 826
void way_weight_max_f(graph g,int start,int stop,way *s);// Dong 855

int adj_max(graph g,way *s);// Tra ve so canh lon nhat, s luu cac dinh co so canh lon nhat ... Dong 885
int adj_min(graph g,way *s);// Tra ve so canh nho nhat, s luu cac dinh co so canh nho nhat ... Dong 905

void DFS_dequy(graph g,int start,int stop,int *dem,Dllist stack,int *visited);// Biến dem cho biết số cách đi giữa hai đỉnh ... Dong 925

int get_choice(void (*menu)(void ),int soluachon);// Trả về lựa chọn ... Dong 959
int read_file(graph g,char file_name[]);// Đọc file dạng ma trận ví dụ ma trận trong đề thi của k56, ma trận kề ... Dong 979
void print_graph(graph g);// In ma trận kề ... Dong 1043
void print_weight_graph(graph g);// In ma trận trọng số ( bài k56).

JRB find_node(JRB root,Jval val,int (*compare)(Jval ,Jval ));
int add_entry(JRB root,int id,Jval val);// Dong 1011

graph create_graph()
{
  graph g;
  g=make_jrb();
  return g;
}

int add_ver(graph g,int ver)
{
  JRB node,edgeTree;
  node=jrb_find_int(g,ver);
  if(node==NULL)
    {
      edgeTree=make_jrb();
      jrb_insert_int(g,ver,new_jval_v(edgeTree));
      return 0;
    }
  return 1;
}

//Ham them cung.
void add_edge(graph g,int ver1,int ver2,Jval val)
{
  JRB node1,node2,newTree1,newTree2;
  node1=jrb_find_int(g,ver1);
  if(node1==NULL)
    {
      newTree1=make_jrb();
      node1=jrb_insert_int(g,ver1,new_jval_v(newTree1));
    }
  if(jrb_find_int((JRB)jval_v(node1->val),ver2)==NULL)
    jrb_insert_int((JRB)jval_v(node1->val),ver2,val);
  node2=jrb_find_int(g,ver2);
  if(node2==NULL)
    {
      newTree2=make_jrb();
      node2=jrb_insert_int(g,ver2,new_jval_v(newTree2));
    }
  if(jrb_find_int((JRB)jval_v(node2->val),ver1)==NULL)
    jrb_insert_int((JRB)jval_v(node2->val),ver1,val);
  return;
}

int del_ver(graph g,int ver)
{
  JRB cur,adj,cur1,cur2;
  int i;
  way s;
  cur=jrb_find_int(g,ver);
  if(cur==NULL)
    return 0;
  get_adj(g,ver,&s);
  for(i=0;i<s.count;i++)
    {
      cur2=jrb_find_int(g,s.ver[i]);
      adj=(JRB)jval_v(cur2->val);
      cur1=jrb_find_int(adj,ver);
      jrb_delete_node(cur1);
    }
  jrb_delete_node(cur);
  return 1;
}

int del_edge(graph g,int ver1,int ver2)
{
  JRB node,node1,adj,adj1;
  node=jrb_find_int(g,ver1);
  node1=jrb_find_int(g,ver2);
  if(node==NULL||node1==NULL)
    return 0;
  adj=jrb_find_int((JRB)jval_v(node->val),ver2);
  adj1=jrb_find_int((JRB)jval_v(node1->val),ver1);
  if(adj==NULL||adj1==NULL)
    return 0;
  jrb_delete_node(adj);
  jrb_delete_node(adj1);
  return 1;
}

void drop_graph(graph g)
{
  JRB cur,adj;
  jrb_traverse(cur,g)
    {
      adj=(JRB)jval_v(cur->val);
      jrb_free_tree(adj);
    }
  jrb_free_tree(g);
}

void get_adj(graph g,int ver,way *s)
{
  JRB adj,cur,cur1;
  (*s).count=0;
  cur=jrb_find_int(g,ver);
  if(cur==NULL)
    return;
  adj=(JRB)jval_v(cur->val);
  jrb_traverse(cur1,adj)
    (*s).ver[(*s).count++]=jval_i(cur1->key);
  return;
}

int check_adj(graph g,int ver1,int ver2)
{
  JRB cur,adj;
  cur=jrb_find_int(g,ver1);
  if(cur==NULL)
    return 0;
  adj=(JRB)jval_v(cur->val);
  if(jrb_find_int(adj,ver2)!=NULL)
    return 1;
  return 0;
}

// Ham tra ve so dinh cua do thi.
int count_ver(graph g)
{
  int count=0;
  JRB cur;
  jrb_traverse(cur,g)
    count++;
  return count;
}

// Ham tra ve so canh cua do thi.
int count_edge(graph g)
{
  int count=0;
  JRB cur,cur1,adj;
  jrb_traverse(cur,g)
    {
      adj=(JRB)jval_v(cur->val);
      jrb_traverse(cur1,adj)
	count++;
    }
  count=count/2;
  return count;
}

int ver_important(graph g)
{
  JRB cur,cur1;
  way *s;
  int n,i,j,k,l=0;
  way v,adj;
  for(i=0;i<MAX;i++)
    v.ver[i]=-1;
  v.count=0;
  s=(way*)malloc(MAX*sizeof(way));
  jrb_traverse(cur,g)
    jrb_traverse(cur1,g)
    {
      if(jval_i(cur->key)!=jval_i(cur1->key))
	{
	  n=get_all_ways(g,jval_i(cur->key),jval_i(cur1->key),s);
	  if(n==1)
	    {
	      if(s[0].count==2)
		{
		  get_adj(g,s[0].ver[0],&adj);
		  if(adj.count>1)
		    return s[0].ver[0];
		  else
		    {
		      get_adj(g,s[0].ver[1],&adj);
		      if(adj.count>1)
			return s[0].ver[1];
		    }
		}
	      if(s[0].count>2)
		return s[0].ver[1];
	    }
	  if(n>1)
	    {
	      for(i=0;i<s[0].count;i++)
		v.ver[i]=s[0].ver[i];
	      v.count=s[0].count;
	      for(i=1;i<n;i++)
		{
		  l=0;
		  for(j=0;j<v.count;j++)
		    for(k=0;k<s[i].count;k++)
		      if(v.ver[j]==s[i].ver[k])
			v.ver[l++]=s[i].ver[k];
		  v.count=l;
		}
	      if(v.count>2)
		return v.ver[1];
	    }
	}
    }
  return -1;
}

int edge_important(graph g,int *ver) //ver la 1 mang chua 2 dinh cua canh can xoa
{
  JRB cur;
  way s[MAX];
  int i;
  way adj;
  int visited[MAX]={0};
  jrb_traverse(cur,g)
    {
      get_adj(g,jval_i(cur->key),&adj);
      visited[jval_i(cur->key)]=1;
      for(i=0;i<adj.count;i++)
	if(!visited[adj.ver[i]])
	  if(get_all_ways(g,jval_i(cur->key),adj.ver[i],s)==1)
	    {
	      ver[0]=jval_i(cur->key);
	      ver[1]=adj.ver[i];
	      return 1;
	    }
    }
  return 0;
}

int get_weight_edge_i(graph g,int ver1,int ver2)
{
  JRB node,adj;
  node=jrb_find_int(g,ver1);
  if(node==NULL)
    return 0;
  adj=jrb_find_int((JRB)jval_v(node->val),ver2);
  if(adj==NULL)
    return 0;
  else
    return jval_i(adj->val);
}

float get_weight_edge_f(graph g,int ver1,int ver2)
{
  JRB node,adj;
  node=jrb_find_int(g,ver1);
  if(node==NULL)
    return 0;
  adj=jrb_find_int((JRB)jval_v(node->val),ver2);
  if(adj==NULL)
    return 0;
  else
    return jval_f(adj->val);
}

int get_weight_way_i(graph g,way s)
{
  int i,w;
  w=0;
  for(i=0;i<s.count-1;i++)
    w=w+get_weight_edge_i(g,s.ver[i],s.ver[i+1]);
  return w;
}

float get_weight_way_f(graph g,way s)
{
  int i;
  float w;
  w=0;
  for(i=0;i<s.count-1;i++)
    w=w+get_weight_edge_f(g,s.ver[i],s.ver[i+1]);
  return w;
}

int get_all_ways(graph g,int start,int stop,way s[])
{
  void DFS_way(graph g,int start,int stop,int *dem,Dllist stack,int *visited,int *parent,way s[])
  {
    int visited2[MAX];
    way output;
    int n,i,u,v;
    Jval ver;
    Dllist node;
    if(dll_empty(stack))
      {
	return;
      }
    node=dll_last(stack);
    u=jval_i(node->val);
    dll_delete_node(node);
    if(!visited[u])
      {               
	if(u==stop)
	  {
	    int pa[MAX];
	    for(i=0;i<MAX;i++)
	      pa[i]=-1;
	    memcpy(pa,parent,MAX*sizeof(int));
	    n=get_parent(start,stop,pa);
	    s[*dem].count=n;
	    for(i=0;i<s[*dem].count;i++)
	      s[*dem].ver[i]=pa[i];
	    (*dem)++;            	    
	  }
	visited[u]=1;
	get_adj(g,u,&output);
	for(i=0;i<output.count;i++)
	  {
	    v=output.ver[i];
	    if(!visited[v])
	      {
		dll_append(stack,new_jval_i(v));
		parent[v]=u;
		memcpy(visited2,visited,sizeof(int)*MAX);
		DFS_way(g,start,stop,dem,stack,visited2,parent,s);
	      }
	  }
      }
  }

  int dem;
  int visited[MAX];
  int parent[MAX];
  Dllist stack;
  int i;
  dem=0;
  for(i=0;i<MAX;i++)
    {
      visited[i]=0;
      parent[i]=-1;
    }
  stack=new_dllist();
  dll_append(stack,new_jval_i(start));
  DFS_way(g,start,stop,&dem,stack,visited,parent,s);
  return dem;
}

int get_all_cycles(graph g,way s[])
{

  int compare(way a,way b)
  {
    int i,j,k;
    if(a.count!=b.count)
      return 0;
    if(a.ver[0]==b.ver[0])
      {
	for(i=0;i<a.count;i++)
	  if(a.ver[i]!=b.ver[b.count-1-i])
	    return 0;
      }
    else
      {
	k=0;
	for(i=0;i<a.count;i++)
	  for(j=0;j<b.count;j++)
	    if(a.ver[i]==b.ver[j])
	      k++;
	if(k<a.count)
	  return 0;
      }
    return 1;
  }

  void DFS_cycle(graph g,int start,int stop,int *dem,Dllist stack,int *visited,int *parent,way s[])
  {
    int visited2[MAX];
    way output;
    int u,v,i;
    Dllist node;
    if(dll_empty(stack))
      {
	return;
      }
    node=dll_last(stack);
    u=jval_i(node->val);
    dll_delete_node(node);              
    if(!visited[u])
      {
	if(u==stop&&parent[u]!=start)
	  {
	    int pa[MAX],n1;
	    way a;
	    for(i=0;i<MAX;i++)
	      pa[i]=-1;
	    memcpy(pa,parent,MAX*sizeof(int));
	    n1=get_parent(start,stop,pa);
	    a.count=n1+1;
	    for(i=0;i<n1;i++)
	      a.ver[i]=pa[i];
	    a.ver[i]=start;
	    if((*dem)==0)
	      {
		s[*dem].count=a.count;
		for(i=0;i<a.count;i++)
		  s[*dem].ver[i]=a.ver[i];
		(*dem)++;
	      }
	    else
	      {
		int kt;
		kt=0;
		for(i=0;i<(*dem);i++)
		  if(compare(s[i],a)!=0)
		    kt=1;
		if(kt==0)
		  {
		    s[*dem].count=a.count;
		    for(i=0;i<a.count;i++)
		      s[*dem].ver[i]=a.ver[i];
		    (*dem)++;
		  }            	    
	      }
	  }
	visited[u]=1;
	get_adj(g,u,&output);
	for(i=0;i<output.count;i++)
	  {
	    v=output.ver[i];
	    if(!visited[v])
	      {
		dll_append(stack,new_jval_i(v));
		parent[v]=u;
		memcpy(visited2,visited,sizeof(int)*MAX);
		DFS_cycle(g,start,stop,dem,stack,visited2,parent,s);
	      }
	  }
      }
  }

  JRB cur;
  int dem,i,j,k,n;
  way out;
  int visited[MAX],parent[MAX],tham[MAX]={0};
  dem=0;
  jrb_traverse(cur,g)
    {
      get_adj(g,jval_i(cur->key),&out);
      tham[jval_i(cur->key)]=1;
      for(k=0;k<out.count;k++)
	{
	  if(!tham[out.ver[k]])
	    {
	      Dllist stack;
	      stack=new_dllist();
	      for(i=0;i<MAX;i++)
		visited[i]=parent[i]=0;
	      dll_append(stack,cur->key);
	      DFS_cycle(g,jval_i(cur->key),out.ver[k],&dem,stack,visited,parent,s);
	      free_dllist(stack);
	    }
	}
    }
  return dem;
}

int get_all_connecteds(graph g,way s[])
{
  int i,j,dem,visited[MAX];
  JRB cur;
  for(i=0;i<MAX;i++)
    visited[i]=0;
  dem=0;
  jrb_traverse(cur,g)
    if(!visited[jval_i(cur->key)])
      bfs(g,jval_i(cur->key),visited,&dem);
  for(i=0;i<dem;i++)
    {
      s[i].count=0;
      jrb_traverse(cur,g)
	if(visited[jval_i(cur->key)]==i+1)
	  s[i].ver[s[i].count++]=jval_i(cur->key);
    }
  return dem;
}

int get_parent(int start,int stop,int *parent)
{
  int b[MAX];
  int i,count,n;
  count=0;
  b[count++]=stop;
  while(parent[stop]!=start)
    {
      stop=parent[stop];
      b[count++]=stop;
    }
  b[count++]=start;
  n=count-1;
  for(i=0;i<count;i++)
    parent[i]=b[n--];
  return count;
}

int get_key(JRB root,char *val)
{
  JRB cur,adj,cur1;
  jrb_traverse(cur,root)
    if(strcmp(val,jval_s(cur->val))==0)
      return jval_i(cur->key);
  return -1;
}

char *get_val(JRB root,int key)
{
  JRB cur;
  cur=jrb_find_int(root,key);
  if(cur==NULL)
    return NULL;
  return jval_s(cur->val);
}

//Ham luu so thanh phan lien thong cua do thi trong bien dem, mang visited cho cac dinh lien thong.
void bfs(graph g,int ver,int visited[],int *dem)
{
  Dllist queue,node;
  int u,v,i;
  JRB adj,cur;
  way w;
  queue=new_dllist();
  dll_append(queue,new_jval_i(ver));
  (*dem)++;
  visited[ver]=*dem;
  while(!dll_empty(queue))
    {
      node=dll_first(queue);
      u=jval_i(node->val);
      dll_delete_node(node);
      get_adj(g,u,&w);
      for(i=0;i<w.count;i++)
	{
	  v=w.ver[i];
	  if(visited[v]==0)
	    {
	      visited[v]=*dem;
	      dll_append(queue,new_jval_i(v));
	    }
	}
    }
}

// Ham luu mot duong di tu start den stop vao mang parent.
void BFS(graph g,int start,int stop,way *s)
{
  int duyet[MAX],tham[MAX],parent[MAX];
  int u,i;
  Dllist node,queue;
  way w;
  for(i=0;i<MAX;i++)
    {
      duyet[i]=tham[i]=0;
      parent[i]=-1;
    }
  queue=new_dllist();
  dll_append(queue,new_jval_i(start));
  tham[start]=1;
  (*s).count=0;
  while(!dll_empty(queue))
    {
      node=dll_first(queue);
      u=jval_i(node->val);
      dll_delete_node(node);
      if(!duyet[u])
	{
	  duyet[u]=1;
	  get_adj(g,u,&w);
	  for(i=0;i<w.count;i++)
	    if(!tham[w.ver[i]])
	      {
		tham[w.ver[i]]=1;
		parent[w.ver[i]]=u;
		if(w.ver[i]==stop)
		  free_dllist(queue);
		dll_append(queue,new_jval_i(w.ver[i]));
	      }
	}
    }
  if(parent[stop]!=-1)
    {
      (*s).count=get_parent(start,stop,parent);
      for(i=0;i<(*s).count;i++)
	(*s).ver[i]=parent[i];
    }
  return;
}

void DFS(graph g,int start,int stop,way *s)
{
  int visited[MAX],parent[MAX];
  int u,i;
  Dllist node,stack;
  way w;
  (*s).count=0;
  for(i=0;i<MAX;i++)
    {
      visited[i]=0;
      parent[i]=-1;
    }
  stack=new_dllist();
  dll_append(stack,new_jval_i(start));
  while(!dll_empty(stack))
    {
      node=dll_last(stack);
      u=jval_i(node->val);
      dll_delete_node(node);
      if(!visited[u])
	{
	  visited[u]=1;
	  get_adj(g,u,&w);
	  for(i=0;i<w.count;i++)
	    if(!visited[w.ver[i]])
	      {
		parent[w.ver[i]]=u;
		if(w.ver[i]==stop)
		  free_dllist(stack);
		dll_append(stack,new_jval_i(w.ver[i]));
	      }
	}
    }
  if(parent[stop]!=-1)
    {
      (*s).count=get_parent(start,stop,parent);
      for(i=0;i<(*s).count;i++)
	(*s).ver[i]=parent[i];
    }
  return;
}

int connecteds_max(graph g,way s[])
{
  int dem,n,i,j,max;
  way w[MAX];
  n=0;
  dem=get_all_connecteds(g,w);
  max=w[0].count;
  for(i=1;i<dem;i++)
    if(max<w[i].count)
      max=w[i].count;
  for(i=0;i<dem;i++)
    if(max==w[i].count)
      {
	s[n].count=w[i].count;
	for(j=0;j<w[i].count;j++)
	  s[n].ver[j]=w[i].ver[j];
	n++;
      }
  return n;
}

int connecteds_min(graph g,way s[])
{
  int dem,n,i,j,min;
  way w[MAX];
  n=0;
  dem=get_all_connecteds(g,w);
  min=w[0].count;
  for(i=1;i<dem;i++)
    if(min>w[i].count)
      min=w[i].count;
  for(i=0;i<dem;i++)
    if(min==w[i].count)
      {
	s[n].count=w[i].count;
	for(j=0;j<w[i].count;j++)
	  s[n].ver[j]=w[i].ver[j];
	n++;
      }
  return n;
}

//Ham tra ve duong di co trong so ngan nhat tu start den stop.
void way_weight_min_i(graph g,int start,int stop,way *s)
{
  int visited[MAX],parent[MAX];
  int weight[MAX];
  int i,u,v;
  Dllist node,queue;
  way w;
  (*s).count=0;
  for(i=0;i<MAX;i++)
    {
      visited[i]=weight[i]=0;
      parent[i]=-1;
    }
  queue=new_dllist();
  dll_append(queue,new_jval_i(start));
  while(!dll_empty(queue))
     {
       node=dll_first(queue);
       u=jval_i(node->val);
       dll_delete_node(node);
       if(!visited[u])
	 {
	   visited[u]=1;
	   get_adj(g,u,&w);
	   for(i=0;i<w.count;i++) 
	     {
	       v=w.ver[i];      
	       if(weight[v]>weight[u]+get_weight_edge_i(g,u,v)||weight[v]==0)
		 {
		   parent[v]=u;
		   weight[v]=weight[u]+get_weight_edge_i(g,u,v);
		 }
	       if(!visited[v])
		 dll_append(queue,new_jval_i(v));
	     }
	 }
     }
  if(parent[stop]!=-1)
    {
      (*s).count=get_parent(start,stop,parent);
      for(i=0;i<(*s).count;i++)
	(*s).ver[i]=parent[i];
    }
  return;
}

void way_weight_min_f(graph g,int start,int stop,way *s)
{
  int visited[MAX],parent[MAX];
  float weight[MAX];
  int i,u,v;
  Dllist node,queue;
  way w;
  (*s).count=0;
  for(i=0;i<MAX;i++)
    {
      weight[i]=0;
      visited[i]=0;
      parent[i]=-1;
    }
  queue=new_dllist();
  dll_append(queue,new_jval_i(start));
  while(!dll_empty(queue))
    {
      node=dll_first(queue);
      u=jval_i(node->val);
      dll_delete_node(node);
      if(!visited[u])
	{
	  visited[u]=1;
	  get_adj(g,u,&w);
	  for(i=0;i<w.count;i++) 
	    {
	      v=w.ver[i];
	      if(weight[v]>weight[u]+get_weight_edge_f(g,u,v)||weight[v]==0)
		{
		  parent[v]=u;
		  weight[v]=weight[u]+get_weight_edge_i(g,u,v);
		}
	      if(!visited[v])
		dll_append(queue,new_jval_i(v));
	    }
	}
    }
  if(parent[stop]!=-1)
    {
      (*s).count=get_parent(start,stop,parent);
      for(i=0;i<(*s).count;i++)
	(*s).ver[i]=parent[i];
    }
  return;
}

void way_weight_max_i(graph g,int start,int stop,way *s)
{
  int dem;
  int weight[MAX];
  int i,j,max;
  way *w;
  (*s).count=0;
  w=(way*)malloc(MAX*sizeof(way));
  for(i=0;i<MAX;i++)
    weight[i]=0;
  dem=get_all_ways(g,start,stop,w);
  if(dem==0)
    return;
  for(i=0;i<dem;i++)
    weight[i]=get_weight_way_i(g,w[i]);
  max=weight[0];
  j=0;
  for(i=0;i<dem;i++)
    if(max<weight[i])
      {
	j=i;
	max=weight[i];
      }
  (*s).count=w[j].count;
  for(i=0;i<w[j].count;i++)
    (*s).ver[i]=w[j].ver[i];
  return;
}

void way_weight_max_f(graph g,int start,int stop,way *s)
{
  int dem,i,j;
  float weight[MAX];
  float max;
  Dllist stack;
  way *w;
  (*s).count=0;
  w=(way*)malloc(MAX*sizeof(way));
  for(i=0;i<MAX;i++)
    weight[i]=0;
  dem=get_all_ways(g,start,stop,w);
  if(dem==0)
    return;
  for(i=0;i<dem;i++)
    weight[i]=get_weight_way_f(g,w[i]);
  max=weight[0];
  j=0;
  for(i=0;i<dem;i++)
    if(max<weight[i])
      {
	j=i;
	max=weight[i];
      }
  (*s).count=w[j].count;
  for(i=0;i<w[j].count;i++)
    (*s).ver[i]=w[j].ver[i];
  return;
}

int adj_max(graph g,way *s)
{
  int max,dem[MAX]={0};
  JRB cur;
  way w;
  (*s).count=0;
  max=-1;
  jrb_traverse(cur,g)
    {
      get_adj(g,jval_i(cur->key),&w);
      dem[jval_i(cur->key)]=w.count;
      if(max<dem[jval_i(cur->key)])
	max=dem[jval_i(cur->key)];
    }
  jrb_traverse(cur,g)
    if(max==dem[jval_i(cur->key)])
      (*s).ver[(*s).count++]=jval_i(cur->key);
  return max;
}

int adj_min(graph g,way *s)
{
  int min,dem[MAX]={0};
  JRB cur;
  way w;
  min=MAX+1;
  (*s).count=0;
  jrb_traverse(cur,g)
    {
      get_adj(g,jval_i(cur->key),&w);
      dem[jval_i(cur->key)]=w.count;
      if(min>dem[jval_i(cur->key)])
	min=dem[jval_i(cur->key)];
    }
  jrb_traverse(cur,g)
    if(min==dem[jval_i(cur->key)])
      (*s).ver[(*s).count++]=jval_i(cur->key);
  return min;
}

void DFS_dequy(graph g,int start,int stop,int *dem,Dllist stack,int *visited)
{
  int visited2[MAX];
  int i,u,v;
  Jval ver;
  Dllist node;
  way w;
  //Ket thuc de quy   
  if(dll_empty(stack))
    {
      return;
    }
  node=dll_last(stack);
  u=jval_i(node->val);
  dll_delete_node(node);
  if(!visited[u])
    {             
      if(u==stop)
	(*dem)++;
      visited[u]=1;
      get_adj(g,u,&w);
      for(i=0;i<w.count;i++)
	{
	  v=w.ver[i];
	  if(!visited[v])
	    {
	      dll_append(stack,new_jval_i(v));
	      memcpy(visited2,visited,sizeof(int)*MAX);
	      DFS_dequy(g,start,stop,dem,stack,visited2);
	    }
	}
    }
}

int get_choice(void (*menu)(void ),int soluachon)
{
  int luachon;
  do
    {
      menu();
      while(scanf("%d",&luachon)!=1)
	{
	  while(getchar()!='\n');
	  printf("\n  !!! Nhap sai! Hay nhap vao so tu 1 den %d!\n",soluachon);
	  menu();
	}
      while(getchar()!='\n');
      if(luachon<1||luachon>soluachon)
	printf("\n  !!! Nhap sai! Hay nhap vao so tu 1 den %d!\n",soluachon);
    }
  while(luachon<1||luachon>soluachon);
  return luachon;
}

int read_file(graph g,char file_name[])
{
  FILE *f;
  int count_ver=0;
  int i,j,n;
  char s[100];
  f=fopen(file_name,"r");
  if(f==NULL)
    {
      printf("\n  File %s khong ton tai.\n",file_name);
      return 0;
    }
  else
    {
      while(fgets(s,100,f)!=NULL)
	count_ver++;
      rewind(f);
      for(i=1;i<=count_ver;i++)
	{
	  add_ver(g,i);
	  for(j=1;j<=count_ver;j++)
	    {
	      fscanf(f,"%d",&n);
	      if(i!=j&&n!=0)
		add_edge(g,i,j,new_jval_i(n));
	    }
	}
    }
  return count_ver;
}

int read_file_f(graph g,char file_name[])
{
  FILE *f;
  int count_ver=0;
  int i,j;
  float n;
  char s[100];
  f=fopen(file_name,"r");
  if(f==NULL)
    {
      printf("\n  File %s khong ton tai.\n",file_name);
      return 0;
    }
  else
    {
      while(fgets(s,100,f)!=NULL)
	count_ver++;
      rewind(f);
      for(i=1;i<=count_ver;i++)
	{
	  add_ver(g,i);
	  for(j=1;j<=count_ver;j++)
	    {
	      fscanf(f,"%f",&n);
	      if(i!=j&&n!=0)
		add_edge(g,i,j,new_jval_f(n));
	    }
	}
    }
  return count_ver;
}

// In ma tran lien ke.
void print_graph(graph g)
{
  JRB cur,cur1;
  jrb_traverse(cur,g)
    {
      jrb_traverse(cur1,g)
	if(check_adj(g,jval_i(cur->key),jval_i(cur1->key))!=0)
	  printf(" 1");
	else
	  printf(" 0");
      printf("\n");
    }
}

//In ma tran trong so.
void print_weight_graph(graph g)
{
  JRB cur,cur1,node;
  jrb_traverse(cur,g)
    {
      jrb_traverse(cur1,g)
	if(check_adj(g,jval_i(cur->key),jval_i(cur1->key))!=0)
	  {
	    node=jrb_find_int((JRB)jval_v(cur->val),jval_i(cur1->key));
	    printf("%-3d",jval_i(node->val));
	  }
	else
	  printf("%-3d",0);
      printf("\n");
    }
}

void print_weight_graph_f(graph g)
{
  JRB cur,cur1,node;
  jrb_traverse(cur,g)
    {
      jrb_traverse(cur1,g)
	if(check_adj(g,jval_i(cur->key),jval_i(cur1->key))!=0)
	  {
	    node=jrb_find_int((JRB)jval_v(cur->val),jval_i(cur1->key));
	    printf("%3.0f",jval_f(node->val));
	  }
	else
	  printf("%3d",0);
      printf("\n");
    }
}

JRB find_node(JRB root,Jval val,int (*compare)(Jval ,Jval ))
{
  JRB cur;
  jrb_traverse(cur,root)
    if(compare(cur->val,val)==0)
      return cur;
  return NULL;
}

int add_entry(JRB root,int id,Jval val)
{
  if(jrb_find_int(root,id)==NULL)
    {
      jrb_insert_int(root,id,val);
      return 1;
    }
  return 0;
}

int check_cycle(graph g)
{
  int visited[MAX]={0};
  way w;
  int i,u,v;
  JRB cur;
  Dllist queue,node;
  queue=new_dllist();
  jrb_traverse(cur,g)
    if(!visited[jval_i(cur->key)])
      {
	dll_append(queue,cur->key);
	visited[jval_i(cur->key)]=1;
	while(!dll_empty(queue))
	  {
	    node=dll_first(queue);
	    u=jval_i(node->val);
	    dll_delete_node(node);
	    visited[u]=2;
	    get_adj(g,u,&w);
	    for(i=0;i<w.count;i++)
	      {
		if(visited[w.ver[i]]==1)
		  return 1;
		else
		  if(visited[w.ver[i]]==0)
		    {
		      visited[w.ver[i]]=1;
		      dll_append(queue,new_jval_i(w.ver[i]));
		    }
	      }
	  }
      }
  return 0;
}


int cap_dinh(graph g,way *s,int x,int x1)// Ham tra ve so cap dinh co >= x dinh lien ke chung. Neu x1==0 thi cap dinh do khong lien ke. Neu x1==1 thi cap dinh do lien ke. Neu x1>1 thi cap dinh do co the lien ke hoac khong.
{
  int compare(way a,way b)
  {
    if((a.ver[0]==b.ver[0]&&a.ver[1]==b.ver[1])||(a.ver[0]==b.ver[1]&&a.ver[1]==b.ver[0]))
      return 1;
    return 0;
  }
  int check(int ver,way b)
  {
    int i;
    for(i=0;i<b.count;i++)
      if(ver==b.ver[i])
	return 1;
    return 0;
  }
  int i,j,l,l1,k,dem;
  JRB cur;
  way a[5];
  dem=0;
  jrb_traverse(cur,g)
    {
      get_adj(g,jval_i(cur->key),&a[0]);
      if(a[0].count>=x)
	{
	  for(i=0;i<a[0].count;i++)
	    {
	      get_adj(g,a[0].ver[i],&a[1]);
	      if(a[1].count>=x)
		{
		  for(j=0;j<a[1].count;j++)
		    {
		      if(a[1].ver[j]!=jval_i(cur->key))
			{
			  k=0;
			  get_adj(g,a[1].ver[j],&a[2]);
			  if((x1==0&&check(a[1].ver[j],a[0])==0)||(x1==1&&check(a[1].ver[j],a[0])==1)||(x1>1))
			    {
			      for(l=0;l<a[0].count;l++)
				for(l1=0;l1<a[2].count;l1++)
				  if(a[0].ver[l]==a[2].ver[l1])
				    k++;
			    }
			  if(k>=x)
			    {
			      if(dem==0)
				{	  
				  s[dem].count=2;
				  s[dem].ver[0]=jval_i(cur->key);
				  s[dem].ver[1]=a[1].ver[j];
				  dem++;	
				}
			      else
				{
				  k=0;
				  a[3].ver[0]=jval_i(cur->key);
				  a[3].ver[1]=a[1].ver[j];
				  for(l=0;l<dem;l++)
				    if(compare(a[3],s[l])==1)
				      k=1;
				  if(k==0)
				    {
				      s[dem].count=2;
				      s[dem].ver[0]=jval_i(cur->key);
				      s[dem].ver[1]=a[1].ver[j];
				      dem++;
				    }
				}
			    }
			}
		    }
		}
	    }
	}
    }
  return dem;
}


// Ham tra ve cac dinh co so dinh lien ke chung voi dinh ver >=x ;
int dinh_chung_thoa_man(graph g,int ver,int *vertive,int x)
{
  int compare(way a,way b,int x1)
  {
    int i,j,k;
    k=0;
    for(i=0;i<a.count;i++)
      for(j=0;j<b.count;j++)
	if(a.ver[i]==b.ver[j])
	  k++;
    if(k>=x1)
      return 1;
    return 0;
  }
  int i;
  way a;
  way b;
  way c;
  JRB cur;
  int parent[MAX]={0};
  int visited[MAX]={0};
  int dem,n;
  dem=0;
  get_adj(g,ver,&a);
  if(a.count<x)
    return 0;
  bfs(g,ver,visited,&dem);
  c.count=0;
  jrb_traverse(cur,g)
    if(visited[jval_i(cur->key)]==dem&&jval_i(cur->key)!=ver)
      c.ver[c.count++]=jval_i(cur->key);
  dem=0; 
  for(i=0;i<c.count;i++)
    {
      get_adj(g,c.ver[i],&b);
      if(b.count>=x)
	if(compare(a,b,x)==1)
	  vertive[dem++]=c.ver[i];
    }
  return dem;
}
