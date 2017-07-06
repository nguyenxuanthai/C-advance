#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include "library/inc/bt.h"
#include <time.h>
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

BTA *dic;

GtkWidget *tu;
GtkWidget *add_tu;
GtkWidget *nghia;
GtkWidget *add_nghia;
GtkListStore *store;
GtkTreeIter iter;
GtkWidget * completion;
GtkWidget *window;


// xoa dau cach
void xoakytu(char str[100],int vitri)
{
	int n = strlen(str);
	for(int i = vitri; i < n; i++)
	{
		str[i]=str[i + 1];
	}
}
// xoa dau cach cua tu 
void xoakhoangtrang(char str[100])
{
	int k = strlen(str)-1;
	while (str[k]==32)
	{
		xoakytu(str,k);
		k = strlen(str)-1;
	}
}


// cua so thong bao ket qua tim kiem
void search_message(GtkWidget *widget,gpointer data){
	GtkBuilder *builder;
	GtkWidget *window_mess;
	GtkWidget *label;
	GtkWidget *label_nghia;	
	int rsize;
	char mean[200],word[200],text[200];

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "library/data/project.glade", NULL);

	window_mess = GTK_WIDGET(gtk_builder_get_object(builder, "window_mess"));
	gtk_window_set_title (GTK_WINDOW (window_mess), "KQ Search");
	label = GTK_WIDGET(gtk_builder_get_object(builder, "label"));
	label_nghia = GTK_WIDGET(gtk_builder_get_object(builder, "label_nghia"));

	gtk_label_set_text(GTK_LABEL(label), gtk_entry_get_text(GTK_ENTRY(widget)));

	strcpy(text,(char *)gtk_entry_get_text(GTK_ENTRY(widget)));
	xoakhoangtrang(text);

	if(btsel(dic,text,mean,200*sizeof(char),&rsize)==0){
		// printf("%s\n",s->list[i] );
		gtk_label_set_text(GTK_LABEL(label),text);
		gtk_label_set_text(GTK_LABEL(label_nghia),mean);
	}
	else{
		gtk_label_set_text(GTK_LABEL(label),text);
		gtk_label_set_text(GTK_LABEL(label_nghia),"Kết quả tìm kiếm:________________________________\n\n        -Không tồn tại từ này");
	}

	gtk_widget_show_all(window_mess);

}

// ham tu dong hoan thanh tu khi nhap
static GtkEntryCompletion* create_completion_widget(void)
{
	char s1[100],s2[1500];
	GtkEntryCompletion *completion;
	GtkListStore *store;
	GtkTreeIter iter;
	completion = gtk_entry_completion_new();
	store = gtk_list_store_new(1, G_TYPE_STRING);

	gtk_entry_completion_set_model(completion, GTK_TREE_MODEL(store));
	g_object_unref(store);
	gtk_entry_completion_set_text_column(completion, 0);

	int size;
	gtk_list_store_clear(GTK_LIST_STORE(store));
	btpos(dic,1);
	while(btseln(dic,s1,s2,256,&size)==0){
		gtk_list_store_append(store,&iter);
		gtk_list_store_set(store,&iter,0,s1,-1);
	}
	return completion;
}
// giao dien cua so tim kiem
void window_search(GtkWidget *widget) {
	GtkBuilder *builder;
	GtkWidget *window_search;
	GtkWidget *search_btn_search;
	GtkWidget *input_search;
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "library/data/project.glade", NULL);
	window_search = GTK_WIDGET(gtk_builder_get_object(builder, "window_search"));
	gtk_window_set_title (GTK_WINDOW (window_search), "Search");
	search_btn_search = GTK_WIDGET(gtk_builder_get_object(builder, "search_btn_search"));
	input_search = GTK_WIDGET(gtk_builder_get_object(builder, "input_search"));
  
  	// goi y tu nhap vao input
  	completion = create_completion_widget();
	gtk_entry_set_completion(GTK_ENTRY(input_search),completion);
	g_object_unref(completion);

	g_signal_connect_swapped(G_OBJECT(search_btn_search), "clicked", G_CALLBACK(search_message),input_search);
	
	gtk_widget_show_all(window_search);
}

