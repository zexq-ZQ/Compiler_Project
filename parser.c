/***************************************
				|GROUP-37|
  Latika Agrawal       -   2019A7PS0433P
  Surya Prakash        -   2019A7PS0057P
  Surya Rathi          -   2019A7PS0128P
  Saransh Goel         -   2019A7PS0988P
  Mobeen Parwaiz       -   2019A7PS0093P
*****************************************/

#include "lookup_table.h"
#include "parser.h"
#include "lexer.h"
#include "ADTset.h"
#include "driver.h"
#include <ctype.h>
#include <malloc.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/***************** STACK OPERATIONS *********************/

/*** INITIALISATION ***/

stack* stack_init() {
	stack* stack_ptr = (stack*)malloc(sizeof(stack));
	if (stack_ptr == NULL) {
		perror("stack init error.. memory not available\n");
	}
	stack_ptr->size = INIT_STACK_SIZE;
	stack_ptr->arr =
		(void**)malloc(sizeof(void*) * (stack_ptr->size));
	if (stack_ptr->arr == NULL) {
		perror("stack array member init error.. memory not available\n");
	}
	stack_ptr->top = 0;
	return stack_ptr;
}

/*** PUSH ***/

void push(stack* stack_ptr, void* node) {
	if (stack_ptr->top == (stack_ptr->size)) {
		stack_ptr->size *= 2;
		void** tmp =
			realloc(stack_ptr->arr, sizeof(void*) * stack_ptr->size);
		if (tmp == NULL) {
			perror("Error in doubling stack's size: realloc failed\n");
		}
		else {
			stack_ptr->arr = tmp;
		}
	}
	stack_ptr->arr[stack_ptr->top] = node;
	stack_ptr->top++;
}

/*** POP ***/

void* pop(stack* stack_ptr) {
	if (stack_ptr->top == 0) {
		return NULL;
	}

	stack_ptr->top--;
	return stack_ptr->arr[stack_ptr->top];
}

/*** Return top element without deleting ***/
void* top(stack* stack_ptr) {
	if (stack_ptr->top == 0) {
		return NULL;
	}
	else {
		return stack_ptr->arr[stack_ptr->top - 1];
	}
}


/**
 ***************************** Parser initialization ***************************
 * - initializes all first and follow sets to be empty
 * - inserts entries for terminals and non-terminals in corresponding hashtables
 * - initializes each entry of parsing table as RULE_NOT_MATCHED
 * - sets the presence of an error to be false
 */

void parser_init() {

	// init_hash_table(terminal_table);
	// init_hash_table(non_terminal_table);

	// initialize all first sets to be empty
	for (int i = 0; i < NO_OF_NON_TERMINALS; i++) {
		set_init(first_set[i]);
	}

	/*for (int i = 0; i < NO_OF_TERMINALS; i++) {
		hash_insert(terminal_table, terminal_string[i], i);
	}

	for (int i = 0; i < NO_OF_NON_TERMINALS; i++) {
		hash_insert(non_terminal_table, non_terminal_string[i], i);
	}*/

	// initializing parse table
	for (int i = 0; i < NO_OF_NON_TERMINALS; i++) {
		for (int j = 0; j < NO_OF_TERMINALS; j++) {
			table[i][j] = RULE_NOT_MATCHED;
		}
	}

	num_tree_nodes = 0;
}

/**
 ******************* Insert at end ********************
 * - inserts a symbol at end of the given r's list
 * PARAMETERS:
 * ptr_tail - tail pointer of r's linked list
 * symbol - symbol to be inserted
 */
void insert_at_end(rhsnode_ptr* ptr_tail, SYMBOL symbol) {

	rhsnode_ptr node = (rhsnode_ptr)malloc(sizeof(rhsnode));

	if (node == NULL) {
		perror("Insertion at end failed\n");
		exit(1);
	}
	node->symbol = symbol;
	node->next = NULL;

	if (*ptr_tail != NULL) {
		(*ptr_tail)->next = node;
	}
	*ptr_tail = node;
}

SYMBOL get_symbol(char str[]) {
	SYMBOL sym;
	if ((str[0] == 'T') && (str[1] == 'K')) {
		sym.is_terminal = true;
		sym.t = lookup_for_match(str, terminal_table);
		
	}
	else {

		sym.is_terminal = false;
		sym.nt = lookup_for_match(str, non_terminal_table);
	}
	return sym;
}

/**
 ************************ Produce grammar ***********************
 * - takes a file as input which consists of grammar for language
 */
