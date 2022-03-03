/***************************************
				|GROUP-37|
  Latika Agrawal       -   2019A7PS0433P
  Surya Prakash        -   2019A7PS0057P
  Surya Rathi          -   2019A7PS0128P
  Saransh Goel         -   2019A7PS0988P
  Mobeen Parwaiz       -   2019A7PS0093P
*****************************************/

#include "lexer.h"
#include <ctype.h>
#include "lookup_table.h"
#include <stdbool.h>
#include <math.h>

int ifBet_a_to_z_not_b_to_d(char c) {
	if (c >= 'a' && c <= 'z' && !(c >= 'b' && c <= 'd')) {
		return 1;
	}
	return 0;
}

int ifBet_a_to_z_or_A_to_Z(char c) {
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
		return 1;
	}
	return 0;
}

int ifBet_a_to_z(char c) {
	if (c >= 'a' && c <= 'z') {
		return 1;
	}
	return 0;
}

int ifBet_b_to_d(char c) {
	if (c >= 'b' && c <= 'd') {
		return 1;
	}
	return 0;
}

int ifBet_0_to_9(char c) {
	int temp = c - '0';
	if (temp >= 0 && temp <= 9) {
		return 1;
	}
	return 0;
}

int ifBet_2_to_7(char c) {
	int temp = c - '0';
	if (temp >= 2 && temp <= 7) {
		return 1;
	}
	return 0;
}

char getcharM(twinBuffer* B, int* index, struct LIST* list) {
	char c = *(*(B->buffer));
	(*(B->buffer))++;
	(*(B->line_no))++;
	struct NODE* new_node = (struct NODE*)malloc(sizeof(struct NODE));
	new_node->value = c;
	new_node->next = NULL;
	if (list->head == NULL) {
		new_node->prev = NULL;
		list->head = new_node;
		list->tail = new_node;
		list->count++;
		return c;
	}
	new_node->prev = list->tail;
	list->tail->next = new_node;
	list->tail = new_node;
	list->count++;
	return c;
}

void ungetcM(twinBuffer* B, int* index, struct LIST* list) {
	if (list->head == NULL) {
		printf("something wrong: not able to retract in empty lexeme\n");
		exit(0);
	}
	(*(B->buffer))--;
	(*(B->line_no))--;
	if (list->head == list->tail) {
		free(list->head);
		list->head = NULL;
		list->tail = NULL;
		list->count = 0;
		return;
	}
	struct NODE* temp = list->tail;
	list->tail = temp->prev;
	list->tail->next = NULL;
	list->count--;
	free(temp);
}

char* list_to_array(struct NODE* head, int count) {
	char* lexeme = (char*)malloc((count + 1) * sizeof(char));
	struct NODE* temp = head;
	int index = 0;
	while (temp != NULL) {
		lexeme[index] = temp->value;
		temp = temp->next;
		index++;
	}
	lexeme[index] = '\0';
	return lexeme;
}

tokenInfo ret_tokenInfo(struct LIST* list, TOKEN token, int line_num) {
	int count = list->count;
	tokenInfo next_token;
	next_token.name = token;
	next_token.line_no = line_num;
	if (token == TK_NUM) {
		char* id;
		strcpy(id, list_to_array(list->head, count));
		next_token.num = atoi(id);
	}
	else if (token == TK_RNUM) {
		char* id; double rnum = 0; int i = 0;
		strcpy(id, list_to_array(list->head, count));
		while (id[i] != '.') {
			rnum = rnum * 10;
			rnum = rnum + ((int)id[i] - 48);
		}
		rnum = id[i++] * 0.1;
		rnum = id[i++] * 0.01;
		if (id[i++] == 'E') {
			bool sign = false;	// positive sign
			int E = 0;
			if (id[i] == '-') {
				++i;
				sign = true;
			}
			else if (id[i] == '+') {
				++i;
			}
			while (id[i] != '\0') {
				E = E * 10;
				E = E + ((int)id[i] - 48);
			}
			if (sign == true) {
				E = -1 * E;
			}
			rnum = rnum * pow(10, E);
		}
		next_token.rnum = rnum;
	}
	else {
		strcpy(next_token.id, list_to_array(list->head, count));
	}
	return next_token;
}

