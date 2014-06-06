.PHONY: app

CXXFLAGS := -Iinclude -Wall -pedantic -std=c++11 -llindacomm

app: test.out

test.out: main.o classA.o classB.o
	g++ $^ -o $@
	
%.o: src/%.cpp
	g++ -c -o $@ $? $(CXXFLAGS)

test:
	./test.out

clear:
	rm *.o *.out