void produce_grammar(FILE* fptr) {

	int r_num = 0;
	char buffer[MAX_LENGTH_RHS];

	build_hashtable("TOKENS.txt", terminal_table);
	build_hashtable("non_terminals.txt", non_terminal_table);

	while (fgets(buffer, sizeof(buffer), fptr) != NULL) {
		char* sym_read;
		int i;

		sym_read = strtok(buffer, " \n");
		for (i = 0; sym_read != NULL; i++) {
			if (i == 0) // LHS of a production
			{
				grammar[r_num].lhs = get_symbol(sym_read).nt;
				grammar[r_num].head = NULL;
				grammar[r_num].tail = NULL;
			}
			else {
				SYMBOL sym = get_symbol(sym_read);
				insert_at_end(&(grammar[r_num].tail), sym);
				if (grammar[r_num].head == NULL) {
					grammar[r_num].head = grammar[r_num].tail;
				}
			}
			sym_read = strtok(NULL, " \n");
		}
		r_num++;
	}
}


/**
 *********** rightmost set bit ************** 
 * - Find rightmost_set_bit in a given number
 * - return int - position of the bit
 */
int rightmost_set_bit(ull* num) {
	ull temp = (*num & (*num - 1));
	int pos = ULL_NUM_BITS - 1 - log2(temp ^ *num);
	*num = temp;
	return pos;
}

/**
 * Create a parse table using grammar and first and follow sets.
 */
void createParseTable() {
	for (int i = 0; i < NO_OF_RULES; i++) {
		// printf("Filling r")
		NON_TERMINAL nt = grammar[i].lhs;
		set rhs_first_set = get_rule_first_set(grammar[i].head);

		for (int j = 0; j < SET_SIZE; j++) {
			ull num = rhs_first_set[j]; // first of lhs

			while (num) {
				int pos = rightmost_set_bit(&num); // position of terminal in first set
				pos += (ULL_NUM_BITS * j);
				if (EPSILON == pos) {
					for (int j = 0; j < SET_SIZE; j++) {
						ull num =
							follow_set[nt][j]; // if epsilon in first set, add follow also
						while (num) {
							int pos = rightmost_set_bit(&num);
							pos += (ULL_NUM_BITS * j);
							if (EPSILON != pos) {
								table[nt][pos] = i;
							}
						}
					} // end of for
				}   // end of if - epsilon presence
				else {
					table[nt][pos] = i; // don't consider epsilon as terminal
				}
			} // end of while - adding elements of first set
		}   // end of for - traversing in all subparts of bitstring
	}     // end of for - travwersal in all rs
}

/**
 * - Do the actual parsing of the source code
 * - PARAMETER: source - source file pointer
 * - RETURNS: tree_node* - root of the formed parse tree
 */
