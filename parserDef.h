/***************************************
				|GROUP-37|
  Latika Agrawal       -   2019A7PS0433P
  Surya Prakash        -   2019A7PS0057P
  Surya Rathi          -   2019A7PS0128P
  Saransh Goel         -   2019A7PS0988P
  Mobeen Parwaiz       -   2019A7PS0093P
*****************************************/

#ifndef PARSERDEF_H
#define PARSERDEF_H


#include "lexerDef.h"
#include "ADTset.h"
// #include "driver.h"


#define MAX_LENGTH_RHS 100		
#define RULE_NOT_MATCHED -1		
#define MAX_ERR_LEN 300	
#define NO_OF_NON_TERMINALS 53
#define NO_OF_RULES 95
#define INIT_STACK_SIZE 200

typedef enum {
	program,
	otherFunctions,
	mainFunction,
	stmts,
	function,
	input_par,
	output_par,
	parameter_list,
	dataType,
	remaining_list,
	primitiveDatatype,
	constructedDatatype,
	typeDefinitions,
	actualOrRedefined,
	declarations,
	otherStmts,
	returnStmt,
	typeDefinition,
	fieldDefinitions,
	fieldDefinition,
	moreFields,
	fieldtype,
	declaration,
	global_or_not,
	stmt,
	assignmentStmt,
	iterativeStmt,
	conditionalStmt,
	elsePart,
	ioStmt,
	funCallStmt,
	singleOrRecId,
	option_single_constructed,
	oneExpansion,
	moreExpansions,
	arithmeticExpression,
	expPrime,
	term,
	termPrime,
	factor,
	highPrecedenceOperator,
	lowPrecedenceOperators,
	outputParameters,
	inputParameters,
	idList,
	booleanExpression,
	var,
	logicalOp,
	relationalOp,
	optionalReturn,
	more_ids,
	definetypestmt,
	A
} NON_TERMINAL;


extern char non_terminal_string[NO_OF_NON_TERMINALS][MAX_SYMBOL_LENGTH];

extern  FILE* parse_tree_file_ptr;

typedef struct {
	union
	{
		TOKEN t;
		NON_TERMINAL nt;
	};
	bool is_terminal;
} SYMBOL;

typedef struct rhsnode
{
	SYMBOL symbol;
	struct rhsnode* next;
} rhsnode;

typedef struct rhsnode* rhsnode_ptr;

typedef struct
{
	NON_TERMINAL lhs;
	rhsnode_ptr head;
	rhsnode_ptr tail;
} rule;

typedef struct tree_node tree_node;

typedef struct tree_node
{
	SYMBOL symbol;
	tokenInfo token;
	int num_child;
	bool visited;
	void* extra_args;
	struct {
		struct {
			char* true_label;
			char* false_label;
		}boolean;
		char* next_label;
		char* cnstrct_code_begin;
	}label;
	char* addr;
	struct {
		int start;
		int end;
	}line_nos;
	tree_node * parent;
	tree_node * sibling;
	tree_node * leftmost_child;
	tree_node * rightmost_child;

	tree_node * node_inh;
	tree_node * node_syn;
}tree_node;


typedef struct
{
	void** arr;
	int top;
	int size;
} stack;


ull first_set[NO_OF_NON_TERMINALS][SET_SIZE];
ull follow_set[NO_OF_NON_TERMINALS][SET_SIZE];

rule grammar[NO_OF_RULES];

struct NODE_H** terminal_table;
struct NODE_H** non_terminal_table;

int table[NO_OF_NON_TERMINALS][NO_OF_TERMINALS];
int num_tree_nodes;

#endif