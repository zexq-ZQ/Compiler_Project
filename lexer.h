/***************************************
				|GROUP-37|
  Latika Agrawal       -   2019A7PS0433P
  Surya Prakash        -   2019A7PS0057P
  Surya Rathi          -   2019A7PS0128P
  Saransh Goel         -   2019A7PS0988P
  Mobeen Parwaiz       -   2019A7PS0093P
*****************************************/

#include "lexerDef.h"
#include <stdio.h>

FILE* getStream(FILE* fp);
tokenInfo getNextToken(twinBuffer B);
void removeComments(char* testcaseFile, char* cleanFile);
void fill_lookup_table();
