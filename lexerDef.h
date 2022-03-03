/***************************************
				|GROUP-37|
  Latika Agrawal       -   2019A7PS0433P
  Surya Prakash        -   2019A7PS0057P
  Surya Rathi          -   2019A7PS0128P
  Saransh Goel         -   2019A7PS0988P
  Mobeen Parwaiz       -   2019A7PS0093P
*****************************************/

#ifndef LEXERDEF_H
#define LEXERDEF_H

// #include "hashtable.h"
// #include "driver.h"

#define BUFFER_SIZE 10000
#define NO_OF_KEYWORDS 28
#define MAX_LEXEME_LENGTH 100
#define KEY_NOT_FOUND -1
#define NO_OF_TERMINALS 61
#define MAX_SYMBOL_LENGTH 25
#define COLUMN_SIZE 100

typedef enum {
	TK_ASSIGNOP, TK_COMMENT, TK_FIELDID, TK_ID, TK_NUM, TK_RNUM, TK_FUNID, TK_RUID, TK_WITH , TK_PARAMETERS,
	TK_END, TK_WHILE, TK_UNION, TK_ENDUNION, TK_DEFINETYPE, TK_AS, TK_TYPE, TK_MAIN, TK_GLOBAL, TK_PARAMETER,
	TK_LIST, TK_SQL, TK_SQR, TK_INPUT, TK_OUTPUT, TK_INT, TK_REAL, TK_COMMA, TK_SEM, TK_COLON, TK_DOT, 
	TK_ENDWHILE, TK_OP, TK_CL, TK_IF, TK_THEN, TK_ENDIF, TK_READ, TK_WRITE, TK_RETURN, TK_PLUS, TK_MINUS, 
	TK_MUL, TK_DIV, TK_CALL, TK_RECORD, TK_ENDRECORD, TK_ELSE, TK_AND, TK_OR, TK_NOT, TK_LT, TK_LE, TK_EQ, 
	TK_GT, TK_GE, TK_NE,
	LEX_ERROR, EPSILON, DOLLAR, TYPE_ERROR
} TOKEN;

typedef struct {
	TOKEN name;
	union {
		char id[MAX_LEXEME_LENGTH];
		int num;
		double rnum;
	};
	int line_no;
} tokenInfo;

typedef struct {
	char buffer[BUFFER_SIZE];
} twinBuffer;

struct NODE {
	char value;
	struct NODE* next;
	struct NODE* prev;
};

struct LIST {
	int count;
	struct NODE* head;
	struct NODE* tail;
};

//typedef struct {
//	TOKEN token;
//	char* keyword;
//}lookup_element;

//int state;
//int retract_count;

// lookup_element lookup_table [NO_OF_KEYWORDS];

char lexeme[MAX_LEXEME_LENGTH];

extern char terminal_string[NO_OF_TERMINALS][MAX_SYMBOL_LENGTH];

#endif