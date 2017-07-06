#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"jval.h"
#include"jrb.h"
#include"dllist.h"
#include "fields.h"
#define MAX 1000

typedef struct //co huong
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

graph create_graph();// Dong 92

void add_ver(graph g,int id,Jval val);// Dong 100
void add_edge(graph g,int ver1,int ver2,Jval weight);//Ham them cung : ver1->ver2 tai g.edges,trong so val. Dong 115
int add_entry(JRB root,int id,Jval val);// Dong 123

int del_edge(graph g,int ver1,int ver2);// Xoa canh ver1->ver2. 156
int del_ver(graph g,int ver);// Xoa dinh. Dong 133

int has_edge(graph g,int v1,int v2);//Ham tra ve 1 neu v1<->v2, tra ve 21 neu v2->v1, tra ve 12 neu v1->v2. Nguoc lai tra ve 0.  178
int indegree(graph g,int ver,int output[]);// Tra ve -1 neu khong co dinh ver. Dong 198
int outdegree(graph g,int ver,int output[]);// Dong 218.

int count_ver(graph g);// Dong 258
int count_edge(graph g);// Dong 267

char *get_val(JRB root,int id);// Dong 280
int get_key(JRB root,char name[]);// Tra ve -1 neu khong co name. 289
int get_parent(graph g,int start,int stop,int parent[]);// Dong 298
int get_weight_i(graph g,int ver1,int ver2);// tra ve trong so cua canh ver1->ver2.  Dong 318
float get_weight_f(graph g,int ver1,int ver2);// Dong 331

int get_all_ways(graph g,int start,int stop,way s[]);// Tra ve so duong di giua start va stop.  Dong 344
int get_all_cycles(graph g,way s[]);// Dong 1140
int get_weight_way_i(graph g, way s);//Tra ve trong so cua duong di s. 362
float get_weight_way_f(graph g,way s);// Dong 371


int connected_before_nsteps(graph g,int ver,int *before_nsteps,int n);// Dong 1522
int connected_after_nsteps(graph g,int ver,int *after_nsteps,int n);// Dong 1560
int sort(graph g,int ver[]);// Dong 453

int weight_max_i(graph g,int ver,int *parent);// Duyet tat ca do thi, tra ve trong so lon nhat cua dinh ver. Cung la thoi gian toi thieu de hoan thanh cong viec ver Kết hợp với hàm get_parent để có được đường đi.  Dong 495
float weight_max_f(graph g,int ver,int *parent);// Dong 545
int weight_min_i(graph g,int ver,int *parent);//  Dong 596
float weight_min_f(graph g,int ver,int *parent);// Dong 646

int way_weight_max_i(graph g,int start,int stop,int *max_weight,int *vers);// Tra ve so dinh trong duong di max. max_weight la trong luong max, ver la mang cac dinh trong duong di max theo thu tu. Dong 698
int way_weight_max_f(graph g,int start,int stop,float *max_weight,int *vers); // Dong 735
int way_weight_min_i(graph g,int start,int stop,int *min_weight,int *vers);// Tra ve so dinh trong duong di min. min_weight la trong luong min, ver la mang cac dinh trong duong di min theo thu tu. Dong 773
int way_weight_min_f(graph g,int start,int stop,int *min_weight,int *vers);//  Dong 811

void duyet_bfs(graph g,int s,int *visited,int *parent,int *dem);// biến dem là số thành phần liên thông của đồ thị, khởi tạo dem=0, rồi mới truyền vào.  Dong 850
void duyet_dfs(graph g,int s,int *visited,int *parent,int *dem);// Như hàm trên.  Dong 879

void BFS_min(graph g,int start,int stop,int *parent);// trong so min dong 907
void BFS_min_f(graph g,int start,int stop,int *parent);// Dong 941
int BFS(graph g,int start,int stop,int *parent);// min theo canh, tra ve 0 neu khong co duong di. Dong 976.
int DFS(graph g,int start,int stop,int *parent);// Dong 1009


int check_chu_trinh(graph g,int ver);// Tra ve 1 neu ton tai. nguoc lai tra ve 0. Dong 1086
int check_cycle(graph g);// Tra ve 1 neu ton tai, nguoc lai tra ve 0. Dong 1131


int get_choice(void (*menu)(void ),int soluachon);// Trả về lựa chọn. Dong 1258
int read_file(graph g,char file_name[]);// Dong 1278
int read_file_f(graph g,char file_name[]);// Dong 1309

