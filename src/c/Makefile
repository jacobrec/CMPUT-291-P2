buildlib: jbdblib.so
	mv jbdblib.so ../jbdblib.so
	make clean

jbdblib.so: lib.o
	gcc -ldb $< -shared -o $@

%.o: %.c
	gcc -c $<

pclean:
	rm *.o -f

clean: pclean
	rm *.so -f

.PHONY: clean pclean buildlib
