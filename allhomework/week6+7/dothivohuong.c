#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"jval.h"
#include"jrb.h"
#include"dllist.h"

int compare_i(Jval key1,Jval key2)
{
  return jval_i(key1)-jval_i(key2);
}

int compare_s(Jval key1,Jval key2)
{
  return strcmp(jval_s(key1),jval_s(key2));
}

int pri_ver(int ver)
{
  printf("%d  ",ver);
  return 0;
}


int add_ver(JRB g,Jval ver,int (*compare)(Jval ,Jval ))// Ham them dinh vao cay.
{
  JRB node,edgeTree;
  node=jrb_find_gen(g,ver,compare);
  if(node==NULL)
    {
      edgeTree=make_jrb();
      jrb_insert_gen(g,ver,new_jval_v(edgeTree),compare);
      return 0;
    }
  return 1;
}

//Ham them cung.
void add_edge(JRB g,Jval ver1,Jval ver2,int (*compare)(Jval ,Jval ))
{
  JRB node1,node2,newTree1,newTree2;
  node1=jrb_find_gen(g,ver1,compare);
  if(node1==NULL)
    {
      newTree1=make_jrb();
      node1=jrb_insert_gen(g,ver1,new_jval_v(newTree1),compare);
    }
  if(jrb_find_gen((JRB)jval_v(node1->val),ver2,compare)==NULL)
    jrb_insert_gen((JRB)jval_v(node1->val),ver2,new_jval_v(NULL),compare);
  node2=jrb_find_gen(g,ver2,compare);
  if(node2==NULL)
    {
      newTree2=make_jrb();
      node2=jrb_insert_gen(g,ver2,new_jval_v(newTree2),compare);
    }
  if(jrb_find_gen((JRB)jval_v(node2->val),ver1,compare)==NULL)
    jrb_insert_gen((JRB)jval_v(node2->val),ver1,new_jval_v(NULL),compare);
  return;
}

//Kiem tra hai dinh lien ke. Neu dung tra ve 1.
int check_adj(JRB g,Jval ver1,Jval ver2,int (*compare)(Jval ,Jval ))
{
  JRB node1;
  node1=jrb_find_gen(g,ver1,compare);
  if(node1==NULL)
    return 0;
  if(jrb_find_gen((JRB)jval_v(node1->val),ver2,compare)==NULL)
    return 0;
  return 1;
}

//Ham tra ve con tro tro toi dinh can tim.

JRB find_ver(JRB g,Jval ver,int (*compare)(Jval ,Jval ))
{
  JRB node;
  node=jrb_find_gen(g,ver,compare);
  if(node==NULL)
    return NULL;
  return (JRB)jval_v(node->val);
}

void menu()
{
  printf("\n==================================================================\n");
  printf("============== CHUONG TRINH QUAN LY HE THONG NHA GA ==============\n");
  printf("==================================================================\n");
  printf("\n 1. Nhap vao ten mot ga, dua ra cac nha ga noi truc tiep voi no.\n");
  printf(" 2. Nhap vao hai nha ga, kiem tra chung co noi truc tiep hay khong.\n");
  printf(" 3. Tim duong di ngan nhat giua hai nha ga.\n");
  printf(" 4. Dua ra so duong di giua hai nha ga.\n");
  printf(" 5. Thoat.\n");
  printf(" 6. Duyet theo chieu rong BFS.\n");
  printf(" 7. Duyet theo chieu sau DFS.\n");
  printf(" 8. Kiem tra do thi co lien thong hay khong.\n");
  printf(" 9. Nhap vao hai dinh, dua ra duong di giua hai dinh.\n");
  printf("\n=====> Nhap lua chon : ");
}

int luachon(void (*menu)(void ),int soluachon)
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

//Ham tra ve so dinh lien ke voi dinh ver,mang output ghi nhan key cua dinh lien ke.
int lienke(JRB g,Jval ver,int *output,int (*compare)(Jval ,Jval ))
{
  JRB tree,cur;
  int total;
  tree=find_ver(g,ver,compare);
  total=0;
  jrb_traverse(cur,tree)
    {
      output[total]=jval_i(cur->key);
      total++;              
    }
  return total;
}