int dinh_chot(graph g);// Tra ve dinh tim duoc. tra ve -1 neu khong co. Dong 1402
int canh_chot(graph g,int *ver);// Tra ve 1 neu tim duoc. Dong 1452
int lien_thong_max(graph g,way *s);// Dong 1475
void drop_graph(graph g);// 1510
JRB find_adj(graph g,int ver);
void DFS_(graph g,int start,int stop,int *dem,Dllist stack,int *visited,int *parent,way *s);




graph create_graph()
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
if(node1!=NULL)
  if(jrb_find_int((JRB)jval_v(node1->val),ver2)==NULL)
    jrb_insert_int((JRB)jval_v(node1->val),ver2,val);
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

int del_ver(graph g,int ver)
{
  JRB cur,adj,cur1,cur2;
  int output[MAX]={0};
  int i,n;
  cur=jrb_find_int(g.vers,ver);
  if(cur==NULL)
    return 0;
  jrb_delete_node(cur);
  cur2=jrb_find_int(g.edges,ver);
  if(cur2==NULL)
    return 0;
  n=indegree(g,ver,output);
  for(i=0;i<n;i++)
    {
      adj=find_adj(g,output[i]);
      cur1=jrb_find_int(adj,ver);
      jrb_delete_node(cur1);
    }
  jrb_delete_node(cur2);
  return 1;
}

int del_edge(graph g,int ver1,int ver2)
{
  JRB node,node1,adj,adj1;
  node=jrb_find_int(g.edges,ver1);
  if(node==NULL)
    return 0;
  adj=jrb_find_int((JRB)jval_v(node->val),ver2);
  if(adj==NULL)
    return 0;
  jrb_delete_node(adj);
  return 1;
}

JRB find_adj(graph g,int ver)
{
  JRB node;
  node=jrb_find_int(g.edges,ver);
  if(node==NULL)
    return NULL;
  return (JRB)jval_v(node->val);
}

int has_edge(graph g,int v1,int v2)
{
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
  if (adj == NULL) {
  adj1=find_adj(g,v2);
  jrb_traverse(cur1,adj1)
    if(v1==jval_i(cur1->key))
      return 21;
  }
  return 0;
}

int indegree(graph g,int ver,int *output) //cac dinh tro toi dinh ver duoc luu trong mang output
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

int outdegree(graph g,int ver,int *output) //dinh ver tro toi cac dinh duoc luu o mang output (ham tra ve so luong cac dinhh ma dinh ver huong toi)
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

int lienke(graph g,int ver,int *output)
{
  int op[MAX],op1[MAX],vi[MAX];
  int n,n1,i,count;
  count=0;
  for(i=0;i<MAX;i++)
    vi[i]=0;
  n=outdegree(g,ver,op);
  n1=indegree(g,ver,op1);
  for(i=0;i<n;i++)
    {
      output[count++]=op[i];
      vi[op[i]]=1;
    }
  for(i=n;i<n+n1;i++)
    if(!vi[op1[i-n]])
      {
	output[count++]=op1[i-n];
	vi[op1[i-n]]=1;
      }
  return count;
}

int count_ver(graph g)
{
  int count=0;
  JRB cur;
  jrb_traverse(cur,g.vers)
    count++;
  return count;
}

int count_edge(graph g)
{
  int count=0;
  JRB cur,cur1,adj;
  jrb_traverse(cur,g.edges)
    {
      adj=(JRB)jval_v(cur->val);
      jrb_traverse(cur1,adj)
	count++;
    }
  return count;
}

char *get_val(JRB root,int id)
{
  JRB node;
  node=jrb_find_int(root,id);
  if(node==NULL)
    return NULL;
  return jval_s(node->val);
}

int get_key(JRB root,char *name)
{
  JRB cur,node;
  jrb_traverse(cur,root)
    if(strcmp(name,jval_s(cur->val))==0)
      return jval_i(cur->key);
  return -1;
}

