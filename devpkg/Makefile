PREFIX?=/usr/local
CFLAGS=-g -Wall -I${PREFIX}/apr/include/apr-1
CFLAGS+=-I${PREFIX}/apr/include/apr-util-1
LDFLAGS=-L${PREFIX}/apr/lib/ -lapr-1 -pthread -laprutil-1

all: devpkg

<<<<<<< HEAD
devpkg: devpkg.c bstrlib.o db.o shell.o commands.o
	cc ${CFLAGS} $? ${LDFLAGS} -o $@
=======
devpkg: bstrlib.o db.o shell.o commands.o
>>>>>>> 5ec1972f98a262f2931bdad2d5bfea80a59fdb97

install: all
	install -d $(DESTDIR)/$(PREFIX)/bin/
	install devpkg $(DESTDIR)/$(PREFIX)/bin/

clean:
	rm -f *.o
	rm -f devpkg
	rm -rf *.dSYM