//  cua so thong bao  them tu vao btree
void window_dialog(GtkWidget *widget,gpointer data){
	GtkBuilder *builder;
	GtkWidget *window;
	GtkWidget *word;
	GtkWidget *mean_nghia;
	GtkWidget *thongbao;
	char tu[200];
	char nghia[200];
	char text[200];
	int rsize;

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "library/data/project.glade", NULL);
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_dialog"));
	gtk_window_set_title (GTK_WINDOW (window), "KQ ADD");
	word = GTK_WIDGET(gtk_builder_get_object(builder, "tu"));
	mean_nghia = GTK_WIDGET(gtk_builder_get_object(builder, "nghia"));
	thongbao = GTK_WIDGET(gtk_builder_get_object(builder, "thongbao"));
	strcpy(tu,(char *)gtk_entry_get_text(GTK_ENTRY(add_tu)));
	xoakhoangtrang(tu);

	strcpy(nghia,(char *)gtk_entry_get_text(GTK_ENTRY(add_nghia)));
	
	if((strcmp(tu,"")== 0) || (strcmp(nghia,"")== 0)){
		gtk_label_set_text(GTK_LABEL(thongbao),"Thông báo:_______________________________\n\n        -Bạn chưa nhập đầy đủ dữ liệu.\n       -Dữ liệu nhập vào không được chứa toàn dấu cách.");
	}
	else{
		if(btsel(dic,tu,text,200*sizeof(char),&rsize)==0){
			gtk_label_set_text(GTK_LABEL(thongbao),"Thông báo:_________________________________\n\n        -Từ này đã tồn tại.");
		}
		else{
			if(btins(dic,tu,nghia,strlen(nghia)*sizeof(char*)))
				gtk_label_set_text(GTK_LABEL(thongbao),"Thông báo:________________________________\n\n        -Thêm từ không thành công - Tràn bộ nhớ");
			else
				gtk_label_set_text(GTK_LABEL(word),tu);
			gtk_label_set_text(GTK_LABEL(mean_nghia),nghia);
			gtk_label_set_text(GTK_LABEL(thongbao),"Thông báo:__________________________________\n\n        -Thêm từ thành công.");
		}
	}
	gtk_widget_show_all(window);
}
// cua so nhap tu them vao
void window_add(GtkWidget *widget) {
	GtkBuilder *builder;
	GtkWidget *window_add;
	GtkWidget *btn_add2;

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "library/data/project.glade", NULL);
	window_add = GTK_WIDGET(gtk_builder_get_object(builder, "window_add"));
	gtk_window_set_title (GTK_WINDOW (window_add), "Add");
	add_tu = GTK_WIDGET(gtk_builder_get_object(builder, "add_tu"));
	add_nghia = GTK_WIDGET(gtk_builder_get_object(builder, "add_nghia"));
	btn_add2 = GTK_WIDGET(gtk_builder_get_object(builder, "btn_add2"));
	g_signal_connect(G_OBJECT(btn_add2), "clicked", G_CALLBACK(window_dialog),NULL);
	gtk_widget_show_all(window_add);

}
// cua so thong bao xoa
void delete_message(GtkWidget *widget,gpointer data){
	GtkBuilder *builder;
	GtkWidget *window_delete_mess;
	GtkWidget *label_tuxoa;
	GtkWidget *label_nghiaxoa;
	GtkWidget *label_thongbao;
	int rsize;
	char word[200],mean[200],text[200];

	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "library/data/project.glade", NULL);
	window_delete_mess = GTK_WIDGET(gtk_builder_get_object(builder, "window_delete_mess"));
	gtk_window_set_title (GTK_WINDOW (window_delete_mess), "KQ Delete");

	label_tuxoa = GTK_WIDGET(gtk_builder_get_object(builder, "label_tuxoa"));
	label_nghiaxoa = GTK_WIDGET(gtk_builder_get_object(builder, "label_nghiaxoa"));
	label_thongbao = GTK_WIDGET(gtk_builder_get_object(builder, "label_thongbao"));
	gtk_label_set_text(GTK_LABEL(label_tuxoa), gtk_entry_get_text(GTK_ENTRY(widget)));
	strcpy(text,(char *)gtk_entry_get_text(GTK_ENTRY(widget)));
	xoakhoangtrang(text);

	if( btdel(dic,text)){
		gtk_label_set_text(GTK_LABEL(label_tuxoa),text);
		gtk_label_set_text(GTK_LABEL(label_thongbao),"_____________________________\n\n     Kết quả xoá:\n\n      Không tồn tại từ này\n");
	}
	else{
		gtk_label_set_text(GTK_LABEL(label_tuxoa),text);
		gtk_label_set_text(GTK_LABEL(label_thongbao),"_____________________________\n\n      Kết quả xoá:\n\n     Đã xoá thành công\n");
	}
	gtk_widget_show_all(window_delete_mess);

}
// cua so xoa tu can xoa
void window_delete(GtkWidget *widget) {
	GtkBuilder *builder;
	GtkWidget *window_delete;
	GtkWidget *search,*delete;
	
	builder = gtk_builder_new();
	gtk_builder_add_from_file(builder, "library/data/project.glade", NULL);
	window_delete = GTK_WIDGET(gtk_builder_get_object(builder, "window_delete"));
	gtk_window_set_title (GTK_WINDOW (window_delete), "Delete");
	search = GTK_WIDGET(gtk_builder_get_object(builder, "search_delete"));
	delete = GTK_WIDGET(gtk_builder_get_object(builder, "btn_delete2"));

	completion = create_completion_widget();
	gtk_entry_set_completion(GTK_ENTRY(search),completion);
	g_object_unref(completion);

	g_signal_connect_swapped(G_OBJECT(delete), "clicked", G_CALLBACK(delete_message),search);
	gtk_widget_show_all(window_delete);
}
// doc file
void docfile(){
	char word1[200];
	char mean1[200];
	FILE *f = fopen("library/data/data2.txt","r");
	btinit();
	dic = btcrt("library/data/mytree",0,0);
	btpos(dic,1);
	while(fscanf(f,"%[^\t]\t%[^\n]\n",word1,mean1)>0){
		btins(dic,word1,mean1,200*sizeof(char));
	}
	fclose(f);	
}
// ghi file xong > exit
void ghifile(GtkWidget *widget,gpointer data){
	FILE *f=fopen("library/data/data2.txt","w");;
	char word[150];
	char mean[1500];
	int i,rsize;
	i=bfndky(dic,"1",&rsize);
	while(bnxtky(dic,word,&i)==0){
		btsel(dic,word,mean,1500,&rsize);
		fprintf(f,"%s\t%s\n",word,mean);
	}
	fclose(f);
	gtk_widget_destroy(window);
}
// chuong trinh chinh
int main(int argc, char** argv){
	// doc file 
	docfile();
	// giao dien chinh(MENU)
	GtkBuilder *builder;
	GtkWidget *btn_search,*btn_add,*btn_delete,*btn_exit;
	
	gtk_init(&argc,&argv);
	builder = gtk_builder_new();
		(builder, "library/data/project.glade", NULL);
	window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));
	gtk_window_set_title (GTK_WINDOW (window), "Menu");
	g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
	btn_search = GTK_WIDGET(gtk_builder_get_object(builder, "btn_search"));
	btn_add = GTK_WIDGET(gtk_builder_get_object(builder, "btn_add"));
	btn_exit = GTK_WIDGET(gtk_builder_get_object(builder, "btn_exit"));
	btn_delete = GTK_WIDGET(gtk_builder_get_object(builder, "btn_delete"));

	// Bat su kien  button
	g_signal_connect_swapped(G_OBJECT(btn_exit), "clicked", G_CALLBACK(ghifile), NULL);
	g_signal_connect_swapped(G_OBJECT(btn_search), "clicked", G_CALLBACK(window_search), NULL);
	g_signal_connect_swapped(G_OBJECT(btn_add), "clicked", G_CALLBACK(window_add), NULL);
	g_signal_connect_swapped(G_OBJECT(btn_delete), "clicked", G_CALLBACK(window_delete), NULL);

	gtk_widget_show_all(window);
	gtk_main();
	return 0;
}

// gcc `pkg-config --cflags gtk+-3.0` -o program program.c `pkg-config --libs gtk+-3.0` libbt.a -w