int get_parent(graph g,int start,int stop,int *parent)
{
  int b[MAX];
  int i,count,n;
  count=0;
  b[count++]=stop;
  if(parent[stop]<0)
    return -1;
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

int get_weight_i(graph g,int ver1,int ver2)
{
  JRB node,adj;
  node=jrb_find_int(g.edges,ver1);
  if(node==NULL)
    return 0;
  adj=jrb_find_int((JRB)jval_v(node->val),ver2);
  if(adj==NULL)
    return 0;
  else
    return jval_i(adj->val);
}

float get_weight_f(graph g,int ver1,int ver2)
{
  JRB node,adj;
  node=jrb_find_int(g.edges,ver1);
  if(node==NULL)
    return 0;
  adj=jrb_find_int((JRB)jval_v(node->val),ver2);
  if(adj==NULL)
    return 0;
  else
    return jval_f(adj->val);
}

int get_all_ways(graph g,int start,int stop,way *s)
{
  int dem,i;
  int visited[MAX];
  int parent[MAX];
  Dllist stack;
  dem=0;
  for(i=0;i<MAX;i++)
    {
      visited[i]=0;
      parent[i]=-1;
    }
  stack=new_dllist();
  dll_append(stack,new_jval_i(start));
  DFS_(g,start,stop,&dem,stack,visited,parent,s);
  return dem;
}

int get_weight_way_i(graph g,way s)
{
  int i,w;
  w=0;
  for(i=0;i<s.count-1;i++)
    w=w+get_weight_i(g,s.ver[i],s.ver[i+1]);
  return w;
}

float get_weight_way_f(graph g,way s)
{
  int i;
  float w;
  w=0;
  for(i=0;i<s.count-1;i++)
    w=w+get_weight_f(g,s.ver[i],s.ver[i+1]);
  return w;
}

// tra ve so luong cac dinh co lien thong truoc voi dinh ver.
int ver_before(graph g,int ver,int *before)
{
  Dllist queue,node;
  int u,n,dem;
  int i;
  int visited[MAX];
  int output[MAX];
  for(i=0;i<MAX;i++)
    visited[i]=output[i]=0;
  queue=new_dllist();
  dll_append(queue,new_jval_i(ver));
  dem=0;
  while(!dll_empty(queue))
    {
      node=dll_first(queue);
      u=jval_i(node->val);
      dll_delete_node(node);
      n=indegree(g,u,output);
      if(n==0&&dll_empty(queue))
	return dem;
      for(i=0;i<n;i++)
	{
	  if(!visited[output[i]])
	    {
	      visited[output[i]]=1;
	      before[dem]=output[i];
	      dll_append(queue,new_jval_i(output[i]));
	      dem++;	      
	    }
	}
    }
  return dem;
}

int ver_after(graph g,int ver,int *after)
{
  Dllist queue,node;
  int u,n,dem;
  int i;
  int visited[MAX];
  int output[MAX];
  for(i=0;i<MAX;i++)
    visited[i]=output[i]=0;
  queue=new_dllist();
  dll_append(queue,new_jval_i(ver));
  dem=0;
  while(!dll_empty(queue))
    {
      node=dll_first(queue);
      u=jval_i(node->val);
      dll_delete_node(node);
      n=outdegree(g,u,output);
      if(n==0&&dll_empty(queue))
	return dem;
      for(i=0;i<n;i++)
	{
	  if(!visited[output[i]])
	    {
	      visited[output[i]]=1;
	      after[dem]=output[i];
	      dll_append(queue,new_jval_i(output[i]));
	      dem++;	      
	    }
	}
    }
  return dem;
}

// Duyet tat ca do thi, tra ve so dinh duyet duoc, va mang ver_after cho mot thu tu truoc -> sau.
int sort(graph g,int *ver_after)
{
  Dllist queue, node;
  int visited[MAX];
  int output[MAX];
  int inde[MAX];
  int i;
  int u,n,dem;
  JRB cur;
  dem=0;
  queue=new_dllist();
  for(i=0;i<MAX;i++)
    {
      visited[i]=output[i]=0;
      inde[i]=-1;
    }
  jrb_traverse(cur,g.edges)
    {
      inde[jval_i(cur->key)]=indegree(g,jval_i(cur->key),output);
      if(inde[jval_i(cur->key)]==0)
	dll_append(queue,cur->key);
    }
  if(check_indegree(g,inde)==0)
    return -1;
  while(!dll_empty(queue))
    {
      node=dll_first(queue);
      u=jval_i(node->val);
      dll_delete_node(node);
      ver_after[dem++]=u;
      n=outdegree(g,u,output);
      for(i=0;i<n;i++)
	{
	  inde[output[i]]--;
	  if(inde[output[i]]==0)
	    {
	      dll_append(queue,new_jval_i(output[i]));
	    }
	}
    }
  return dem;
}

int weight_max_i(graph g,int ver,int *parent)
{
  int output[MAX]={0};
  int weight[MAX]={0};
  int weight1[MAX];
  int inde[MAX];
  int u,v;
  int i,n;
  Dllist node,queue;
  JRB cur;
  for(i=0;i<MAX;i++)
    {
      output[i]=weight[i]=weight1[i]=0;
      inde[i]=-1;
    }
  jrb_traverse(cur,g.edges)
    inde[jval_i(cur->key)]=indegree(g,jval_i(cur->key),output);
  if(check_indegree(g,inde)==0)
    return -1;
  jrb_traverse(cur,g.edges)
    {
      if(indegree(g,jval_i(cur->key),output)==0)
	{
	  queue=new_dllist();
	  dll_append(queue,cur->key);
	  while(!dll_empty(queue))
	    {
	      node=dll_first(queue);
	      u=jval_i(node->val);
	      dll_delete_node(node);
	      n=outdegree(g,u,output);
	      for(i=0;i<n;i++)
		{
		  v=output[i];
		  weight[i]=get_weight_i(g,u,v);
		  if(weight1[v]<=weight1[u]+weight[i])
		    {
		      parent[v]=u;
		      weight1[v]=weight1[u]+weight[i];
		    }
		  inde[v]--;
		  if(inde[v]>=0)
		    dll_append(queue,new_jval_i(v));
		}
	    }
	}
    }
  return weight1[ver];
}

float weight_max_f(graph g,int ver,int *parent)
{
  int output[MAX]={0};
  float weight[MAX]={0};
  float weight1[MAX];
  int inde[MAX];
  int u,v;
  int i,n;
  Dllist node,queue;
  JRB cur;
  for(i=0;i<MAX;i++)
    {
      weight[i]=weight1[i]=0;
      inde[i]=-1;
    }
  jrb_traverse(cur,g.edges)
    inde[jval_i(cur->key)]=indegree(g,jval_i(cur->key),output);
  if(check_indegree(g,inde)==0)
    return -1;
  jrb_traverse(cur,g.edges)
    {
      if(indegree(g,jval_i(cur->key),output)==0)
	{
	  queue=new_dllist();
	  dll_append(queue,cur->key);
	  while(!dll_empty(queue))
	    {
	      node=dll_first(queue);
	      u=jval_i(node->val);
	      dll_delete_node(node);
	      n=outdegree(g,u,output);
	      for(i=0;i<n;i++)
		{
		  v=output[i];
		  weight[i]=get_weight_f(g,u,v);
		  if(weight1[v]<=weight1[u]+weight[i])
		    {
		      parent[v]=u;
		      weight1[v]=weight1[u]+weight[i];
		      // inde[v]--;
		    }
		  inde[v]--;
		  if(inde[v]>=0)
		    dll_append(queue,new_jval_i(v));
		}
	    }
	}
    }
  return weight1[ver];
}

int weight_min_i(graph g,int ver,int *parent)
{
  int output[MAX]={0};
  int weight[MAX]={0};
  int weight1[MAX];
  int inde[MAX];
  int u,v;
  int i,n;
  Dllist node,queue;
  JRB cur;
  for(i=0;i<MAX;i++)
    {
      output[i]=weight[i]=weight1[i]=0;
      inde[i]=-1;
    }
  jrb_traverse(cur,g.edges)
    inde[jval_i(cur->key)]=indegree(g,jval_i(cur->key),output);
  if(check_indegree(g,inde)==0)
    return -1;
  jrb_traverse(cur,g.edges)
    {
      if(indegree(g,jval_i(cur->key),output)==0)
	{
	  queue=new_dllist();
	  dll_append(queue,cur->key);
	  while(!dll_empty(queue))
	    {
	      node=dll_first(queue);
	      u=jval_i(node->val);
	      dll_delete_node(node);
	      n=outdegree(g,u,output);
	      for(i=0;i<n;i++)
		{
		  v=output[i];
		  weight[i]=get_weight_i(g,u,v);
		  if(weight1[v]>weight1[u]+weight[i]||weight1[v]==0)
		    {
		      parent[v]=u;
		      weight1[v]=weight1[u]+weight[i];
		    }
		  inde[v]--;
		  if(inde[v]>=0)
		    dll_append(queue,new_jval_i(v));
		}
	    }
	}
    }
  return weight1[ver];
}

float weight_min_f(graph g,int ver,int *parent)
{
  int output[MAX]={0};
  float weight[MAX]={0};
  float weight1[MAX];
  int inde[MAX];
  int u,v;
  int i,n;
  Dllist node,queue;
  JRB cur;
  for(i=0;i<MAX;i++)
    {
      output[i]=0;
      weight[i]=weight1[i]=0;
      inde[i]=-1;
    }
  jrb_traverse(cur,g.edges)
    inde[jval_i(cur->key)]=indegree(g,jval_i(cur->key),output);
  if(check_indegree(g,inde)==0)
    return -1;
  jrb_traverse(cur,g.edges)
    {
      if(indegree(g,jval_i(cur->key),output)==0)
	{
	  queue=new_dllist();
	  dll_append(queue,cur->key);
	  while(!dll_empty(queue))
	    {
	      node=dll_first(queue);
	      u=jval_i(node->val);
	      dll_delete_node(node);
	      n=outdegree(g,u,output);
	      for(i=0;i<n;i++)
		{
		  v=output[i];
		  weight[i]=get_weight_f(g,u,v);
		  if(weight1[v]>weight1[u]+weight[i]||weight1[v]==0)
		    {
		      parent[v]=u;
		      weight1[v]=weight1[u]+weight[i];
		      // inde[v]--;
		    }
		  inde[v]--;
		  if(inde[v]>=0)
		    dll_append(queue,new_jval_i(v));
		}
	    }
	}
    }
  return weight1[ver];
}

int way_weight_max_i(graph g,int start,int stop,int *max_weight,int *vers)
{
  int dem;
  int visited[MAX];
  int parent[MAX];
  int weight[MAX];
  int i,j,max;
  Dllist stack;
  way *s;
  s=(way*)malloc(MAX*sizeof(way));
  dem=0;
  for(i=0;i<MAX;i++)
    {
      visited[i]=weight[i]=0;
      parent[i]=-1;
    }
  stack=new_dllist();
  dll_append(stack,new_jval_i(start));
  DFS_(g,start,stop,&dem,stack,visited,parent,s);
  if(dem==0)
    return 0;
  for(i=0;i<dem;i++)
    weight[i]=get_weight_way_i(g,s[i]);
  (*max_weight)=weight[0];
  j=0;
  for(i=0;i<dem;i++)
    if((*max_weight)<weight[i])
      {
	j=i;
	(*max_weight)=weight[i];
      }
  for(i=0;i<s[j].count;i++)
    vers[i]=s[j].ver[i];
  //  printf(" %d.\n",(*max_weight));
  return s[j].count;
}

int way_weight_max_f(graph g,int start,int stop,float *max_weight,int *vers)
{
  int dem;
  int visited[MAX];
  int parent[MAX];
  float weight[MAX];
  int i,j,max;
  Dllist stack;
  way *s;
  s=(way*)malloc(MAX*sizeof(way));
  dem=0;
  for(i=0;i<MAX;i++)
    {
      visited[i]=0;
      weight[i]=0;
      parent[i]=-1;
    }
  stack=new_dllist();
  dll_append(stack,new_jval_i(start));
  DFS_(g,start,stop,&dem,stack,visited,parent,s);
  if(dem==0)
    return 0;
  for(i=0;i<dem;i++)
    weight[i]=get_weight_way_f(g,s[i]);
  (*max_weight)=weight[0];
  j=0;
  for(i=0;i<dem;i++)
    if((*max_weight)<weight[i])
      {
	j=i;
	(*max_weight)=weight[i];
      }
  for(i=0;i<s[j].count;i++)
    vers[i]=s[j].ver[i];
  //  printf(" %d.\n",(*max_weight));
  return s[j].count;
}

int way_weight_min_i(graph g,int start,int stop,int *min_weight,int *vers)
{
  int dem;
  int visited[MAX];
  int parent[MAX];
  int weight[MAX];
  int i,j;
  Dllist stack;
  way *s;
  s=(way*)malloc(MAX*sizeof(way));
  dem=0;
  for(i=0;i<MAX;i++)
    {
      visited[i]=weight[i]=0;
      parent[i]=-1;
    }
  stack=new_dllist();
  dll_append(stack,new_jval_i(start));
  DFS_(g,start,stop,&dem,stack,visited,parent,s);
  if(dem==0)
    return 0;
  for(i=0;i<dem;i++)
    weight[i]=get_weight_way_i(g,s[i]);
  (*min_weight)=weight[0];
  j=0;
  for(i=0;i<dem;i++)
    if((*min_weight)>weight[i])
      {
	j=i;
	(*min_weight)=weight[i];
      }
  for(i=0;i<s[j].count;i++)
    vers[i]=s[j].ver[i];
  //  printf(" %d.\n",(*min_weight));
  return s[j].count;
}


int way_weight_min_f(graph g,int start,int stop,int *min_weight,int *vers)
{
  int dem;
  int visited[MAX];
  int parent[MAX];
  float weight[MAX];
  int i,j;
  Dllist stack;
  way *s;
  s=(way*)malloc(MAX*sizeof(way));
  dem=0;
  for(i=0;i<MAX;i++)
    {
      visited[i]=0;
      parent[i]=-1;
      weight[i]=0;
    }
  stack=new_dllist();
  dll_append(stack,new_jval_i(start));
  DFS_(g,start,stop,&dem,stack,visited,parent,s);
  if(dem==0)
    return 0;
  for(i=0;i<dem;i++)
    weight[i]=get_weight_way_f(g,s[i]);
  (*min_weight)=weight[0];
  j=0;
  for(i=0;i<dem;i++)
    if((*min_weight)>weight[i])
      {
	j=i;
	(*min_weight)=weight[i];
      }
  for(i=0;i<s[j].count;i++)
    vers[i]=s[j].ver[i];
  //  printf(" %d.\n",(*min_weight));
  return s[j].count;
}

//Ham luu so thanh phan lien thong cua do thi trong bien dem, mang visited cho cac dinh lien thong.
void duyet_bfs(graph g,int s,int *visited,int *parent,int *dem)
{
  Dllist queue,node;
  int u,v,n,i;
  int output[MAX];
  queue=new_dllist();
  dll_append(queue,new_jval_i(s));
  (*dem)++;
  visited[s]=*dem;
  while(!dll_empty(queue))
    {
      node=dll_first(queue);
      u=jval_i(node->val);
      dll_delete_node(node);
      n=lienke(g,u,output);
      for(i=0;i<n;i++)
	{
	  v=output[i];
	  if(visited[v]==0)
	    {
	      visited[v]=*dem;
	      parent[v]=u;
	      dll_append(queue,new_jval_i(v));
	    }
	}
    }
}

// Ham luu so thanh phan lien thong cua do thi trong bien dem, mang visited cho cac dinh lien thong.
void duyet_dfs(graph g,int s,int *visited,int *parent,int *dem)
{
  Dllist stack,node;
  int output[MAX];
  int u,v,n,i;
  stack=new_dllist();
  (*dem)++;
  visited[s]=*dem;
  dll_append(stack,new_jval_i(s));
  while(!dll_empty(stack))
    {
      node=dll_last(stack);
      u=jval_i(node->val);
      dll_delete_node(node);
      n=lienke(g,u,output);
      for(i=0;i<n;i++)
	{
	  v=output[i];
	  if(!visited[v])
	    {
	      visited[v]=*dem;
	      parent[v]=u;
	      dll_append(stack,new_jval_i(v));
	    }
	}
    }
}

void BFS_min(graph g,int start,int stop,int *parent)
{
  int visited[MAX]={0};
  int weight[MAX]={0};
  int n,i,u,v;
  Dllist node,queue;
  JRB cur,adj;
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
	   adj=find_adj(g,u);
	   jrb_traverse(cur,adj) 
	     {
	       v=jval_i(cur->key);
	      
	       if(weight[v]>weight[u]+jval_i(cur->val)||weight[v]==0)
		 {
		   parent[v]=u;
		   weight[v]=weight[u]+jval_i(cur->val);
		 }
	       if(!visited[v])
		 dll_append(queue,new_jval_i(v));
	     }
	 }
     }
}

