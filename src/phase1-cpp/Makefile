CXX=g++
FLAGS=`pkg-config --cflags --libs libxml++-3.0`
xmlparserfast: src/main.cpp bin/parser.o bin/mail.o
	$(CXX) $^ -o $@ $(FLAGS) -lxml++-3.0

bin:
	mkdir bin -p

bin/%.o: src/%.cpp src/%.hpp bin
	$(CXX) $(FLAGS) $< -c -o $@

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

