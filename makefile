shell: shell.o lex.yy.o
	cc -o shell shell.o lex.yy.o
shell.o: shell.c
	cc -g -c shell.c
lex.yy.o: lex.yy.c
	cc -c lex.yy.c