void BFS_min_f(graph g,int start,int stop,int *parent)
{
  int visited[MAX]={0};
  float weight[MAX]={0};
  int n,i,u,v;
  Dllist node,queue;
  JRB cur,adj;
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
	   adj=find_adj(g,u);
	   jrb_traverse(cur,adj) 
	     {
	       v=jval_i(cur->key);
	      
	       if(weight[v]>weight[u]+jval_f(cur->val)||weight[v]==0)
		 {
		   parent[v]=u;
		   weight[v]=weight[u]+jval_f(cur->val);
		 }
	       if(!visited[v])
		 dll_append(queue,new_jval_i(v));
	     }
	 }
     }
}

// Kiem tra co ton tai duong di giua hai dinh.
int BFS(graph g,int start,int stop,int *parent)
{
  int tham[MAX]={0};
  int duyet[MAX]={0};
  int output[MAX]={0};
  int u,v;
  int n,i;
  Dllist queue,node;
  queue=new_dllist();
  dll_append(queue,new_jval_i(start));
  while(!dll_empty(queue))
    {
      node=dll_first(queue);
      u=jval_i(node->val);
      dll_delete_node(node);
      if(!duyet[u])
	{
	  duyet[u]=1;
	  n=outdegree(g,u,output);
	  for(i=0;i<n;i++)
	    if(!tham[output[i]])
	      {
		tham[output[i]]=1;
		parent[output[i]]=u;
		dll_append(queue,new_jval_i(output[i]));
		if(output[i]==stop)
		  return 1;	      
	      }
	}
    }
  return 0;
}

