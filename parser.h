#include "parserDef.h"
#include <stdio.h>

FirstAndFollow ComputeFirstAndFollowSets(grammar G);
void createParseTable(FirstAndFollow F, table T);
void parseInputSourceCode(char* testcaseFile, table T);
void printParseTree(parseTree PT, char* outfile);