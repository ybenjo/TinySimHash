# Makefile
CXX=g++
CXX_FLAGS=-O3 -Wall
TT=-I/opt/local/include -L/opt/local/lib -ltokyotyrant -ltokyocabinet -lz -lbz2 -lresolv -ldl -lpthread -lm -lc

# ソースからインストールした場合、TTをこれに書き換える
# TT=-I/usr/local/include -L/usr/local/lib -ltokyotyrant -ltokyocabinet -lz -lbz2 -lresolv -lnsl -ldl -lrt -lpthread -lm -lc

# -lnsl -lrt

test: simhash.o
	${CXX} ${CXX_FLAGS} -lgtest -lpthread ./tests/test_main.cc simhash.o -o test

simhash: simhash.o
	${CXX} ${CXX_FLAGS} ${TT} ./src/main.cc simhash.o -o simhash

simhash.o:
	${CXX} ${CXX_FLAGS} -c ./src/simhash.cc

clean:
	rm -f simhash.o