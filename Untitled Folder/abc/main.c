#include <stdio.h>
#include "jrb_graph2.h"

#define NUMBER_VERS 15
int main(int argc, char *argv[]){
	// tao 1 do thi co huong la "g(co canh edges va vers)"
	graph g;
	g = create_graph();

	if(argc < 2 || argc > 5){
		printf("Dau vao sai!\n");
		return 0;
	}

	// su ly tham so: ./p -h 
	if(strcmp(argv[1],"-h") == 0){
		printf("Chao Nam Dep Tra S2 \n"); // in cai Lz gi cung dc OK
	}

	// su ly tham so:./p -v demo.txt => doc file sau do add vao do thi => show du lieu
	if(strcmp(argv[1], "-v") == 0){
		IS is; 		// tuong  tu nhu FILE *f
		is = new_inputstruct(argv[2]); // argv[2] = demo.txt => mo file
		if(is == NULL){
			printf("File nay 500GB ko the mo dc: %s ^^\n",argv[2]);
			exit(0); // thung dung exit de thoat khi loi!
		}
		// File demo.txt ok thi lam tiep
		while(get_line(is) >= 0){ // ham nay la lay ra 1 dong cua File
			if(is->NF == 2){   // neu ma dong nay no co 2 phan tu vd: 1 2
				add_ver(g,atoi(is->fields[0]),new_jval_s(strdup(is->fields[1])));  // Thêm đỉnh vào đồ thị ham nay can config
				printf("%s %s\n",is->fields[0],is->fields[1]);
			}
			if(is->NF == 3){  // la khi ma da co 2 dinh thi se xet do dai 2 dinh do (khoang cach) 
				add_edge(g,atoi(is->fields[0]), atoi(is->fields[1]),new_jval_i(atoi(is->fields[2]))); //vd  1 -> 2 cach nhau 100
				add_edge(g,atoi(is->fields[1]), atoi(is->fields[0]),new_jval_i(atoi(is->fields[2]))); // tuong tu 2-> 1 cach nhau 100
			}
		}
		jettison_inputstruct(is); // ~fclose(f);
		drop_graph(g); // huy do thi (drop-xoa) ~ giai phong bo nho!
	}

	// su ly tham so: ./p -w demo.txt 1 3
	if(strcmp(argv[1], "-w") == 0){
		IS is; 		// tuong  tu nhu FILE *f
		is = new_inputstruct(argv[2]); // argv[2] = demo.txt => mo file
		if(is == NULL){
			printf("File nay 500GB ko the mo dc: %s ^^\n",argv[2]);
			exit(0); // thung dung exit de thoat khi loi!
		}
		// File demo.txt ok thi lam tiep
		while(get_line(is) >= 0){ // ham nay la lay ra 1 dong cua File
			if(is->NF == 2){   // neu ma dong nay no co 2 phan tu vd: 1 2
				add_ver(g,atoi(is->fields[0]),new_jval_s(strdup(is->fields[1])));  // Thêm đỉnh vào đồ thị ham nay can config
			}
			if(is->NF == 3){  // la khi ma da co 2 dinh thi se xet do dai 2 dinh do (khoang cach) 
				add_edge(g,atoi(is->fields[0]), atoi(is->fields[1]),new_jval_i(atoi(is->fields[2]))); //vd  1 -> 2 cach nhau 100
				add_edge(g,atoi(is->fields[1]), atoi(is->fields[0]),new_jval_i(atoi(is->fields[2]))); // tuong tu 2-> 1 cach nhau 100
			}
		}
		jettison_inputstruct(is); // ~fclose(f);

		int weight;
		weight = get_weight_i(g, atoi(argv[3]), atoi(argv[4]) ); // tra ve trong so cua canh ver1->ver2.
		printf("%d\n",weight);
		drop_graph(g); // huy do thi (drop-xoa) ~ giai phong bo nho!
	}

	// su ly tham so: ./p -p demo.txt 1 3  tim duong di ngan nhat tu 1 -> 3
	if(strcmp(argv[1], "-p") == 0){
		IS is; 		// tuong  tu nhu FILE *f
		is = new_inputstruct(argv[2]); // argv[2] = demo.txt => mo file
		if(is == NULL){
			printf("File nay 500GB ko the mo dc: %s ^^\n",argv[2]);
			exit(0); // thung dung exit de thoat khi loi!
		}
		// File demo.txt ok thi lam tiep
		while(get_line(is) >= 0){ // ham nay la lay ra 1 dong cua File
			if(is->NF == 2){   // neu ma dong nay no co 2 phan tu vd: 1 2
				add_ver(g,atoi(is->fields[0]),new_jval_s(strdup(is->fields[1])));  // Thêm đỉnh vào đồ thị ham nay can config
			}
			if(is->NF == 3){  // la khi ma da co 2 dinh thi se xet do dai 2 dinh do (khoang cach) 
				add_edge(g,atoi(is->fields[0]), atoi(is->fields[1]),new_jval_i(atoi(is->fields[2]))); //vd  1 -> 2 cach nhau 100
				add_edge(g,atoi(is->fields[1]), atoi(is->fields[0]),new_jval_i(atoi(is->fields[2]))); // tuong tu 2-> 1 cach nhau 100
			}
		}
		jettison_inputstruct(is); // ~fclose(f);
		
		// Tra ve duong di min theo trong so
		int  i,sodinh, weight,ver[NUMBER_VERS];
		JRB cur;
		// Tra ve so dinh trong duong di min. min_weight la trong luong min, ver la mang cac dinh trong duong di min theo thu tu
		sodinh = way_weight_min_i(g, atoi(argv[3]), atoi(argv[4]), &weight,ver); 
		printf("%d\n",weight); // in ra khoang cach tu 1-> 3
		for(i = 0; i <  sodinh; i++){
			cur = jrb_find_int(g.vers, ver[i]);  // tim cai dinh ver do (g.ver = root )
			printf("%s\n", cur->val);  // cau truc typedef trong file jrb.h 1 JRB se co truong val
		}
		drop_graph(g); // huy do thi (drop-xoa) ~ giai phong bo nho!
	}

	// su ly tham so: ./p -n demo.txt 1
	if(strcmp(argv[1], "-n") == 0){
		IS is; 		// tuong  tu nhu FILE *f
		is = new_inputstruct(argv[2]); // argv[2] = demo.txt => mo file
		if(is == NULL){
			printf("File nay 500GB ko the mo dc: %s ^^\n",argv[2]);
			exit(0); // thung dung exit de thoat khi loi!
		}
		// File demo.txt ok thi lam tiep
		while(get_line(is) >= 0){ // ham nay la lay ra 1 dong cua File
			if(is->NF == 2){   // neu ma dong nay no co 2 phan tu vd: 1 2
				add_ver(g,atoi(is->fields[0]),new_jval_s(strdup(is->fields[1])));  // Thêm đỉnh vào đồ thị ham nay can config
			}
			if(is->NF == 3){  // la khi ma da co 2 dinh thi se xet do dai 2 dinh do (khoang cach) 
				add_edge(g,atoi(is->fields[0]), atoi(is->fields[1]),new_jval_i(atoi(is->fields[2]))); //vd  1 -> 2 cach nhau 100
				add_edge(g,atoi(is->fields[1]), atoi(is->fields[0]),new_jval_i(atoi(is->fields[2]))); // tuong tu 2-> 1 cach nhau 100
			}
		}
		jettison_inputstruct(is); // ~fclose(f);

		int i,result,ver[NUMBER_VERS];
		JRB cur;
		int weight;
		result = indegree(g,atoi(argv[3]),ver); // tra ve -1 neu ko co dinh ver.co se tra ve  so dinh cho-> "ver"
		printf("%d\n",result);
		for(i=0; i< result; i++){
			cur =jrb_find_int(g.vers,ver[i]); // tim dinh noi voi argv[3]
			weight = get_weight_i(g,atoi(argv[3]),jval_i(cur->key));
			printf("%d %s %d\n",cur->key,cur->val,weight);
		}
		drop_graph(g);
	}

	// su ly tham so: ./p -s demo.txt 1 3 ( gioi han tap dinh) => dua ra so diem luu thong tu 1-> 3
	if(strcmp(argv[1], "-s") == 0){
		IS is; 		// tuong  tu nhu FILE *f
		is = new_inputstruct(argv[2]); // argv[2] = demo.txt => mo file
		if(is == NULL){
			printf("File nay 500GB ko the mo dc: %s ^^\n",argv[2]);
			exit(0); // thung dung exit de thoat khi loi!
		}
		// File demo.txt ok thi lam tiep
		while(get_line(is) >= 0){ // ham nay la lay ra 1 dong cua File
			if(is->NF == 2){   // neu ma dong nay no co 2 phan tu vd: 1 2
				add_ver(g,atoi(is->fields[0]),new_jval_s(strdup(is->fields[1])));  // Thêm đỉnh vào đồ thị ham nay can config
			}
			if(is->NF == 3){  // la khi ma da co 2 dinh thi se xet do dai 2 dinh do (khoang cach) 
				add_edge(g,atoi(is->fields[0]), atoi(is->fields[1]),new_jval_i(atoi(is->fields[2]))); //vd  1 -> 2 cach nhau 100
				add_edge(g,atoi(is->fields[1]), atoi(is->fields[0]),new_jval_i(atoi(is->fields[2]))); // tuong tu 2-> 1 cach nhau 100
			}
		}
		jettison_inputstruct(is); // ~fclose(f);

		int i,result,ver[NUMBER_VERS];
		JRB cur;
		int weight;

		result = get_all_ways(g, atoi(argv[3]), atoi(argv[4]),ver);

		printf("%d\n",result);
		
		for(i = 0; i < result; i++){
			cur = jrb_find_int(g.vers,ver[i]); // tim dinh noi voi argv[3]
			printf("%d %s\n",cur->key,cur->val);
		}

		drop_graph(g);
	}

}