/***************************************
				|GROUP-37|
  Latika Agrawal       -   2019A7PS0433P
  Surya Prakash        -   2019A7PS0057P
  Surya Rathi          -   2019A7PS0128P
  Saransh Goel         -   2019A7PS0988P
  Mobeen Parwaiz       -   2019A7PS0093P
*****************************************/

#ifndef DRIVER_H
#define DRIVER_H

#include<stdio.h>
#include "parserDef.h"

#define MAX_FILE_NAME_LEN 100
#define NUM_TAC_OP 30

#define MAX_LABEL_LEN 20
#define MAX_TEMP_LEN 20
#define MAX_NUM_QUADRUPLES 500
#define MAX_ERROR_STRING_LENGTH 500
#define MAX_ERROR_NUM 500

#define LEXICAL_ERROR 0
#define SYNTACTIC_ERROR 1
#define INTERNAL_ERROR 2

#define MAX_NESTING 200

char terminal_string[NO_OF_TERMINALS][MAX_SYMBOL_LENGTH];
char non_terminal_string[NO_OF_NON_TERMINALS][MAX_SYMBOL_LENGTH];

typedef enum { ast, pt } tree_type;

char parse_tree_file[MAX_FILE_NAME_LEN];
char assembly_file[MAX_FILE_NAME_LEN];
FILE* parse_tree_file_ptr;
FILE* assembly_file_ptr;
int curr_start;
int curr_end;
int lex_err_num;
int syn_err_num;
int sem_err_num;
void store_error(int line_no, int type, char* message);

int num_tree_nodes;

char lexical_errors[MAX_ERROR_NUM][MAX_ERROR_STRING_LENGTH];
char syntactic_errors[MAX_ERROR_NUM][MAX_ERROR_STRING_LENGTH];
char semantic_errors[MAX_ERROR_NUM][MAX_ERROR_STRING_LENGTH];

void print_errors();

#endif
