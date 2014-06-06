.PHONY: all

CXXFLAGS := -Iinclude -Wall -pedantic -W -std=c++11

all: testApp lib

testApp:
	cd test; make

lib: 
	cd lib; make all install
