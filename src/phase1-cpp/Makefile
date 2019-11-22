CXX=g++
LFLAGS=-Iheaders/libxml++-2.6.1 `pkg-config --cflags --libs glibmm-2.4` -Ofast
FLAGS=/usr/lib/x86_64-linux-gnu/libxml++-2.6.so.2 $(LFLAGS)
xmlparserfast: src/main.cpp bin/parser.o bin/mail.o
	$(CXX) $^ -o $@ $(FLAGS)

bin:
	mkdir bin -p

bin/%.o: src/%.cpp src/%.hpp bin
	$(CXX) $< -c -o $@ $(LFLAGS)

clean:
	rm bin -rf
	rm xmlparserfast -f
	rm terms.txt
	rm emails.txt
	rm dates.txt
	rm recs.txt

test-1k: xmlparserfast
	cat ../../testing/1k.xml | ./xmlparserfast
	diff ../../testing/1k-terms.txt terms.txt
	diff ../../testing/1k-dates.txt dates.txt
	diff ../../testing/1k-emails.txt emails.txt
	diff ../../testing/1k-recs.txt recs.txt
	make clean

test-time: xmlparserfast
	bash -c "time cat ../../testing/100k.xml | ./xmlparserfast"
	make clean


.PHONY: clean test-1k test-time

