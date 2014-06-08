.PHONY: app

CXXFLAGS := -Iinclude -Wall -pedantic -std=c++11 -llindacomm

app: test.out

test.out: main.o
	g++ $^ -o $@ -llindacomm
	
%.o: src/%.cpp
	g++ -c -o $@ $? $(CXXFLAGS)

test:
	./test.out 

memory-leak-check:
	valgrind --leak-check=full ./test.out > out.txt 2>&1	

clean:
	rm *.o *.out out.txt
