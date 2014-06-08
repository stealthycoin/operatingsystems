SOURCES=metatag.c metacat.c kilofiles.c

all: $(SOURCES) metatag metacat kilofiles

metatag:
	cc -o metatag metatag.c

metacat:
	cc -o metacat metacat.c

kilofiles:
	cc -o kilofiles kilofiles.c

spotless: clean
	- rm $(EXECS)

clean:
