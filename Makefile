# Makefile
CXX=g++
CXX_FLAGS=-O3 -Wall

test: simhash.o
	${CXX} ${CXX_FLAGS} -lgtest -lpthread ./tests/test_main.cc simhash.o -o test

simhash: simhash.o
	${CXX} ${CXX_FLAGS} ./src/main.cc simhash.o -o simhash

simhash.o:
	${CXX} ${CXX_FLAGS}  -c ./src/simhash.cc

clean:
	rm -f simhash.o