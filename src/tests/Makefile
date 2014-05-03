# CREATED 1 May 2014
#  Team Brilliant Squid
#

CC = cc
objects = 

all: $(objects)
	$(CC) -o IO_bound IO_bound.c -lm
	$(CC) -o cpu_bound cpu_bound.c -lm
	dd if=/dev/urandom of=IO_bound.in count=50000


clean: 
	rm IO_bound
	rm cpu_bound
	rm *~
