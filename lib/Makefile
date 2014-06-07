.PHONY: all

CXXFLAGS := -Iinclude -Wall -pedantic -std=c++11

all: lib daemon
	
daemon: main.o daemon.o defragmenter.o
	g++ $^ -o daemon.out -lboost_thread

lib: lindaComm.o tuple.o tupleSerializer.o tupleTemplate.o
	ar rcs liblindacomm.a $^
	
%.o: src/%.cpp
	g++ -c -o $@ $? $(CXXFLAGS)	

install: lib daemon move-lib move-daemon

move-lib:
	cp ./include/lindaComm.h /usr/include/lindacomm.h; \
	cp ./include/tuple.h /usr/include/; \
	cp ./include/tupleSerializer.h /usr/include/; \
	cp ./include/tupleTemplate.h /usr/include/; \
	cp ./include/parameter.h /usr/include/; \
	cp liblindacomm.a /usr/lib;
	
move-daemon:
	cp daemon.out /usr/bin/lindacommd	

uninstall:
	cd /usr/include; rm lindacomm.h tuple.h tupleTemplate.h tupleSerializer.h parameter.h /usr/lib/liblindacomm.a /usr/bin/lindacommd

reinstall: uninstall clean install

remake: clean all

clean:
	rm *.o *.out *.a