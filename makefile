shell: shell.o lex.yy.o
	cc -Wall -o shell shell.o lex.yy.o
shell.o: shell.c
	cc -Wall -g -c shell.c
lex.yy.o: lex.yy.c
	cc -c lex.yy.c