tree_node* parseInputSourceCode(FILE* source) {

	stack* main_stack = stack_init();
	stack* aux_stack = stack_init();
	tree_node* root = create_tree_node();

	root->symbol.nt = program;
	root->symbol.is_terminal = false;
	push(main_stack, root); // push start symbol on stack
	tokenInfo tkn = getNextToken(getStream(source));

	while (true) {
		num_tree_nodes++;
		tree_node* node = (tree_node*)pop(main_stack);
		if ((node != NULL) && (node->symbol).is_terminal == true) {
			if (node->symbol.t == EPSILON) {
				node->token.name = EPSILON;
				strncpy(node->token.id, "epsilon", MAX_LEXEME_LENGTH);
				continue;
			}
			if (node->symbol.t != tkn.name) // terminal on top of stack does not match
										 // with lookhead symbol
			{
				// node->token.name = LEX_ERROR;		
				char* err_msg = (char*)malloc(sizeof(char) * MAX_ERR_LEN);
				snprintf(err_msg, MAX_ERR_LEN, "Expected \"%s\", Found \"%s\" \n", terminal_string[node->symbol.t], terminal_string[tkn.name]);

				store_error(tkn.line_no, SYNTACTIC_ERROR, err_msg);

				printf("Popping token %s from stack top\n",
					terminal_string[node->symbol.t]);
				// tkn = getNextToken(getStream(source));
				node = (tree_node*)pop(main_stack);
				if (node == NULL) {
					store_error(tkn.line_no, SYNTACTIC_ERROR, "Panic Mode Error Recovery Not possible, Partial parse tree formed");
					return root;
				}
				continue;
			}
			else { // la token and stack top match
				node->token.line_no = tkn.line_no;
				node->token.name = tkn.name;
				switch (tkn.name) {
				case TK_NUM:
					node->token.num = tkn.num;
					break;

				case TK_RNUM:
					node->token.rnum = tkn.rnum;
					break;

				default:
					// node->token.id = (char *)malloc(sizeof(MAX_LEXEME_LENGTH));
					strncpy(node->token.id, tkn.id, MAX_LEXEME_LENGTH);
				}
			}

			tkn = getNextToken(getStream(source));
			continue;
		}

		if (tkn.name == LEX_ERROR) {
			store_error(tkn.line_no, LEXICAL_ERROR, tkn.id);

			tkn = getNextToken(getStream(source));
			push(main_stack, node);
			continue;
		}
		if (node == NULL) {
			if (tkn.name != DOLLAR) // r not read completely but stack became empty
			{
				store_error(tkn.line_no, LEXICAL_ERROR, "Extra symbols in the source code");
			}
			else {
				// printf("\nInput source code is now syntactically correct...........\n\n");
			}
			break;
		}

		int r_no =table[node->symbol.nt][tkn.name];
		if (r_no == RULE_NOT_MATCHED) {
			// printf("[%s][%s]", non_terminal_string[node->symbol.nt],
				  //  terminal_string[tkn.name]);      
			store_error(tkn.line_no, LEXICAL_ERROR, "No matching r found in grammar");

			printf("Waiting for an element in follow of \"%s\"\n" , non_terminal_string[node->symbol.nt]);

			while (set_find_elem(follow_set[node->symbol.nt], tkn.name) == false) {
				tkn = getNextToken(getStream(source));
				if (tkn.name == DOLLAR) {
					store_error(tkn.line_no, SYNTACTIC_ERROR, "Panic Mode Error Recovery Not possible, Partial parse tree formed");
					return root;
				}
			}
			printf("Token \"%s\" found at line number %d\n",
				terminal_string[tkn.name], tkn.line_no);
			printf("Resuming parsing\n");
			continue;
		}
		rule r = grammar[r_no];
		rhsnode_ptr rhs_ptr = r.head;

		while (rhs_ptr != NULL) {
			tree_node* temp = create_tree_node();
			temp->parent = node;
			temp->symbol = rhs_ptr->symbol;
			add_child(node, temp);
			push(aux_stack, temp);
			rhs_ptr = rhs_ptr->next;
		}

		tree_node* temp = (tree_node*)pop(aux_stack);

		while (temp != NULL) {
			push(main_stack, temp);
			temp = (tree_node*)pop(aux_stack);
		}
	} // end of while(true) loop : parsing done
	return root;
}

/**
 * - Clear all entries in the grammar linked lists array.
 */
void free_grammar() {
	for (int i = 0; i < NO_OF_RULES; i++) {
		rhsnode_ptr temp = grammar[i].head, prev;
		while (temp != NULL) {
			prev = temp->next;
			free(temp);
			temp = prev;
		}
	}
}

/**
 * @brief Print a string as center aligned
 *
 */
void pretty_print(char* s) {
	int column_size = COLUMN_SIZE, len, left_margin;

	len = strlen(s);
	left_margin = (column_size - len) / 2;

	for (int i = 0; i < left_margin; i++)
	{
		printf(" ");
	}

	printf("%s", s);

	int right_margin = left_margin;

	if (len % 2 == 1)
		right_margin++;
	for (int i = 0; i < right_margin; i++)
	{
		printf(" ");
	}

	printf("|");
}


///**
// * @brief Temp function to test if a given node will be involved in type computation
// *
// * i.e. if it is involved in an assignment stmt, e.g. EXPRESSION, RELATIONALEXPR, TERM, FACTOR
// */
//bool is_expr_node(NON_TERMINAL nt) {
//	switch (nt) {
//	case EXPRESSION:
//	case RELATIONALEXPR:
//	case NONUNARYARITHMETICEXPR:
//	case UNARYARITHMETICEXPR:
//	case TERM:
//	case FACTOR:
//		return true;
//		break;
//	default:
//		return false;
//		break;
//	}
//}

/**
 * @brief Print a node object
 *
 * @param node
 */
