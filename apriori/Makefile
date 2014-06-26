CPP_FILES := $(wildcard src/*.cpp)
OBJ_FILES := $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))

CANDIDATE_CPP := $(wildcard src/candidate/*.cpp)
CANDIDATE_OBJ := $(addprefix obj/candidate/,$(notdir $(CANDIDATE_CPP:.cpp=.o)))

OBJ_DIR := obj/candidate/
LD_FLAGS := -Wall -g -std=c++11
CC_FLAGS := -Wall -g -std=c++11

all: apriori

apriori: $(OBJ_FILES) $(CANDIDATE_OBJ)
	g++ $(LD_FLAGS) -o $@ $^

obj/%.o: src/%.cpp $(OBJ_DIR)
	g++ $(CC_FLAGS) -c -o $@ $<
	
obj/candidate/%.o: src/candidate/%.cpp
	g++ $(CC_FLAGS) -c -o $@ $<
	
$(OBJ_DIR):
	mkdir -p $@

clean:
	-rm -rf obj/
	-rm -rf apriori
