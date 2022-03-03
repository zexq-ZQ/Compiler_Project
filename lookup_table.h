#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "lexerDef.h"
#define HASHSIZE 30

struct NODE_H{
	char *keyword;
	TOKEN token;
	struct NODE_H *next;
};

struct NODE_H **table;

int fast_mod_exp(int a, int b, int m);


int calculate_hash(char *str);
void insert_linked_list(struct NODE_H **head, char *keyword, TOKEN token);

void insert_in_hash_table(struct NODE_H **table, char *str, TOKEN token);

void build_hashtable();

int lookup_for_match(char *str);