int DFS(graph g,int start,int stop,int *parent)
{
  int tham[MAX]={0};
  int duyet[MAX]={0};
  int output[MAX]={0};
  int u,v;
  int n,i;
  Dllist stack,node;
  stack=new_dllist();
  dll_append(stack,new_jval_i(start));
  while(!dll_empty(stack))
    {
      node=dll_last(stack);
      u=jval_i(node->val);
      dll_delete_node(node);
      if(!duyet[u])
	{
	  duyet[u]=1;
	  n=outdegree(g,u,output);
	  for(i=0;i<n;i++)
	    if(!tham[output[i]])
	      {
		tham[output[i]]=1;
		parent[output[i]]=u;
		dll_append(stack,new_jval_i(output[i]));
		if(output[i]==stop)
		  return 1;	      
	      }
	}
    }
  return 0;
}

void DFS_(graph g,int start,int stop,int *dem,Dllist stack,int *visited,int *parent,way *s)
{
  int visited2[MAX],output[MAX];
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
	    pa[i]=0;
	  memcpy(pa,parent,MAX*sizeof(int));
	  n=get_parent(g,start,stop,pa);
	  s[*dem].count=n;
	  for(i=0;i<s[*dem].count;i++)
	    s[*dem].ver[i]=pa[i];
	  (*dem)++;            	    
	}
      visited[u]=1;
      n=outdegree(g,u,output);
      for(i=0;i<n;i++)
	{
	  v=output[i];
	  if(!visited[v])
	    {
	      dll_append(stack,new_jval_i(v));
	      parent[v]=u;
	      memcpy(visited2,visited,sizeof(int)*MAX);
	      DFS_(g,start,stop,dem,stack,visited2,parent,s);
	    }
	}
    }
}

