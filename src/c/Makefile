DFLAGS=-g -ldb
FLAGS=-fPIC -Wall
CC=gcc
CXX=g++
LFLAGS=-I../phase1-cpp/headers/libxml++-2.6.1 `pkg-config --cflags --libs glibmm-2.4` -Ofast
CXXFLAGS=/usr/lib/x86_64-linux-gnu/libxml++-2.6.so.2 $(LFLAGS)
buildlib: jbdblib.so libparser_api.so
	mv jbdblib.so ../jbdblib.so
	mv libparser_api.so ../libparser_api.so
	make clean

debug: lib.d.o set.d.o
	$(CC) $(DFLAGS) $^ -o $@

jbdblib.so: lib.o set.o libparser_api.so
	$(CC) $(FLAGS) $^ -shared -o $@ -ldb -L. -lparser_api -Wl,-rpath=.

libparser_api.so: rowparser.cpp parser_api.cpp
	$(CXX) $(FLAGS) $(CXXFLAGS) -shared $^ -o $@ -Wl,-rpath=/usr/lib/x86_64-linux-gnu/

%.o: %.c
	$(CC) $(FLAGS) -c $^

%.d.o: %.c
	$(CC) $(DFLAGS) -c $^

pclean:
	rm *.o -f

clean: pclean
	rm *.so -f
	rm test -f

runtest: test
	./test

test: set.o test_set.o test_api.o libparser_api.so
	$(CC) $(DFLAGS) test.c $^ $(CXXFLAGS) -o test -Wl,-rpath=.




.PHONY: clean pclean buildlib runtest
