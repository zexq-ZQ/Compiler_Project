#include "lexerDef.h"

enum SYMBOL {
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
	actualOrRedefined , 
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
	elsePart , 
	ioStmt, 
	funCallStmt, 
	singleOrRecId, 
	option_single_constructed, 
	oneExpansion, 
	moreExpansions, 
	arithmeticExpression, 
	expPrime , 
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
	A, 
	epsilon
};

struct FirstAndFollow {
	TOKEN* First;
	TOKEN* Follow;
};

struct rule {
	SYMBOL symbol;
	SYMBOL** RHS;	// a 2D array: each row of symbols is possible replacement of the symbol
};

struct grammar {
	SYMBOL start;
	rule* prod_rules;
};

struct table_row {
	SYMBOL non_terminal;
	TOKEN* First;
	TOKEN* Follow;
};

struct table {
	int n;	// no. of rows
	table_row* rows;
};

struct parseTreeNode {
	SYMBOL symbol;
	int number_children;
	parseTreeNode* children;
};

struct parseTree {
	parseTreeNode* root;
};