void print_node_for_tool(tree_node* node /*, tree_type typ*/) {

	num_tree_nodes++;
	if (node == NULL)
		return;
	bool is_terminal = (node->symbol).is_terminal;
	if (is_terminal == true) {

		switch ((node->token).name) {
		case TK_NUM:
			fprintf(parse_tree_file_ptr, "[num %d]", (node->token).num);
			break;

		case TK_RNUM:
			fprintf(parse_tree_file_ptr, "[rnum %f]", (node->token).rnum);
			break;

		default: {
			// if(node->token.name == ID){
			// 	printf("ast :  %s, s : %d, e : %d\n", node->token.id, node->line_nos.start, node->line_nos.end);
			// }
			char tkn_name[MAX_LEXEME_LENGTH];
			strncpy(tkn_name, terminal_string[(node->symbol).t], MAX_LEXEME_LENGTH);

			for (int i = 0; i < strlen(tkn_name); i++)
				tkn_name[i] = tolower(tkn_name[i]);

			if (node->token.name != TK_SQL && node->token.name != TK_SQR)
				fprintf(parse_tree_file_ptr, "[%s %s] ", tkn_name, node->token.id);

			else
				fprintf(parse_tree_file_ptr, "[%s] ", tkn_name);

		} break;
		}
	}
	else {
		fprintf(parse_tree_file_ptr, "[%s(%d)", non_terminal_string[(node->symbol).nt], node->num_child);

		//if (is_expr_node(node->symbol.nt) && typ == ast) { //it's a node corresponding to RHS of an expression and printing ast
		//	fprintf(parse_tree_file_ptr, "{%s} ", terminal_string[get_expr_type(node, node->scope_sym_tab).name]);
		//}
	}
}

/**
 * @brief Print the parse tree
 *
 * @param root root node of the tree
 */
void printParseTree(tree_node* root /*, tree_type typ*/) {
	if (root == NULL)
		return;
	print_node_for_tool(root /*, typ*/);

	if (root->leftmost_child)
		printParseTree(root->leftmost_child /*, typ*/);
	//   else
	// 	fprintf(parse_tree_file_ptr,"]");

	if (root->leftmost_child) {
		tree_node* temp = root->leftmost_child->sibling;
		while (temp != NULL) {
			printParseTree(temp /*, typ*/);
			temp = temp->sibling;
		}
	}
	if (root->symbol.is_terminal == false)
		fprintf(parse_tree_file_ptr, "]");
}

/**
 * @brief Print all first sets of the given grammar
 *
 */
void print_first_sets() {
	for (int i = 0; i < NO_OF_NON_TERMINALS; i++) {
		printf("FIRST[");
		char* s = non_terminal_string[i];
		printf("%*s%*s", (int)(15 + strlen(s) / 2), s, (int)(15 - strlen(s) / 2),
			"");
		printf("] = { ");
		for (int j = 0; j < SET_SIZE; j++) {
			for (int k = 0; k < ULL_NUM_BITS; k++) {
				if (set_find_elem(first_set[i], j * ULL_NUM_BITS + k) == true) {
					printf("%s  ", terminal_string[j * ULL_NUM_BITS + k]);
				}
			}
		}
		printf("}\n");
	}
}

/**
 * @brief Print the follow sets of the given grammar
 *
 */
void print_follow_sets() {
	for (int i = 0; i < NO_OF_NON_TERMINALS; i++) {
		printf("FOLLOW[");
		char* s = non_terminal_string[i];
		printf("%*s%*s", (int)(15 + strlen(s) / 2), s, (int)(15 - strlen(s) / 2),
			"");
		printf("] = { ");
		for (int j = 0; j < SET_SIZE; j++) {
			for (int k = 0; k < ULL_NUM_BITS; k++) {
				if (set_find_elem(follow_set[i], j * ULL_NUM_BITS + k) == true) {
					printf("%s  ", terminal_string[j * ULL_NUM_BITS + k]);
				}
			}
		}
		printf("}\n");
	}
}

/**
 * @brief Print the first set of a given non-terminal
 *
 */
void print_first(NON_TERMINAL nt) {
	// printf("\n{");
	printf("FIRST(%s) = { ", non_terminal_string[nt]);
	for (int j = 0; j < SET_SIZE; j++) {
		for (int k = 0; k < ULL_NUM_BITS; k++) {
			if ((first_set[nt][j] & ((1ULL << (ULL_NUM_BITS - 1)) >> k)) != 0) {
				// printf(" %d ", j);
				printf("%s, ", terminal_string[j * ULL_NUM_BITS + k]);
			}
		}
	}
	printf(" }\n");
}

/**
 * Computing the follow sets of the non-terminals in grammar
 */

