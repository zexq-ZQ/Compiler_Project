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
	char c = B->buffer[*index];
	(*index)++;
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

void ungetcM(int* index, struct LIST* list) {
	if (list->head == NULL) {
		printf("something wrong: not able to retract in empty lexeme\n");
		exit(0);
	}
	(*index)--;
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

tokenInfo ret_tokenInfo(struct LIST* list, TOKEN token) {
	int count = list->count;
	tokenInfo next_token;
	strcpy(next_token.id, list_to_array(list->head, count));
	next_token.name = token;
	return next_token;
}

char* getStream(FILE* fp) {}
tokenInfo getNextToken(twinBuffer B) {
	build_hashtable();
	int index = 0;
	struct LIST* list = (struct LIST*)malloc(sizeof(struct LIST));
	list->count = 0;
	list->head = NULL;
	list->tail = NULL;
	tokenInfo ret_token;
	char c = getcharM(&B, &index, list);
	while (c == '\n' || c == '\t' || c == ' ') {
		c = getcharM(&B, &index, list);
	}
	printf("point3, %c\n", c);
	if (c == '<') {
		c = getcharM(&B, &index, list);
		if (c == '-') {
			c = getcharM(&B, &index, list);
			if (c == '-') {
				c = getcharM(&B, &index, list);
				if (c == '-') {
					return ret_tokenInfo(list, TK_ASSIGNOP);
				}
			}
		}
		else if (c == '=') {
			return ret_tokenInfo(list, TK_LE);
		}
		else {
			ungetcM(&index, list);
			return ret_tokenInfo(list, TK_LT);
		}
	}
	else if (c == '=') {
		c = getcharM(&B, &index, list);
		if (c == '=') {
			return ret_tokenInfo(list, TK_EQ);
		}
	}
	else if (c == '>') {
		c = getcharM(&B, &index, list);
		if (c == '=') {
			return ret_tokenInfo(list, TK_GE);
		}
		else {
			ungetcM(&index, list);
			return ret_tokenInfo(list, TK_GT);
		}
	}
	else if (c == '!') {
		c = getcharM(&B, &index, list);
		if (c == '=') {
			return ret_tokenInfo(list, TK_NE);
		}
	}
	else if (c == '@') {
		c = getcharM(&B, &index, list);
		if (c == '@') {
			c = getcharM(&B, &index, list);
			if (c == '@') {
				return ret_tokenInfo(list, TK_AND);
			}
		}
	}
	else if (c == '&') {
		c = getcharM(&B, &index, list);
		if (c == '&') {
			c = getcharM(&B, &index, list);
			if (c == '&') {
				return ret_tokenInfo(list, TK_OR);
			}
		}
	}
	else if (c == '~') {
		return ret_tokenInfo(list, TK_NOT);
	}
	else if (c == '+') {
		return ret_tokenInfo(list, TK_PLUS);
	}
	else if (c == '-') {
		return ret_tokenInfo(list, TK_MINUS);
	}
	else if (c == '*') {
		return ret_tokenInfo(list, TK_MUL);
	}
	else if (c == '/') {
		return ret_tokenInfo(list, TK_DIV);
	}
	else if (c == '%') {
		c = getcharM(&B, &index, list);
		while (c != '\n') {
			c = getcharM(&B, &index, list);
		}
		return ret_tokenInfo(list, TK_COMMENT);
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
				ungetcM(&index, list);
				return ret_tokenInfo(list, TK_ID);
			}
			else {
				ungetcM(&index, list);
				return ret_tokenInfo(list, TK_ID);
			}
		}
		else if (ifBet_a_to_z(c)) {
			c = getcharM(&B, &index, list);
			while (ifBet_a_to_z(c)) {
				c = getcharM(&B, &index, list);
			}
			ungetcM(&index, list);
			char* lexeme = list_to_array(list->head, list->count);
			int token = lookup_for_match(lexeme);
			if (token != -1) {
				return ret_tokenInfo(list, token);
			}
			else {
				return ret_tokenInfo(list, TK_FIELDID);
			}
		}
		else {
			ungetcM(&index, list);
			char* lexeme = list_to_array(list->head, list->count);
			int token = lookup_for_match(lexeme);
			if (token != -1) {
				return ret_tokenInfo(list, token);
			}
			else {
				return ret_tokenInfo(list, TK_FIELDID);
			}

		}
	}
	else if (ifBet_a_to_z_not_b_to_d(c)) {
		c = getcharM(&B, &index, list);
		printf("point1, %c\n", c);
		while (ifBet_a_to_z(c)) {
			c = getcharM(&B, &index, list);
			printf("point2, %c\n", c);
		}
		ungetcM(&index, list);
		char* lexeme = list_to_array(list->head, list->count);
		int token = lookup_for_match(lexeme);
		if (token != -1) {
			return ret_tokenInfo(list, token);
		}
		else {
			return ret_tokenInfo(list, TK_FIELDID);
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
				ungetcM(&index, list);
				return ret_tokenInfo(list, TK_FUNID);
			}
			else {
				ungetcM(&index, list);

				char* lexeme = list_to_array(list->head, list->count);
				if (lookup_for_match(lexeme) == TK_MAIN) {
					return ret_tokenInfo(list, TK_MAIN);
				}
				else {
					return ret_tokenInfo(list, TK_FUNID);
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
			ungetcM(&index, list);
			return ret_tokenInfo(list, TK_RUID);
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
								return ret_tokenInfo(list, TK_RNUM);
							}
						}
						else if (c == '+' || c == '-') {
							c = getcharM(&B, &index, list);
							if (ifBet_0_to_9(c)) {
								c = getcharM(&B, &index, list);
								if (ifBet_0_to_9(c)) {
									return ret_tokenInfo(list, TK_RNUM);
								}
							}
						}
					}
					else {
						return ret_tokenInfo(list, TK_RNUM);
					}
				}
			}
		}
		else {
			ungetcM(&index, list);
			return ret_tokenInfo(list, TK_NUM);
		}
	}
	else if (c == '[') {
		return ret_tokenInfo(list, TK_SQL);
	}
	else if (c == ']') {
		return ret_tokenInfo(list, TK_SQR);
	}
	else if (c == ',') {
		return ret_tokenInfo(list, TK_SQL);
	}
	else if (c == ';') {
		return ret_tokenInfo(list, TK_SQR);
	}
	else if (c == ':') {
		return ret_tokenInfo(list, TK_OP);
	}
	else if (c == '.') {
		return ret_tokenInfo(list, TK_CL);
	}
	else if (c == '(') {
		return ret_tokenInfo(list, TK_OP);
	}
	else if (c == ')') {
		return ret_tokenInfo(list, TK_CL);
	}
}
void removeComments(char* testcaseFile, char* cleanFile) {
	int len = strlen(testcaseFile);
	cleanFile = (char*)malloc(len * sizeof(char));
	int index_test_case = 0, index_clean_file = 0;
	char c;
	while (index_test_case < len) {
		c = testcaseFile[index_test_case];
		if (c != '%') {
			cleanFile[index_clean_file] = c;
			index_test_case++;
			index_clean_file++;
			continue;
		}
		else {
			while (c != '\n') {
				index_test_case++;
			}
			index_test_case--;
		}
	}
}



