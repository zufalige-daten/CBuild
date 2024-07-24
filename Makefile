CFLAGS=-I include -c -o
LFLAGS=-o

all: bin/obj/main.o bin/obj/lexer.o bin/obj/compandlink.o bin/obj/msafe.o bin/obj/parseapp.o bin/obj/srcstruct.o
	g++ bin/obj/*.o $(LFLAGS) bin/cbuild
bin/obj/main.o: src/main.cc
	g++ src/main.cc $(CFLAGS) bin/obj/main.o
bin/obj/lexer.o: src/lexer.cc
	g++ src/lexer.cc $(CFLAGS) bin/obj/lexer.o
bin/obj/compandlink.o: src/compandlink.cc
	g++ src/compandlink.cc $(CFLAGS) bin/obj/compandlink.o
bin/obj/msafe.o: src/msafe.cc
	g++ src/msafe.cc $(CFLAGS) bin/obj/msafe.o
bin/obj/parseapp.o: src/parseapp.cc
	g++ src/parseapp.cc $(CFLAGS) bin/obj/parseapp.o
bin/obj/srcstruct.o: src/srcstruct.cc
	g++ src/srcstruct.cc $(CFLAGS) bin/obj/srcstruct.o

in-cbuild:
	cbuild

install:
	cp bin/cbuild /usr/local/bin/cbuild

