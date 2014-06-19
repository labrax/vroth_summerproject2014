
LD_FLAGS := -Wall -g -std=c++11
CC_FLAGS := -Wall -g -std=c++11

all: prepare_spmf

prepare_spmf: main.o file_transactions.o
	g++ $(LD_FLAGS) -o prepare_spmf main.o file_transactions.o

main.o: src/main.cpp
	g++ $(CC_FLAGS) -c -o main.o src/main.cpp
	
file_transactions.o: src/file_transactions.cpp
	g++ $(CC_FLAGS) -c -o file_transactions.o src/file_transactions.cpp
	
clean:
	-rm -rf *.o
	-rm -rf prepare_spmf
