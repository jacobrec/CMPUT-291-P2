buildlib: jbdblib.so
	mv jbdblib.so ../jbdblib.so
	make clean

debug: lib.o set.o
	gcc -g -ldb $^ -o $@

jbdblib.so: lib.o set.o
	gcc -ldb $^ -shared -o $@

%.o: %.c
	gcc -ldb -g -c $^

pclean:
	rm *.o -f

clean: pclean
	rm *.so -f
	rm test -f

runtest: test
	./test

test: set.o test_set.o
	gcc test.c $^ -o test




.PHONY: clean pclean buildlib runtest