// Ham tra ve so dinh cua do thi.
int count_ver(JRB g)
{
  int count=0;
  JRB cur;
  jrb_traverse(cur,g)
    count++;
  return count;
}

// Ham tra ve so canh cua do thi.
int count_edge(JRB g)
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


void BFS(JRB g,int start,int stop,int *parent)
{
   int visited[25]={0};
   int output[25];
   int n,i,u,v;
   Dllist node,queue;
   Jval ver;
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
	   if(u==stop)
	     return;
	   ver.i=u;
	   n=lienke(g,ver,output,compare_i);
	   for(i=0;i<n;i++)
	     {
	       v=output[i];
	       if(!visited[v])
		 {
		   parent[v]=u;
		   ver.i=v;
		   dll_append(queue,ver);
		 }
	     }
	 }
     }
}

void DFS_dequy(JRB g,int start,int stop,int *dem,Dllist stack,int *visited)
{
  int visited2[25],output[25];
  int n,i,u,v;
  Jval ver;
  Dllist node;

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
      // printVertex(u);               
      if(u==stop)
	{
	  (*dem)++;
	  //  printf("OK\n");            	    
	}
      visited[u]=1;
      ver.i=u;
      n=lienke(g,ver,output,compare_i);
      for(i=0;i<n;i++)
	{
	  v=output[i];
	  if(!visited[v])
	    {
	      dll_append(stack,new_jval_i(v));
	      memcpy(visited2,visited,sizeof(int)*25);
	      DFS_dequy(g,start,stop,dem,stack,visited2);
	    }
	}
    }
}

//Ham kiem tra lien thong.
int lien_thong(JRB g,int start,int stop,int (*compare)(Jval ,Jval ))
{
  Dllist stack;
  int dem=0,visited[25]={0};
  stack=new_dllist();
  if(jrb_find_gen(g,new_jval_i(start),compare)==NULL)
    return 0;
  if(jrb_find_gen(g,new_jval_i(stop),compare)==NULL)
    return 0;
  dll_append(stack,new_jval_i(start));
  DFS_dequy(g,start,stop,&dem,stack,visited);
  return dem;
}

void bfs(JRB g,int start,int stop,int (*function) (int)) 
{
  Dllist queue, node;
  int visited[100];
  int u, v, i;
  JRB adj, cur;
  for(i=0;i<100;i++)
    visited[i]=0;
  queue=new_dllist();
  visited[start]=1;
  dll_append(queue,new_jval_i(start));
  while(!dll_empty(queue))
    {
      node=dll_first(queue);
      u=jval_i(node->val);
      dll_delete_node(node);
      function(u);
      if(u==stop)
	return;
      adj=find_ver(g,new_jval_i(u),compare_i);
      jrb_traverse(cur,adj)
	{
	  v=jval_i(cur->key);
	  if(visited[v]==0)
	    {
	      visited[v]=1;
	      dll_append(queue,new_jval_i(v));
	    }
	}
    }
}