// Tra ve 1 neu ton tai. nguoc lai tra ve 0.
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

int get_all_cycles(graph g,way *s)
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

  void DFS_cycle(graph g,int start,int stop,int *dem,Dllist stack,int *visited,int *parent,way *s)
  {
    int visited2[MAX],output[MAX];
    int n,u,v,i,j;
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
	    int pa[MAX],n1;
	    way a;
	    for(i=0;i<MAX;i++)
	      pa[i]=0;
	    memcpy(pa,parent,MAX*sizeof(int));
	    n1=get_parent(g,start,stop,pa);
	    a.count=n1+1;
	    for(i=0;i<n1;i++)
	      a.ver[i]=pa[i];
	    a.ver[i]=start;
	    // Co the them trong so cua chu trinh o day weight_way(g,a).
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
	n=outdegree(g,u,output);
	for(i=0;i<n;i++)
	  {
	    v=output[i];
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
  int dem,i,j,k,n,out[MAX];
  int visited[MAX],parent[MAX],tham[MAX]={0};
  dem=0;
  jrb_traverse(cur,g.edges)
    {
      n=indegree(g,jval_i(cur->key),out);
      tham[jval_i(cur->key)]=1;
      for(k=0;k<n;k++)
	{
	  if(!tham[out[k]])
	    {
	      Dllist stack;
	      stack=new_dllist();
	      for(i=0;i<100;i++)
		visited[i]=parent[i]=0;
	      dll_append(stack,cur->key);
	      DFS_cycle(g,jval_i(cur->key),out[k],&dem,stack,visited,parent,s);
	      free_dllist(stack);
	    }
	}
    }
  return dem;
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
	  add_ver(g,i,new_jval_s(strdup("a")));
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
	  add_ver(g,i,new_jval_s(strdup("a")));
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

void DFS_vo_huong(graph g,int start,int stop,int *dem,Dllist stack,int *visited,int *parent,way *s)
{
  int visited2[MAX],output[MAX];
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
	    pa[i]=0;
	  memcpy(pa,parent,MAX*sizeof(int));
	  n=get_parent(g,start,stop,pa);
	  s[*dem].count=n;
	  for(i=0;i<s[*dem].count;i++)
	    s[*dem].ver[i]=pa[i];
	  (*dem)++;            	    
	}
      visited[u]=1;
      n=lienke(g,u,output);
      for(i=0;i<n;i++)
	{
	  v=output[i];
	  if(!visited[v])
	    {
	      dll_append(stack,new_jval_i(v));
	      parent[v]=u;
	      memcpy(visited2,visited,sizeof(int)*MAX);
	      DFS_vo_huong(g,start,stop,dem,stack,visited2,parent,s);
	    }
	}
    }
}

