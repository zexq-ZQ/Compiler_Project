#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"lookup_table.h"
#define HASHSIZE 30

int fast_mod_exp(int a, int b, int m){
	int res = 1;
	while(b>0){
		if(b & 1){
			res = (res * a) % m;
		}
		a = (a * a) % m;
		b >>= 1;
	}
	return res;
}

int calculate_hash(char *str){
	int n = strlen(str);
	int result = 0;

	for(int i = 0; i<n; i++){
		result += ((str[i] * fast_mod_exp(7, i, HASHSIZE)) % HASHSIZE); 
	}	

	return result % HASHSIZE;
}
void insert_linked_list(struct NODE_H **head, char *keyword, TOKEN token){
	struct NODE_H *node = (struct NODE_H*)malloc(sizeof(struct NODE_H));
	node->keyword = keyword;
	node->token = token;	
	node->next = NULL;
	if((*head) == NULL){
		*head = node; 	
		return;
	}	
	struct NODE_H *temp = (*head);
	while(temp->next != NULL){
		temp = temp->next;
	}	
	temp->next = node;
}

void insert_in_hash_table(struct NODE_H **table, char *str, TOKEN token){
	int index = calculate_hash(str);
	insert_linked_list(&table[index], str, token);
}

void build_hashtable(char * source, struct NODE_H** table){
	table = (struct NODE_H**)malloc(HASHSIZE*sizeof(struct NODE_H*));
	for(int i = 0; i<HASHSIZE; i++){
		table[i] = NULL;
	}
	int count = 0;
	FILE *fp = fopen(source,"r");
	TOKEN token;
	while(!feof(fp)){
		char *str = (char*)malloc(20*sizeof(char));
		fscanf(fp, "%s %u\n",str,&token);
		count++;
		printf("%d. %s, %u\n",count,str, token);
		insert_in_hash_table(table, str, token);
	}
	fclose(fp);
}

int lookup_for_match(char *str, struct NODE_H** table){
	int index = calculate_hash(str);
	struct NODE_H *temp = table[index];
	while(temp != NULL){
		if(!strcmp(str, temp->keyword)){
			return temp->token;
		}
		temp = temp->next;
	}
	return -1;
}

/*int main(){
	printf("%d\n",TK_MAIN);
	build_hashtable();
	for(int i = 0; i< HASHSIZE; i++){
		struct NODE_H *temp = table[i];
		printf("%d: ",i+1);
		while(temp != NULL){
			printf("%s:%d, ",temp->keyword,temp->token);
			temp = temp->next;
		}
		printf("\n");
	}
	char str[] = "_main";	
	printf("%s : %d\n",str ,lookup_for_match(str));
	char str1[] = "main";	
	printf("%s : %d\n",str1 ,lookup_for_match(str1));
}*/