//void removeComments(char* testcaseFile, char** cleanFile);
//tokenInfo getNextToken(twinBuffer B);
twinBuffer getStream(FILE* fp) {
	char c;
	int count = 0;
	while (!feof(fp)) {
		c = fgetc(fp);
		count++;
	}
	int index = 0;
	twinBuffer result;
	char* arr1 = (char*)malloc(count * sizeof(char));
	int* arr2 = (int*)malloc(count * sizeof(int));
	result.buffer = &arr1;
	result.line_no = &arr2;
	rewind(fp);
	int line_no = 1;
	while (!feof(fp)) {
		c = fgetc(fp);
		if (c == '\n') {
			line_no++;
		}
		(*(result.buffer))[index] = c;
		(*(result.line_no))[index] = line_no;
		index++;
	}
	(*(result.buffer))[index - 1] = EOF;
	(*(result.line_no))[index - 1] = line_no;

	while (*(*result.buffer) != EOF) {
		tokenInfo info_of_token;
		info_of_token = getNextToken(result);
		TOKEN token = info_of_token.name;
		if (token == TK_RNUM) {
			 double rnum = info_of_token.rnum;
			 printf("line no: %d, lexeme: %lf, token: %d\n", info_of_token.line_no, rnum, token);
		}
		else if (token == TK_NUM) {
			int num = info_of_token.num;
			printf("line no: %d, lexeme: %d, token: %d\n", info_of_token.line_no, num, token);
		}
		else {
			char* lexeme = info_of_token.id;
			printf("line no: %d, lexeme: %s, token: %d\n", info_of_token.line_no, lexeme, token);
		}
			
	}
	return result;

}