int get_way_vo_huong(graph g,int start,int stop,way *s)
{
  int dem,i;
  int visited[MAX];
  int parent[MAX];
  Dllist stack;
  dem=0;
  for(i=0;i<MAX;i++)
    {
      visited[i]=0;
      parent[i]=-1;
    }
  stack=new_dllist();
  dll_append(stack,new_jval_i(start));
  DFS_vo_huong(g,start,stop,&dem,stack,visited,parent,s);
  return dem;
}

int dinh_chot(graph g)
{
  JRB cur,cur1;
  way *s;
  int n,i,j,k,l=0;
  way v;
  int output[MAX];
  for(i=0;i<MAX;i++)
    v.ver[i]=-1;
  v.count=0;
  s=(way*)malloc(MAX*sizeof(way));
  jrb_traverse(cur,g.edges)
    jrb_traverse(cur1,g.edges)
    {
      if(jval_i(cur->key)!=jval_i(cur1->key))
	{
	  n=get_way_vo_huong(g,jval_i(cur->key),jval_i(cur1->key),s);
	  if(n==1)
	    {
	      if(s[0].count==2)
		if(lienke(g,s[0].ver[0],output)>1)
		  return s[0].ver[0];
		else
		  if(lienke(g,s[0].ver[1],output)>1)
		    return s[0].ver[1];
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

int canh_chot(graph g,int *ver)
{
  JRB cur,cur1;
  way s[MAX];
  int n,l,i;
  int output[MAX];
  int visited[MAX]={0};
  jrb_traverse(cur,g.edges)
    {
      n=lienke(g,jval_i(cur->key),output);
      visited[jval_i(cur->key)]=1;
      for(i=0;i<n;i++)
	if(!visited[output[i]])
	  if(get_way_vo_huong(g,jval_i(cur->key),output[i],s)==1)
	    {
	      ver[0]=jval_i(cur->key);
	      ver[1]=output[i];
	      return 1;
	    }
    }
  return 0;
}

int lien_thong_max(graph g,way *s)
{
  int visited[MAX]={0};
  int parent[MAX];
  int dem,n,i,j,max;
  JRB cur;
  way w[MAX];
  dem=0;
  j=0;
  n=0;
  jrb_traverse(cur,g.vers)
    if(visited[jval_i(cur->key)]==0)
      duyet_bfs(g,jval_i(cur->key),visited,parent,&dem);
  for(i=0;i<dem;i++)
    {
      w[i].count=0;
      jrb_traverse(cur,g.vers)
	if(visited[jval_i(cur->key)]==i+1)
	  w[i].ver[w[i].count++]=jval_i(cur->key);
    }
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

int connected_before_nsteps(graph g,int ver,int *vert,int k)
{
  Dllist queue,node;
  int u,n,dem,count;
  int i;
  int visited[MAX];
  int output[MAX];
  int weight[MAX]={0};
  for(i=0;i<MAX;i++)
    visited[i]=output[i]=0;
  queue=new_dllist();
  dll_append(queue,new_jval_i(ver));
  dem=0;
  count=0;
  while(!dll_empty(queue))
    {
      node=dll_first(queue);
      u=jval_i(node->val);
      dll_delete_node(node);
      n=indegree(g,u,output);
      if(n==0&&dll_empty(queue))
	return dem;
      for(i=0;i<n;i++)
	{
	  if(!visited[output[i]])
	    {
	      visited[output[i]]=1;
	      weight[output[i]]=weight[u]+1;
	      if(weight[output[i]]<=k)
		vert[dem++]=output[i];
	      dll_append(queue,new_jval_i(output[i]));
	    }	      
	}
    }
  return dem;
}

int connected_after_nsteps(graph g,int ver,int *vert,int k)
{
  Dllist queue,node;
  int u,n,dem,count;
  int i;
  int visited[MAX];
  int output[MAX];
  int weight[MAX]={0};
  for(i=0;i<MAX;i++)
    visited[i]=output[i]=0;
  queue=new_dllist();
  dll_append(queue,new_jval_i(ver));
  dem=0;
  count=0;
  while(!dll_empty(queue))
    {
      node=dll_first(queue);
      u=jval_i(node->val);
      dll_delete_node(node);
      n=outdegree(g,u,output);
      if(n==0&&dll_empty(queue))
	return dem;
      for(i=0;i<n;i++)
	{
	  if(!visited[output[i]])
	    {
	      visited[output[i]]=1;
	      weight[output[i]]=weight[u]+1;
	      if(weight[output[i]]<=k)
		vert[dem++]=output[i];
	      dll_append(queue,new_jval_i(output[i]));
	    }	      
	}
    }
  return dem;
}