void populate_follow_sets() {
	bool is_changed = true;
	int lhs;
	int rhs_sym;
	rhsnode_ptr rhs_ptr;

	set_add_elem(follow_set[program], DOLLAR);

	while (is_changed == true) // traverse until convergence
	{
		is_changed = false;
		for (int i = 0; i < NO_OF_RULES; i++) {
			lhs = grammar[i].lhs;
			rhs_ptr = grammar[i].head;
			rhsnode_ptr temp = rhs_ptr;
			while (temp != NULL) // traverse till end of the r
			{
				if ((temp->symbol).is_terminal == false) {
					rhs_sym = ((temp->symbol).nt);
					set rhs_rule_set = get_rule_first_set(temp->next);
					set tmp_follow = (ull*)malloc(sizeof(ull) * SET_SIZE);

					if (tmp_follow == NULL) {
						perror("Follow set memory allocation failed\n");
						exit(1);
					}

					for (int j = 0; j < SET_SIZE; j++) {
						tmp_follow[j] = follow_set[rhs_sym][j];
					}

					bool eps_in_rhs = false;

					if (set_find_elem(rhs_rule_set, EPSILON) ==
						true) // eps present in this r
					{
						eps_in_rhs = true;
					}

					set_remove_elem(rhs_rule_set, EPSILON);

					set_union(follow_set[rhs_sym], follow_set[rhs_sym], rhs_rule_set);

					if ((eps_in_rhs == true) || (temp->next == NULL)) {
						set_union(follow_set[rhs_sym], follow_set[rhs_sym],
							follow_set[lhs]);
					}

					for (int j = 0; j < SET_SIZE; j++) {
						if (follow_set[rhs_sym][j] != tmp_follow[j]) {
							is_changed = true;
						}
					}
					free(tmp_follow);
				}
				temp = temp->next;
			} // end of r linked list traversal while loop
		}   // end of for - grammar traversal
	}     // end of while - infinite loop until convergence
}

/**
 * Computing first sets of given non-terminals of the grammar
 */
void populate_first_sets() {
	bool is_changed = true;
	int lhs;
	rhsnode_ptr rhs_ptr;
	while (is_changed == true) // traverse until convergence
	{
		is_changed = false;
		for (int i = 0; i < NO_OF_RULES; i++) {
			lhs = grammar[i].lhs;
			rhs_ptr = grammar[i].head;
			if ((rhs_ptr->symbol).is_terminal ==
				true) // if terminal, add it and move ahead
			{
				TOKEN t = (rhs_ptr->symbol).t;
				if (set_find_elem(first_set[lhs], t) ==
					false) // check if terminal already there in the first set or not
				{
					set_add_elem(first_set[lhs], t);
					is_changed = true;
				}
			}
			else // NON_TERMINAL
			{
				rhsnode_ptr temp = rhs_ptr;
				ull* rhs_symbol_fset;
				ull* lhs_symbol_fset = first_set[lhs];
				while (temp != NULL) // traverse till end of the r
				{
					if ((temp->symbol).is_terminal ==
						true) // if  terminal add and move to next r
					{
						TOKEN t = (temp->symbol).t;
						if (set_find_elem(first_set[lhs], t) ==
							false) // check if terminal already there in the
								   // first set or not
						{
							set_add_elem(first_set[lhs], t);
							is_changed = true;
						}
						break;
					}

					rhs_symbol_fset = first_set[(temp->symbol).nt];

					bool is_diff = false;
					bool eps_in_rhs = false;
					bool eps_in_lhs = false;

					if (set_find_elem(rhs_symbol_fset,
						EPSILON) ==
						true) // remove epsilon from current nt before checking things
					{
						eps_in_rhs = true;
						set_remove_elem(rhs_symbol_fset, EPSILON);
					}

					if (set_find_elem(lhs_symbol_fset, EPSILON) == true) {
						eps_in_lhs = true;
					}

					if (is_superset(lhs_symbol_fset, rhs_symbol_fset) ==
						false) // rhs nt has a terminal which lhs nt does not
							   // have in it's fset
					{
						is_changed = true;
						set_union(lhs_symbol_fset, lhs_symbol_fset, rhs_symbol_fset);

						if (eps_in_rhs ==
							false) // if rhs nt does not have eps, no need to go further
						{
							break;
						}
						else {
							set_add_elem(rhs_symbol_fset, EPSILON); // set eps back to rhs nt
							if (eps_in_lhs == false) {
								if (temp->next == NULL) // only add eps to lhs nt if rhs is last
														// nt in the r
								{
									set_add_elem(lhs_symbol_fset, EPSILON);
									is_changed = true;
								}
							}
						}
					}
					else // if is_diff == false, break;
					{
						if (eps_in_rhs == true) {
							set_add_elem(rhs_symbol_fset, EPSILON); // set eps back to rhs nt
							if (eps_in_lhs == false) {
								if (temp->next == NULL) // only add eps to lhs nt if rhs is last
														// nt in the r
								{
									set_add_elem(lhs_symbol_fset, EPSILON);
									is_changed = true;
								}
							}
						}
						break; //
					}
					temp = temp->next;
				} // end of r linked list traversal while loop
			}   // end of else (non-terminal branch)
		}     // end of for - grammar traversal
	}       // end of while - infinite loop until convergence
} // end of function

