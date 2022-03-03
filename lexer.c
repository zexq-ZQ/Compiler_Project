#include<stdio.h>
#include<ctype.h>
#include"lookup_table.h"

struct tokenInfo{
	enum TOKEN token;
	char* lexeme;
	int line_num;
};

struct twinBuffer {
	char** buffer;
	int **line_num;
};

struct NODE{
	char value;
	struct NODE *next;
	struct NODE *prev;
};

struct LIST{
	int count;
	struct NODE *head;
	struct NODE *tail;
};
/*
TOKEN getRelop()
{
TOKEN retToken = new(RELOP);
while(1) {  //repeat character processing until a return or failure occurs 

    switch(state) {
        case 0: c = nextChar();
        if ( c == '<' ) state = 1;
        else if ( c == '=' ) state = 5;
        else if ( c == '>' ) state = 6;
        else fail();  lexeme is not a relop 
        break;
        case 1: ...
        ...
        case 8: retract();
        retToken.attribute = GT;
        return(retToken);

        }
    }
}
*/
int ifBet_a_to_z_not_b_to_d(char c){
    if (c >= 'a' && c <= 'z' && !(c>='b' && c <= 'd') ){
        return 1;
    }
    return 0;
}

int ifBet_a_to_z_or_A_to_Z(char c){
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
        return 1;
    }
    return 0;
}

int ifBet_a_to_z(char c){
    if (c >= 'a' && c <= 'z'){
        return 1;
    }
    return 0;
}

int ifBet_b_to_d(char c){
    if (c >= 'b' && c <= 'd'){
        return 1;
    }
    return 0;
}

int ifBet_0_to_9(char c){
    int temp = c - '0';
    if(temp >= 0 && temp <= 9){
        return 1;
    }
    return 0;
}

int ifBet_2_to_7(char c){
    int temp = c - '0';
    if(temp >= 2 && temp <= 7){
        return 1;
    }
    return 0;
}

