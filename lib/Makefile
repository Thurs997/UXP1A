.PHONY: all

CXXFLAGS := -Iinclude -Wall -pedantic -std=c++11

all: lib daemon

lib: lindacomm.a

daemon.out: daemon.o 

lindacomm.a: 
	ar rcs lindacomm.a lindacomm.out

lindacomm.out: tuple.o
	
%.o: src/%.cpp
	g++ -c -o $@ $? $(CXXFLAGS)
	

install:
	cp ./include/lindaComm.h /usr/include/lindacomm.h; cp daemon.out /usr/bin/lindacommd

uninstall:
	rm /usr/lib/lindaComm.h /usr/lib/lindaComm

reinstall: uninstall clean all install

remake: clean all

clean:
	rm *.o *.out *.a