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
	TK_AND, TK_OR, TK_NOT, TK_LT, TK_LE, TK_EQ, TK_GT,TK_GE, TK_NE
};

struct NODE_H{
	char *keyword;
	enum TOKEN token;
	struct NODE_H *next;
};

struct NODE_H **table;

int fast_mod_exp(int a, int b, int m);


int calculate_hash(char *str);
void insert_linked_list(struct NODE_H **head, char *keyword, enum TOKEN token);

void insert_in_hash_table(struct NODE_H **table, char *str, enum TOKEN token);

void build_hashtable();

enum TOKEN lookup_for_match(char *str);