char getcharM(struct twinBuffer *B, int *index, struct LIST *list){
	char c = *(*(B->buffer));
	(*(B->buffer))++;
	(*(B->line_num))++;
	struct NODE *new_node = (struct NODE*)malloc(sizeof(struct NODE));
	new_node->value = c;
	new_node->next = NULL;
	if(list->head == NULL){
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

void ungetcM(struct twinBuffer *B, int *index, struct LIST *list){
	if(list->head == NULL){
		printf("something wrong: not able to retract in empty lexeme\n");
		exit(0);
	}
	(*(B->buffer))--;
	(*(B->line_num))--;
	if(list->head == list->tail){
		free(list->head);
		list->head = NULL;
		list->tail = NULL;
		list->count = 0;
		return;
	}
	struct NODE *temp = list->tail;
	list->tail = temp->prev;
	list->tail->next = NULL;
	list->count--;
	free(temp);
}

char* list_to_array(struct NODE *head, int count){
	char *lexeme = (char*)malloc((count+1)*sizeof(char));
       	struct NODE *temp = head;
	int index = 0;
	while(temp != NULL){
		lexeme[index] = temp->value;
		temp = temp->next;
		index++;
	}	
	lexeme[index] = '\0';
	return lexeme;
}

struct tokenInfo ret_tokenInfo(struct LIST *list, enum TOKEN token, int line_num){
	int count = list->count;
	struct tokenInfo next_token;
	next_token.lexeme = list_to_array(list->head, count);
	next_token.token = token;
	next_token.line_num = line_num;
	return next_token;
}
void removeComments(char *testcaseFile, char **cleanFile);
struct tokenInfo getNextToken(struct twinBuffer B);
struct twinBuffer getStream(FILE *fp){
	char c;
	int count = 0;
	while(!feof(fp)){
		c = fgetc(fp);
		count++;	
	}
	int index = 0;
	struct twinBuffer result;
	char *arr1 = (char*)malloc(count*sizeof(char));
	int *arr2 = (int*)malloc(count*sizeof(int));
	result.buffer = &arr1;
	result.line_num= &arr2;
	rewind(fp);
	int line_num = 1;
	while(!feof(fp)){
		c = fgetc(fp);
		if(c == '\n'){
			line_num++;
		}
		(*(result.buffer))[index] = c;
		(*(result.line_num))[index] = line_num;
		index++;
	}
	(*(result.buffer))[index-1] = EOF;
	(*(result.line_num))[index-1] = line_num;

while(*(*result.buffer) != EOF){
		struct tokenInfo info_of_token;
		info_of_token = getNextToken(result);
		enum TOKEN token = info_of_token.token;
		char *lexeme = info_of_token.lexeme;
		printf("line no: %d, lexeme: %s, token: %d\n",info_of_token.line_num,lexeme,token);
	}
	return result;

}
struct tokenInfo getNextToken(struct twinBuffer B){
	build_hashtable();
	int index = 0;
	struct LIST *list = (struct LIST*)malloc(sizeof(struct LIST));
	list->count = 0;
	list->head = NULL;
	list->tail = NULL;
	struct tokenInfo ret_token;
	char c = *(*(B.buffer));
	if(c <= 32){
		while(c <= 32){
			if(c == EOF){
				struct tokenInfo temp;
				temp.token = 0;
				char arr3[] = "eof"; 
				temp.lexeme = arr3;
				return temp;
			}
			(*(B.buffer))++;
			(*(B.line_num))++;
			c = *(*(B.buffer));
		}
	}
	c = getcharM(&B, &index, list);
	    if(c == '<'){
		c = getcharM(&B, &index, list);
		if(c == '-'){
		    c = getcharM(&B, &index, list);
		    if(c == '-'){
		        c = getcharM(&B, &index, list);
		        if(c == '-'){
		            return ret_tokenInfo(list,TK_ASSIGNOP,*(*(B.line_num)-1));
		        }
		    }
		}
		else if(c == '='){
		    return ret_tokenInfo(list,TK_LE,*(*(B.line_num)-1));
		}
		else{
		    ungetcM(&B, &index, list);
		    return ret_tokenInfo(list,TK_LT,*(*(B.line_num)-1));
		}
	    }
	    else if(c == '='){
		c = getcharM(&B, &index, list);
		if(c == '='){
		    return ret_tokenInfo(list,TK_EQ,*(*(B.line_num)-1));
		}
		
	    }
	    else if(c == '>'){
	    	c = getcharM(&B, &index, list);
	    	if(c == '='){
			return ret_tokenInfo(list,TK_GE,*(*(B.line_num)-1));
		}
		else{
			ungetcM(&B, &index, list);
			return ret_tokenInfo(list,TK_GT,*(*(B.line_num)-1));
		}	
	    }
	    else if(c == '!'){
		c = getcharM(&B, &index, list);
		if(c == '='){
			return ret_tokenInfo(list,TK_NE,*(*(B.line_num)-1));
		}
	    }
	    else if(c == '@'){
		c = getcharM(&B, &index, list);
		if(c == '@'){
			c = getcharM(&B, &index, list);
			if(c == '@'){
				return ret_tokenInfo(list,TK_AND,*(*(B.line_num)-1));
			}
		}
	    }
	    else if(c == '&'){
		c = getcharM(&B, &index, list);
		if(c == '&'){
			c = getcharM(&B, &index, list);
			if(c == '&'){
				return ret_tokenInfo(list,TK_OR,*(*(B.line_num)-1));
			}
		}
	    }
	    else if(c == '~'){
		return ret_tokenInfo(list,TK_NOT,*(*(B.line_num)-1));
	    }
	    else if(c == '+'){
		return ret_tokenInfo(list,TK_PLUS,*(*(B.line_num)-1));
	    }
	    else if(c == '-'){
		return ret_tokenInfo(list,TK_MINUS,*(*(B.line_num)-1));
	    }
	    else if(c == '*'){
		return ret_tokenInfo(list,TK_MUL,*(*(B.line_num)-1));
	    }
	    else if(c == '/'){
		return ret_tokenInfo(list,TK_DIV,*(*(B.line_num)-1));
	    }
	    else if(c == '%'){
		c = getcharM(&B, &index, list);
		while(c != '\n'){
			c = getcharM(&B, &index, list);
		}
		return ret_tokenInfo(list,TK_COMMENT,*(*(B.line_num)-1));
	    }
	    else if(ifBet_b_to_d(c)){
	    	c = getcharM(&B, &index, list);
		if(ifBet_2_to_7(c)){
			c = getcharM(&B, &index, list);
			while(ifBet_b_to_d(c)){
				c = getcharM(&B, &index, list);
			}
			if(ifBet_2_to_7(c)){
				c = getcharM(&B, &index, list);
				while(ifBet_2_to_7(c)){
					c = getcharM(&B, &index, list);
				}
				ungetcM(&B, &index, list);
				return ret_tokenInfo(list,TK_ID,*(*(B.line_num)-1));
			}
			else{
				ungetcM(&B, &index, list);
				return ret_tokenInfo(list,TK_ID,*(*(B.line_num)-1));
			}
		}
		else if(ifBet_a_to_z(c)){
			c = getcharM(&B, &index, list);
			while(ifBet_a_to_z(c)){
				c = getcharM(&B, &index, list);
			}
			ungetcM(&B, &index, list);
			char *lexeme = list_to_array(list->head, list->count);
			int token = lookup_for_match(lexeme);
			if(token != -1){
				return ret_tokenInfo(list,token,*(*(B.line_num)-1));
			}
			else{
				return ret_tokenInfo(list,TK_FIELDID,*(*(B.line_num)-1));
			}
		}
		else{
			ungetcM(&B, &index, list);
			char *lexeme = list_to_array(list->head, list->count);
			int token = lookup_for_match(lexeme);
			if(token != -1){
				return ret_tokenInfo(list,token,*(*(B.line_num)-1));
			}
			else{
				return ret_tokenInfo(list,TK_FIELDID,*(*(B.line_num)-1));
			}

		}
	    }
	    else if(ifBet_a_to_z_not_b_to_d(c)){
		c = getcharM(&B, &index, list);
		while(ifBet_a_to_z(c)){
			c = getcharM(&B, &index, list);
		}
		ungetcM(&B, &index, list);
		char *lexeme = list_to_array(list->head, list->count);
		int token = lookup_for_match(lexeme);
		if(token != -1){
			return ret_tokenInfo(list,token,*(*(B.line_num)-1));
		}
		else{
			return ret_tokenInfo(list,TK_FIELDID,*(*(B.line_num)-1));
		}
	    }
	    else if(c == '_'){
		c = getcharM(&B, &index, list);
		if(ifBet_a_to_z_or_A_to_Z(c)){
			c = getcharM(&B, &index, list);
			while(ifBet_a_to_z_or_A_to_Z(c)){
				c = getcharM(&B, &index, list);
			}
			if(ifBet_0_to_9(c)){
				c = getcharM(&B, &index, list);
				while(ifBet_0_to_9(c)){
					c = getcharM(&B, &index, list);
				}
				ungetcM(&B, &index, list);
				return ret_tokenInfo(list,TK_FUNID,*(*(B.line_num)-1));
			}
			else{
				ungetcM(&B, &index, list);
				
				char *lexeme = list_to_array(list->head, list->count);
				if(lookup_for_match(lexeme) == TK_MAIN){
					return ret_tokenInfo(list,TK_MAIN,*(*(B.line_num)-1));
				}
				else{
					return ret_tokenInfo(list,TK_FUNID,*(*(B.line_num)-1));
				}
			}
		}
	    }
	    else if(c == '#'){
		c = getcharM(&B, &index, list);
		if(ifBet_a_to_z(c)){
			c = getcharM(&B, &index, list);
			while(ifBet_a_to_z(c)){
				c = getcharM(&B, &index, list);
			}
			ungetcM(&B, &index, list);
			return ret_tokenInfo(list,TK_RUID,*(*(B.line_num)-1));
		}
	    }
	    else if(ifBet_0_to_9(c)){
		c = getcharM(&B, &index, list);
		while(ifBet_0_to_9(c)){
			c = getcharM(&B, &index, list);
		}
		if(c == '.'){
			c = getcharM(&B, &index, list);
			if(ifBet_0_to_9(c)){
				c = getcharM(&B, &index, list);
				if(ifBet_0_to_9(c)){
					c = getcharM(&B, &index, list);
					if(c == 'E'){
						c = getcharM(&B, &index, list);
						if(ifBet_0_to_9(c)){
							c = getcharM(&B, &index, list);
							if(ifBet_0_to_9(c)){
								return ret_tokenInfo(list,TK_RNUM,*(*(B.line_num)-1));
							}
						}
						else if(c == '+' || c == '-'){
							c = getcharM(&B, &index, list);
							if(ifBet_0_to_9(c)){
								c = getcharM(&B, &index, list);
								if(ifBet_0_to_9(c)){
									return ret_tokenInfo(list,TK_RNUM,*(*(B.line_num)-1));
								}								
							}
						}
					}
					else{
						ungetcM(&B, &index, list);
						return ret_tokenInfo(list,TK_RNUM,*(*(B.line_num)-1));
					}
				}
			}
		}
		else{
			ungetcM(&B, &index, list);
			return ret_tokenInfo(list,TK_NUM,*(*(B.line_num)-1));	
		}
	    }
	    else if(c == '['){
		return ret_tokenInfo(list,TK_SQL,*(*(B.line_num)-1));
	    }
	    else if(c == ']'){
		return ret_tokenInfo(list,TK_SQR,*(*(B.line_num)-1));
	    }
	    else if(c == ','){
		return ret_tokenInfo(list,TK_SQL,*(*(B.line_num)-1));
	    }
	    else if(c == ';'){
		return ret_tokenInfo(list,TK_SQR,*(*(B.line_num)-1));
	    }
	    else if(c == ':'){
		return ret_tokenInfo(list,TK_OP,*(*(B.line_num)-1));
	    }
	    else if(c == '.'){
		return ret_tokenInfo(list,TK_CL,*(*(B.line_num)-1));
	    }
	    else if(c == '('){
		return ret_tokenInfo(list,TK_OP,*(*(B.line_num)-1));
	    }
	    else if(c == ')'){
		return ret_tokenInfo(list,TK_CL,*(*(B.line_num)-1));
	    }
	    else{
	    	struct tokenInfo temp;
		temp.token = 0;
		char arr[] = "unknown error";
		temp.lexeme = arr;
		temp.line_num = *(*(B.line_num)-1);
		return temp;
	    }
	    ungetcM(&B, &index, list);
	    struct tokenInfo temp;
		temp.token = 0;
		char arr[] = "unknown error";
		temp.lexeme = arr;
		temp.line_num = *(*(B.line_num)-1);
		return temp;
}
void removeComments(char *testcaseFile, char **cleanFile){
	int len = strlen(testcaseFile);
	*cleanFile = (char*)malloc(len*sizeof(char));
	int index_test_case = 0, index_clean_file = 0;
	char c; 
	while(index_test_case < len){
		c = testcaseFile[index_test_case];
		if(c != '%'){
			(*cleanFile)[index_clean_file] = c;	
			index_test_case++;
			index_clean_file++;
			continue;
		}
		else{
			while(c != '\n' && c != EOF){
				index_test_case++;
				c = testcaseFile[index_test_case];
			}
			//index_test_case--;
		}
	}
}

int main(){
	FILE *fp = fopen("testcases_stage1/t3.txt","r");
	struct twinBuffer inpStr = getStream(fp);
	fclose(fp);
	printf("%ld\n",strlen(*(inpStr.buffer)));
	printf("%s\n",*(inpStr.buffer));
	//char *clean_str;
	//removeComments(str, &clean_str);
	//printf("comment removed\n");
	//printf("%s\n",clean_str);
	
	//char *temp = clean_str;
	/*for(int i = 0; i<136; i++){
		//printf("%s\n",temp);
		//buffer.buffer = &temp;	
		struct tokenInfo info_of_token;
		info_of_token = getNextToken(inpStr);
		enum TOKEN token = info_of_token.token;
		char *lexeme = info_of_token.lexeme;
		printf("line no: %d, lexeme: %s, token: %d\n",info_of_token.line_num,lexeme,token);
	}*/

		//buffer.buffer = str;
		//printf("%d\n",getNextToken(buffer).token);
}