void dfs(JRB g,int start,int stop,int (*function)(int ))
{
  int visited[100];
  Dllist stack,node;
  int u,v,i;
  JRB adj,cur;
  for(i=0;i<100;i++)
    visited[i]=0;
  visited[start]=1;
  stack=new_dllist();
  dll_append(stack,new_jval_i(start));
  while(!dll_empty(stack))
    {
      node=dll_last(stack);
      u=jval_i(node->val);
      dll_delete_node(node);
      function(u);
      if(u==stop)
	return;
      adj=find_ver(g,new_jval_i(u),compare_i);
      jrb_traverse(cur,adj)
	{
	  v=jval_i(cur->key);
	  if(visited[v]==0)
	    {
	      visited[v]=1;
	      dll_append(stack,new_jval_i(v));
	    }
	}
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


//Ham duyet theo chieu rong bat dau tu dinh s.
int visited[15]={0};
int dem=0;
int parent[15]={0};
void duyet_bfs(JRB g,int s)
{
  Dllist queue,node;
  int u,v,i;
  JRB adj,cur;
  queue=new_dllist();
  dll_append(queue,new_jval_i(s));
  dem++;
  visited[s]=dem;
  while(!dll_empty(queue))
    {
      node=dll_first(queue);
      u=jval_i(node->val);
      dll_delete_node(node);
      adj=find_ver(g,new_jval_i(u),compare_i);
      jrb_traverse(cur,adj)
	{
	  v=jval_i(cur->key);
	  if(visited[v]==0)
	    {
	      visited[v]=dem;
	      parent[v]=u;
	      dll_append(queue,new_jval_i(v));
	    }
	}
    }
}

void duyet_dfs(JRB g,int s)
{
  Dllist stack,node;
  JRB cur,adj;
  int u,v;
  int i;
  stack=new_dllist();
  dem++;
  visited[s]=dem;
  dll_append(stack,new_jval_i(s));
  while(!dll_empty(stack))
    {
      node=dll_last(stack);
      u=jval_i(node->val);
      dll_delete_node(node);
      adj=find_ver(g,new_jval_i(u),compare_i);
      jrb_traverse(cur,adj)
	{
	  v=jval_i(cur->key);
	  if(!visited[v])
	    {
	      visited[v]=dem;
	      parent[v]=u;
	      dll_append(stack,new_jval_i(v));
	    }
	}
    }
}

void in_dothi(JRB g)
{
  int i,j,n;
  JRB cur,adj,cur1;
  int a[15][15];
  n=count_ver(g);
  printf("%d\n",n);
  for(i=0;i<15;i++)
    {
      for(j=0;j<15;j++)
	a[i][j]=0;
      a[i][i]=1;
    }
  jrb_traverse(cur,g)
    {
      adj=(JRB)jval_v(cur->val);
      jrb_traverse(cur1,adj)
	a[jval_i(cur->key)][jval_i(cur1->key)]=1;
    }
  printf("    ");
  for(i=1;i<=n;i++)
    printf("%3d",i);
  printf("\n");
  for(i=1;i<=n;i++)
    {
      printf("%3d:",i);
      for(j=1;j<=n;j++)
	printf("%3d",a[i][j]);
      printf("\n");
    }
}


int main()
{
  FILE *f;
  char name[]="stations.txt";
  char s[50],*s1,s2[50],*s3;
  int i,n,l,k=-1,m=0,tong=0;
  int a[12][12];
  JRB g,root;
  JRB adj,cur,cur1;
  Jval ver,ver1;
  g=make_jrb();
  root=make_jrb();
  f=fopen(name,"r");
  if(f==NULL)
    {
      printf("File %s chua ton tai.\n",name);
      return 0;
    }
  else
    {
      while(fscanf(f,"%[^\n]\n",s)!=EOF)
	{
	  if(s[0]=='[')
	    if(s[1]=='S')
	      {
		k=0;
		continue;
	      }
	    else
	      {
		k=1;
		continue;
	      }
	  if(k==0)
	    {
	      tong++;
	      sscanf(s,"%*c%d%*c%[^\n]",&ver.i,s2);
	      jrb_insert_gen(root,ver,new_jval_s(strdup(s2)),compare_i);
	      add_ver(g,ver,compare_i);
	    }
	  else
	    if(k==1)
	      {
		i=0;
		l=0;
		while(s[i]!='\0')
		  {
		    if(s[i]=='S')
		      l++;
		    i++;
		  }
		i=0;
		s1=strtok(s,"=");
		s3=strtok(NULL,"S");
		sscanf(s3,"%d",&a[m][i]);
		ver.i=a[m][i];
		for(i=1;i<l;i++)
		  {
		    s3=strtok(NULL,"S");
		    sscanf(s3,"%d",&a[m][i]);
		    ver1.i=a[m][i];
		    add_edge(g,ver,ver1,compare_i);
		    ver=ver1;
		  }
		a[m][l]=0;
		m++;
	      }
	}
      fclose(f);
    }
  n=0;
  while(n!=5)
    {
      n=luachon(menu,9);
      switch(n)
	{
	case 1:
	  {
	    JRB cur2;
	    int j,*lk;
	    lk=(int*)malloc(tong*sizeof(int));
	    printf("\n\tDua ra cac nha ga noi truc tiep voi mot nha ga nao do.\n");
	    printf("\n Nhap vao ten mot nha ga : ");
	    scanf("%[^\n]%*c",s);
	    k=0;
	    jrb_traverse(cur,root)
	      if(compare_s(cur->val,new_jval_s(strdup(s)))==0)
		{
		  k=1;
		  break;
		}
	    if(k==0)
	      printf("\n=====> Khong co nha ga %s trong he thong.\n",s);
	    else
	      {
		printf("\n=====> Cac ga noi truc tiep voi ga %s la :\n",s);
		j=lienke(g,cur->key,lk,compare_i);
		for(i=0;i<j;i++)
		  {
		    ver.i=lk[i];
		    cur2=jrb_find_gen(root,ver,compare_i);
		    printf("\t%s\n",jval_s(cur2->val));
		  }
		/*	adj=find_ver(g,cur->key,compare_i);
		printf("\n=====> Cac ga noi truc tiep voi ga %s la :\n",s);
		jrb_traverse(cur2,adj)
		  {
		    cur1=jrb_find_gen(root,cur2->key,compare_i);
		    printf("\t%s\n",jval_s(cur1->val));
		    }*/
	      }
	    printf("\n\tNha ga %s thuoc cac tuyen :\n",s);
	    for(i=0;i<m;i++)
	      for(j=0;a[i][j]!=0;j++)
		{
		  if(a[i][j]==jval_i(cur->key))
		    printf("\tM%d ",i+1);
		}
	    printf("\n");
	    free(lk);
	    break;
	  }
	case 2:
	  {
	    int k1=0,k2=0;
	    printf("\n\tKiem tra hai nha ga co noi truc tiep hay khong.\n");
	    printf("\n Nhap vao ten nha ga thu nhat : ");
	    scanf("%[^\n]%*c",s);
	    printf("\n Nhap vao ten nha ga thu hai : ");
	    scanf("%[^\n]%*c",s2);
	    jrb_traverse(cur,root)
	      if(compare_s(cur->val,new_jval_s(strdup(s)))==0)
		{
		  k1=1;
		  break;
		}
	    jrb_traverse(cur1,root)
	      if(compare_s(cur1->val,new_jval_s(strdup(s2)))==0)
		  {
		    k2=1;
		    break;
		  }
	    if(k1==0)
	      {
		printf("\n!!! Khong ton tai nha ga %s.\n",s);
		break;
	      }
	    if(k2==0)
	      {
		printf("\n!!! Khong ton tai nha ga %s.\n",s2);
		break;
	      }
	    if(check_adj(g,cur->key,cur1->key,compare_i)==0)
	      printf("\n=====> Hai nha ga khong noi truc tiep voi nhau.\n");
	    else
	      printf("\n=====> Hai nha ga noi truc tiep.\n");
	    break;
	  }
	case 3:
	  {
	    int *parent,b[12],soluong;
	    char start[30],stop[30];
	    soluong=count_ver(g);
	    printf("%d  %d  %d\n",soluong,tong,count_edge(g));
	    parent=(int*)malloc((tong+1)*sizeof(int));
	    for(i=0;i<tong+1;i++)
	      parent[i]=0;
	    printf("\n\tNhap vao ten nha ga bat dau : ");
	    scanf("%[^\n]%*c",start);
	    printf("\n\tNhap vao ten nha ga ket thuc : ");
	    scanf("%[^\n]%*c",stop);
	    cur=find_node(root,new_jval_s(strdup(start)),compare_s);
	    cur1=find_node(root,new_jval_s(strdup(stop)),compare_s);
	    if(cur==NULL)
	      {
		printf("\n\tNha ga %s khong ton tai.\n",start);
		break;
	      }
	    if(cur1==NULL)
	      {
		printf("\n\tNha ga %s khong ton tai.\n",stop);
		break;
	      }
	    BFS(g,jval_i(cur->key),jval_i(cur1->key),parent);
	    i=0;
	    int n1,n2,j;
	    n1=jval_i(cur->key);
	    n2=jval_i(cur1->key);
	    b[0]=n2;
	    while(parent[n2]!=n1)
	      {
		n2=parent[n2];
		b[++i]=n2;
	      }
	    b[++i]=n1;
	    j=i;
	    while(j>0)
	      {
		ver.i=b[j];
		cur=jrb_find_gen(root,ver,compare_i);
		printf("%d. %s -> ",b[j],jval_s(cur->val));
		for(n1=j-2;n1>0;n1--)
		  {
		    Jval ver3;
		    ver3.i=b[n1];
		    if(check_adj(g,ver,ver3,compare_i)!=0)
		      j=n1+1;
		  }
		j--;
	      }
	    ver.i=b[0];
	    cur=jrb_find_gen(root,ver,compare_i);
	    printf("%d. %s\n",b[0],jval_s(cur->val));
	    break;
	  }
	case 4:
	  {
	    Dllist stack;
	    char sta[30],sto[30];
	    int i, start, stop, dem=0, visited[25]={0};
	    printf("\n\tNhap nha ga thu nhat : ");
	    scanf("%[^\n]%*c",sta);
	    printf("\n\tNhap nha ga thu hai : ");
	    scanf("%[^\n]%*c",sto);
	    stack=new_dllist();
	    cur=find_node(root,new_jval_s(strdup(sta)),compare_s);
	    cur1=find_node(root,new_jval_s(strdup(sto)),compare_s);
	    if(cur==NULL)
	      {
		printf("\n\tNha ga %s khong ton tai.\n",sta);
		break;
	      }
	    if(cur1==NULL)
	      {
		printf("\n\tNha ga %s khong ton tai.\n",sto);
		break;
	      }
	    dll_append(stack,cur->key);
	    start=jval_i(cur->key);
	    stop=jval_i(cur1->key);
	    DFS_dequy(g, start, stop, &dem, stack, visited);
	    printf("\n=====> So cach di tu %s den %s la : %d\n",sta,sto,dem);
	    break;
	  }
	case 5:
	  {
	    int i,j;
	    in_dothi(g);
	  // printf("%d\n",lien_thong(g,1,12,compare_i));
	    printf("\n\tBYE BYE! SEE YOU AGAIN!\n\n");
	    break;
	  }
	case 6:
	  bfs(g,1,-1,pri_ver);
	  printf("\n");
	  break;
	case 7:
	  dfs(g,1,-1,pri_ver);
	  printf("\n");
	  break;
	case 8:
	  {
	    JRB cur;
	    int i,j;
	    for(i=0;i<15;i++)
	      visited[i]=parent[i]=0;
	    dem=0;
	    jrb_traverse(cur,g)
	      if(visited[jval_i(cur->key)]==0)
		//duyet_bfs(g,jval_i(cur->key));
		duyet_dfs(g,jval_i(cur->key));
	    if(dem==1)
	      printf("\n=====> Do thi lien thong.\n");
	    else
	      {
		printf("\n  Do thi co %d thanh phan lien thong.\n",dem);
		  for(i=1;i<=dem;i++)
		    {
		      printf("\n\tThanh phan lien thong thu %d : ",i);
		      jrb_traverse(cur,g)
			if(visited[jval_i(cur->key)]==i)
			  printf("%d  ",jval_i(cur->key));
		      printf("\n");
		    }
	      }
	    break;
	  }
	case 9:
	  {
	    int i,j,start,stop;
	    printf("\n  Nhap vao hai dinh : ");
	    scanf("%d%d",&start,&stop);
	    for(i=0;i<15;i++)
	      visited[i]=parent[i]=0;
	    dem=0;
	    duyet_bfs(g,start);
	    //duyet_dfs(g,start);
	    if(visited[start]!=visited[stop])
	      printf("\n=====> Khong ton tai duong di giua hai dinh.\n");
	    else
	      {
		int b[15]={0};
		b[0]=stop;
		i=0;
		while(parent[stop]!=start)
		  {
		    stop=parent[stop];
		    b[++i]=stop;
		  }
		b[++i]=start;
		for(j=i;j>0;j--)
		  printf("%d ->",b[j]);
		printf("%d.\n",b[0]);
	      }
	    break;
	  } 
	}
    }
  FILE *fl;
  fl=fopen("dothivohuong.txt","w");
  JRB c,c1;
  jrb_traverse(c,g)
    {
      jrb_traverse(c1,g)
	if(check_adj(g,c->key,c1->key,compare_i)!=0||compare_i(c->key,c1->key)==0)
	  fprintf(f,"1 ");
	else
	  fprintf(f,"0 ");
      fprintf(f,"\n");
    }
  fclose(fl);
  jrb_free_tree(root);
  jrb_free_tree(g);
  return 0;
}
