DFLAGS=-g -ldb
FLAGS=-ldb -fPIC
CC=gcc
buildlib: jbdblib.so
	mv jbdblib.so ../jbdblib.so
	make clean

debug: lib.d.o set.d.o
	$(CC) $(DFLAGS) $^ -o $@

jbdblib.so: lib.o set.o
	$(CC) $(FLAGS) $^ -shared -o $@

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

test: set.o test_set.o
	$(CC) $(DFLAGS) test.c $^ -o test




.PHONY: clean pclean buildlib runtest
