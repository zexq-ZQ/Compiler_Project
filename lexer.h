#include "lexerDef.h"
#include <stdio.h>

FILE* getStream(FILE* fp);
tokenInfo getNextToken(twinBuffer B);
void removeComments(char* testcaseFile, char* cleanFile);