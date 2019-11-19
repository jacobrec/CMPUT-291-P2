buildlib: jbdblib.so
	mv jbdblib.so ../jbdblib.so
	make clean

test: lib.o
	gcc -g -ldb $< -o $@

jbdblib.so: lib.o
	gcc -ldb $< -shared -o $@

%.o: %.c
	gcc -ldb -g -c $<

pclean:
	rm *.o -f

clean: pclean
	rm *.so -f
	rm test -f

.PHONY: clean pclean buildlib