/**
 * @brief Get the first set for a string of terminals and non-terminals
 *
 * @param node starting node of the string
 */
set get_rule_first_set(rhsnode_ptr node) {

	set fset = (set)malloc(sizeof(ull) * SET_SIZE);

	if (fset == NULL) {
		perror("get r first set failed\n");
		exit(1);
	}

	set_init(fset);
	rhsnode_ptr temp = node;
	int sym;

	while (temp != NULL) {
		if ((temp->symbol).is_terminal == true) {
			sym = (temp->symbol).t;
			set_add_elem(fset, sym);
			return fset;
		}

		else {
			sym = (temp->symbol).nt;
			set_union(fset, fset, first_set[sym]);

			if (set_find_elem(first_set[sym], EPSILON) == false) // eps not in the nt
			{
				return fset;
			}
			else {
				if (temp->next != NULL) {
					set_remove_elem(fset, EPSILON);
				}
			} // end of else - eps present in fset
		}   // end of else - is nt

		temp = temp->next;
	} // end of while - ll traversal
	return fset;
}
/**
 * @brief Print a grammar r
 *
 * @param r_no r number to be printed
 */
void print_rule(int r_no) {
	int lhs = grammar[r_no].lhs;
	rhsnode_ptr head = grammar[r_no].head;
	printf("%s -> ", non_terminal_string[lhs]);
	while (head != NULL) {
		print_symbol(head->symbol);
		printf(" ");
		head = head->next;
	}
	printf("\n");
}



/**
 * @brief Print the parse table entries
 *
 */
void print_parse_table() {
	for (int i = 0; i < NO_OF_NON_TERMINALS; i++) {
		for (int j = 0; j < NO_OF_TERMINALS; j++) {
			if (table[i][j] != RULE_NOT_MATCHED) {
				printf("parse_table[%s][%s] : ", non_terminal_string[i],
					terminal_string[j]);
				print_rule(table[i][j]);
			}
		}
	}
}

void print_symbol(SYMBOL sym) {
	if (sym.is_terminal == true) {
		printf("%s", terminal_string[sym.t]);
	}
	else {
		printf("%s", non_terminal_string[sym.nt]);
	}
}

tree_node* create_tree_node() {
	tree_node* node = (tree_node*)malloc(sizeof(tree_node));
	if (node == NULL) {
		perror("tree_node allocation error..\n");
		exit(0);
	}
	node->visited = false;
	node->parent = NULL;
	node->sibling = NULL;
	node->leftmost_child = NULL;
	node->rightmost_child = NULL;
	node->node_inh = NULL;
	node->node_syn = NULL;
	node->num_child = 0;
	node->extra_args = NULL;
	node->addr = NULL;
	node->label.next_label = NULL;
	node->label.cnstrct_code_begin = NULL;
	strncpy(node->token.id, "", MAX_LEXEME_LENGTH);
	node->line_nos.start = 0;
	node->line_nos.end = 0;
	return node;
}

void add_child(tree_node* parent, tree_node* child) {
	if (parent->rightmost_child == NULL) {
		parent->leftmost_child = child;
		parent->rightmost_child = child;
	}
	else {
		parent->rightmost_child->sibling = child;
		parent->rightmost_child = child;
	}
	parent->num_child++;
	child->parent = parent;
	child->sibling = NULL;
	if (child->symbol.is_terminal == false) {
		if (parent->line_nos.start == 0)
			parent->line_nos.start = child->line_nos.start;
		if (child->line_nos.end != 0)
			parent->line_nos.end = child->line_nos.end;
	}
}
