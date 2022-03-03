run_lexer:
	gcc -c lookup_table.c
	gcc lexer.c lookup_table.o -o lexer
	./lexer
