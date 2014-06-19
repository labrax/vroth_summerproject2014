CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
OBJ_DIR := obj/
LD_FLAGS := -Wall -g -std=c++11
CC_FLAGS := -Wall -g -std=c++11

all: apriori

apriori: $(OBJ_FILES)
	g++ $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.cpp $(OBJ_DIR)
	g++ $(CC_FLAGS) -c -o $@ $<
	
$(OBJ_DIR):
	mkdir $@

clean:
	-rm -rf obj/
	-rm -rf apriori
