SOURCES=metatag.c metacat.c

all: $(SOURCES) metatag metacat

metatag:
	cc -o metatag metatag.c
metacat:
	cc -o metacat metacat.c

spotless: clean
	- rm $(EXECS)

clean:
