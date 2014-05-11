all : test1 test2 test3 test4

test1 : test1.c
	gcc -o test1 test1.c slug_mem.c

test2 : test2.c
	gcc -o test2 test2.c slug_mem.c

test3 : test3.c
	gcc -o test3 test3.c slug_mem.c

test4 : test4.c
	gcc -o test4 test4.c slug_mem.c

clean :
	rm test[0-9]
