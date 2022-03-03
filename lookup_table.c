#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define HASHSIZE 30
enum TOKEN {
	TK_ASSIGNOP, TK_COMMENT, TK_FIELDID, TK_ID, TK_NUM, TK_RNUM, TK_FUNID, TK_RUID, 
	TK_WITH , TK_PARAMETERS, TK_END, TK_WHILE, TK_UNION, TK_ENDUNION, TK_DEFINETYPE, TK_AS, TK_TYPE, 
	TK_MAIN, TK_GLOBAL, TK_PARAMETER, TK_LIST, TK_SQL, TK_SQR, TK_INPUT, TK_OUTPUT, TK_INT, TK_REAL, 
	TK_COMMA, TK_SEM, TK_COLON, TK_DOT, TK_ENDWHILE, TK_OP, TK_CL, TK_IF, TK_THEN, TK_ENDIF, TK_READ,
	TK_WRITE, TK_RETURN, TK_PLUS, TK_MINUS, TK_MUL, TK_DIV, TK_CALL, TK_RECORD, TK_ENDRECORD, TK_ELSE,
	TK_AND, TK_OR, TK_NOT, TK_LT, TK_LE, TK_EQ, TK_GT, TK_NE
};

struct NODE_H{
	char *keyword;
	enum TOKEN token;
	struct NODE_H *next;
};

struct NODE_H **table;

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
void insert_linked_list(struct NODE_H **head, char *keyword, enum TOKEN token){
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

void insert_in_hash_table(struct NODE_H **table, char *str, enum TOKEN token){
	int index = calculate_hash(str);
	insert_linked_list(&table[index], str, token);
}

void build_hashtable(){
	table = (struct NODE_H**)malloc(HASHSIZE*sizeof(struct NODE_H*));
	for(int i = 0; i<HASHSIZE; i++){
		table[i] = NULL;
	}
	int count = 0;
	FILE *fp = fopen("keywords.txt","r");
	enum TOKEN token;
	while(!feof(fp)){
		char *str = (char*)malloc(20*sizeof(char));
		fscanf(fp, "%s %u\n",str,&token);
		count++;
		//printf("%d. %s, %u\n",count,str, token);
		insert_in_hash_table(table, str, token);
	}
	fclose(fp);
}

int lookup_for_match(char *str){
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