tokenInfo getNextToken(twinBuffer B) {
	build_hashtable();
	int index = 0;
	struct LIST* list = (struct LIST*)malloc(sizeof(struct LIST));
	list->count = 0;
	list->head = NULL;
	list->tail = NULL;
	tokenInfo ret_token;
	char c = *(*(B.buffer));
	if (c <= 32) {
		while (c <= 32) {
			if (c == EOF) {
				tokenInfo temp;
				temp.name = 0;
				char arr3[] = "eof";
				strcpy(temp.id, arr3);
				return temp;
			}
			(*(B.buffer))++;
			(*(B.line_no))++;
			c = *(*(B.buffer));
		}
	}
	c = getcharM(&B, &index, list);
	if (c == '<') {
		c = getcharM(&B, &index, list);
		if (c == '-') {
			c = getcharM(&B, &index, list);
			if (c == '-') {
				c = getcharM(&B, &index, list);
				if (c == '-') {
					return ret_tokenInfo(list, TK_ASSIGNOP, *(*(B.line_no) - 1));
				}
			}
		}
		else if (c == '=') {
			return ret_tokenInfo(list, TK_LE, *(*(B.line_no) - 1));
		}
		else {
			ungetcM(&B, &index, list);
			return ret_tokenInfo(list, TK_LT, *(*(B.line_no) - 1));
		}
	}
	else if (c == '=') {
		c = getcharM(&B, &index, list);
		if (c == '=') {
			return ret_tokenInfo(list, TK_EQ, *(*(B.line_no) - 1));
		}

	}
	else if (c == '>') {
		c = getcharM(&B, &index, list);
		if (c == '=') {
			return ret_tokenInfo(list, TK_GE, *(*(B.line_no) - 1));
		}
		else {
			ungetcM(&B, &index, list);
			return ret_tokenInfo(list, TK_GT, *(*(B.line_no) - 1));
		}
	}
	else if (c == '!') {
		c = getcharM(&B, &index, list);
		if (c == '=') {
			return ret_tokenInfo(list, TK_NE, *(*(B.line_no) - 1));
		}
	}
	else if (c == '@') {
		c = getcharM(&B, &index, list);
		if (c == '@') {
			c = getcharM(&B, &index, list);
			if (c == '@') {
				return ret_tokenInfo(list, TK_AND, *(*(B.line_no) - 1));
			}
		}
	}
	else if (c == '&') {
		c = getcharM(&B, &index, list);
		if (c == '&') {
			c = getcharM(&B, &index, list);
			if (c == '&') {
				return ret_tokenInfo(list, TK_OR, *(*(B.line_no) - 1));
			}
		}
	}
	else if (c == '~') {
		return ret_tokenInfo(list, TK_NOT, *(*(B.line_no) - 1));
	}
	else if (c == '+') {
		return ret_tokenInfo(list, TK_PLUS, *(*(B.line_no) - 1));
	}
	else if (c == '-') {
		return ret_tokenInfo(list, TK_MINUS, *(*(B.line_no) - 1));
	}
	else if (c == '*') {
		return ret_tokenInfo(list, TK_MUL, *(*(B.line_no) - 1));
	}
	else if (c == '/') {
		return ret_tokenInfo(list, TK_DIV, *(*(B.line_no) - 1));
	}
	else if (c == '%') {
		c = getcharM(&B, &index, list);
		while (c != '\n') {
			c = getcharM(&B, &index, list);
		}
		return ret_tokenInfo(list, TK_COMMENT, *(*(B.line_no) - 1));
	}
	else if (ifBet_b_to_d(c)) {
		c = getcharM(&B, &index, list);
		if (ifBet_2_to_7(c)) {
			c = getcharM(&B, &index, list);
			while (ifBet_b_to_d(c)) {
				c = getcharM(&B, &index, list);
			}
			if (ifBet_2_to_7(c)) {
				c = getcharM(&B, &index, list);
				while (ifBet_2_to_7(c)) {
					c = getcharM(&B, &index, list);
				}
				ungetcM(&B, &index, list);
				return ret_tokenInfo(list, TK_ID, *(*(B.line_no) - 1));
			}
			else {
				ungetcM(&B, &index, list);
				return ret_tokenInfo(list, TK_ID, *(*(B.line_no) - 1));
			}
		}
		else if (ifBet_a_to_z(c)) {
			c = getcharM(&B, &index, list);
			while (ifBet_a_to_z(c)) {
				c = getcharM(&B, &index, list);
			}
			ungetcM(&B, &index, list);
			char* lexeme = list_to_array(list->head, list->count);
			int token = lookup_for_match(lexeme);
			if (token != -1) {
				return ret_tokenInfo(list, token, *(*(B.line_no) - 1));
			}
			else {
				return ret_tokenInfo(list, TK_FIELDID, *(*(B.line_no) - 1));
			}
		}
		else {
			ungetcM(&B, &index, list);
			char* lexeme = list_to_array(list->head, list->count);
			int token = lookup_for_match(lexeme);
			if (token != -1) {
				return ret_tokenInfo(list, token, *(*(B.line_no) - 1));
			}
			else {
				return ret_tokenInfo(list, TK_FIELDID, *(*(B.line_no) - 1));
			}

		}
	}
	else if (ifBet_a_to_z_not_b_to_d(c)) {
		c = getcharM(&B, &index, list);
		while (ifBet_a_to_z(c)) {
			c = getcharM(&B, &index, list);
		}
		ungetcM(&B, &index, list);
		char* lexeme = list_to_array(list->head, list->count);
		int token = lookup_for_match(lexeme);
		if (token != -1) {
			return ret_tokenInfo(list, token, *(*(B.line_no) - 1));
		}
		else {
			return ret_tokenInfo(list, TK_FIELDID, *(*(B.line_no) - 1));
		}
	}
	else if (c == '_') {
		c = getcharM(&B, &index, list);
		if (ifBet_a_to_z_or_A_to_Z(c)) {
			c = getcharM(&B, &index, list);
			while (ifBet_a_to_z_or_A_to_Z(c)) {
				c = getcharM(&B, &index, list);
			}
			if (ifBet_0_to_9(c)) {
				c = getcharM(&B, &index, list);
				while (ifBet_0_to_9(c)) {
					c = getcharM(&B, &index, list);
				}
				ungetcM(&B, &index, list);
				return ret_tokenInfo(list, TK_FUNID, *(*(B.line_no) - 1));
			}
			else {
				ungetcM(&B, &index, list);

				char* lexeme = list_to_array(list->head, list->count);
				if (lookup_for_match(lexeme) == TK_MAIN) {
					return ret_tokenInfo(list, TK_MAIN, *(*(B.line_no) - 1));
				}
				else {
					return ret_tokenInfo(list, TK_FUNID, *(*(B.line_no) - 1));
				}
			}
		}
	}
	else if (c == '#') {
		c = getcharM(&B, &index, list);
		if (ifBet_a_to_z(c)) {
			c = getcharM(&B, &index, list);
			while (ifBet_a_to_z(c)) {
				c = getcharM(&B, &index, list);
			}
			ungetcM(&B, &index, list);
			return ret_tokenInfo(list, TK_RUID, *(*(B.line_no) - 1));
		}
	}
	else if (ifBet_0_to_9(c)) {
		c = getcharM(&B, &index, list);
		while (ifBet_0_to_9(c)) {
			c = getcharM(&B, &index, list);
		}
		if (c == '.') {
			c = getcharM(&B, &index, list);
			if (ifBet_0_to_9(c)) {
				c = getcharM(&B, &index, list);
				if (ifBet_0_to_9(c)) {
					c = getcharM(&B, &index, list);
					if (c == 'E') {
						c = getcharM(&B, &index, list);
						if (ifBet_0_to_9(c)) {
							c = getcharM(&B, &index, list);
							if (ifBet_0_to_9(c)) {
								return ret_tokenInfo(list, TK_RNUM, *(*(B.line_no) - 1));
							}
						}
						else if (c == '+' || c == '-') {
							c = getcharM(&B, &index, list);
							if (ifBet_0_to_9(c)) {
								c = getcharM(&B, &index, list);
								if (ifBet_0_to_9(c)) {
									return ret_tokenInfo(list, TK_RNUM, *(*(B.line_no) - 1));
								}
							}
						}
					}
					else {
						ungetcM(&B, &index, list);
						return ret_tokenInfo(list, TK_RNUM, *(*(B.line_no) - 1));
					}
				}
			}
		}
		else {
			ungetcM(&B, &index, list);
			return ret_tokenInfo(list, TK_NUM, *(*(B.line_no) - 1));
		}
	}
	else if (c == '[') {
		return ret_tokenInfo(list, TK_SQL, *(*(B.line_no) - 1));
	}
	else if (c == ']') {
		return ret_tokenInfo(list, TK_SQR, *(*(B.line_no) - 1));
	}
	else if (c == ',') {
		return ret_tokenInfo(list, TK_SQL, *(*(B.line_no) - 1));
	}
	else if (c == ';') {
		return ret_tokenInfo(list, TK_SQR, *(*(B.line_no) - 1));
	}
	else if (c == ':') {
		return ret_tokenInfo(list, TK_OP, *(*(B.line_no) - 1));
	}
	else if (c == '.') {
		return ret_tokenInfo(list, TK_CL, *(*(B.line_no) - 1));
	}
	else if (c == '(') {
		return ret_tokenInfo(list, TK_OP, *(*(B.line_no) - 1));
	}
	else if (c == ')') {
		return ret_tokenInfo(list, TK_CL, *(*(B.line_no) - 1));
	}
	else {
		tokenInfo temp;
		temp.name = LEX_ERROR;
		strcpy(temp.id, "Unrecognised token");
		return temp;
	}
	ungetcM(&B, &index, list);
	tokenInfo temp;
	temp.name = LEX_ERROR;
	strcpy(temp.id, "Unrecognised token");
	return temp;
}
void removeComments(char* testcaseFile, char** cleanFile) {
	int len = strlen(testcaseFile);
	*cleanFile = (char*)malloc(len * sizeof(char));
	int index_test_case = 0, index_clean_file = 0;
	char c;
	while (index_test_case < len) {
		c = testcaseFile[index_test_case];
		if (c != '%') {
			(*cleanFile)[index_clean_file] = c;
			index_test_case++;
			index_clean_file++;
			continue;
		}
		else {
			while (c != '\n' && c != EOF) {
				index_test_case++;
				c = testcaseFile[index_test_case];
			}
			//index_test_case--;
		}
	}
}

void tokenize_source_file(FILE* source) {
	tokenInfo tkn;

	printf("%-15s  |  %-20s  |  %-20s\n", "line_noBER", "LEXEME", "TOKEN_NAME");

	if (source == NULL) {
		printf("source null : print token stream\n");
	}

	while (1) {
		tkn = getNextToken(getStream(source));
		if (tkn.name == DOLLAR) {
			break;
		}
		else {
			if (tkn.name == LEX_ERROR) {
				printf("==========================================================\n");
				printf("%-15d  |  %-20s  |  %-20s\n", tkn.line_no, tkn.id,
					"LEXICAL ERROR");
				printf("==========================================================\n");
			}
			else {
					printf("%-15d  |  ", tkn.line_no);
					switch (tkn.name) {
					case TK_NUM:
						printf("%-20d  |  ", tkn.num);
						break;
					case TK_RNUM:
						printf("%-20f  |  ", tkn.rnum);
						break;
					default:
						printf("%-20s  |  ", tkn.id);
						break;
					}
					printf("%-20s\n", terminal_string[tkn.name]);
				
			}
		}
	} // end of while
}
