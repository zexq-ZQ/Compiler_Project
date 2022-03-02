#include "lexerDef.h"

enum SYMBOL {
	program, otherFunctions, mainFunction, stmts, function, input_par, output_par, parameter_list, dataType,
	remaining_list, primitiveDatatype, constructedDatatype, typeDefinitions, declarations, otherStmts, 
	returnStmt, typeDefinition, fieldDefinitions, fieldDefinitions, moreFields, declaration, global_or_not,
	stmt, assignmentStmt, iterativeStmt, conditionalStmt, ioStmt, funCallStmt, singleOrRecId, 
	arithmeticExpression, outputParameters, inputParameters, idList, booleanExpression, var, operator_as, 
	operator_md, logicalOp, relationalOp, optionalReturn, more_ids, definetypestmt, A, epsilon
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