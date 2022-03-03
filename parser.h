/***************************************
				|GROUP-37|
  Latika Agrawal       -   2019A7PS0433P
  Surya Prakash        -   2019A7PS0057P
  Surya Rathi          -   2019A7PS0128P
  Saransh Goel         -   2019A7PS0988P
  Mobeen Parwaiz       -   2019A7PS0093P
*****************************************/

#ifndef PARSER_H
#define PARSER_H

#include "parserDef.h"
#include <stdio.h>



void produce_grammar(FILE* fp);

SYMBOL get_symbol(char str[]);

void insert_at_end(rhsnode_ptr* tail, SYMBOL sym);

void parser_init();

int rightmost_set_bit(unsigned long long* num);

void createParseTable();

void populate_first_sets();

void populate_follow_sets();

void print_first_sets();

void print_follow_sets();

void print_first(NON_TERMINAL a);

void print_rule_fset(ull* fset);

void print_parse_table();

void printParseTree(tree_node* root);

// void print_parse_tree_for_tool(tree_node* root, tree_type typ);

void print_rule(int rule_no);

void print_symbol(SYMBOL sym);

tree_node* parseInputSourceCode(FILE* source);

tree_node* create_tree_node();

void add_child(tree_node* parent, tree_node* child);

ull* get_rule_first_set(rhsnode_ptr node);

bool is_superset(ull a[SET_SIZE], ull b[SET_SIZE]);

void free_grammar();

void pretty_print(char* s);

stack* stack_init();

void push(stack* stack_ptr, void* node);

void* top(stack* stack_ptr);

void* pop(stack* stack_ptr);

#endif

//#include "parserDef.h"
//#include <stdio.h>
//
//FirstAndFollow ComputeFirstAndFollowSets(grammar G);
//void createParseTable(FirstAndFollow F, table T);
//void parseInputSourceCode(char* testcaseFile, table T);
//void printParseTree(parseTree PT, char* outfile);