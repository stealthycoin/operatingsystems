all : mem test1 test2 test3 test4 test5 test6 test7 test8

mem: slug_mem.c slug_mem.h
	cc -c slug_mem.c

test1 : test1.c slug_mem.c slug_mem.h
	cc -o test1 test1.c slug_mem.o -lm

test2 : test2.c slug_mem.c slug_mem.h
	cc -o test2 test2.c slug_mem.o -lm

test3 : test3.c slug_mem.c slug_mem.h
	cc -o test3 test3.c slug_mem.o -lm

test4 : test4.c slug_mem.c slug_mem.h
	cc -o test4 test4.c slug_mem.o -lm

test5 : test5.c slug_mem.c slug_mem.h
	cc -o test5 test5.c slug_mem.o -lm

test6: test6.c slug_mem.c slug_mem.h
	cc -o test6 test6.c slug_mem.o -lm

test7: test7.c slug_mem.c slug_mem.h
	cc -o test7 test7.c slug_mem.o -lm

test8: test8.c slug_mem.c slug_mem.h
	cc -o test8 test8.c slug_mem.o -lm

clean:
	-rm test[1-9]
	-rm result[1-9]

test:
	-./test1 ?> result1
	-./test2 ?> result2
	-./test3 ?> result3
	-./test4 ?> result4
	-./test5 ?> result5
	-./test6 ?> result6
	-./test7 ?> result7
	-./test8 ?> result8
