
SRC_DIR := src/
OBJ_DIR := obj/

OBJ_FILES := obj/database_normalized.o obj/itemset.o obj/large.o obj/main.o
CANDIDATE_FILES := obj/candidate/node.o obj/candidate/candidate.o

CANDIDATE_FOLDER := obj/candidate/

.PHONY: directories

LD_FLAGS := -Wall -std=c++11 -O4
CC_FLAGS := -Wall -std=c++11 -O4

all: directories apriori

apriori: $(OBJ_FILES) $(CANDIDATE_FILES)
	g++ $(LD_FLAGS) -o $@ $^

$(OBJ_DIR)candidate/candidate.o: $(SRC_DIR)candidate/candidate.cpp
	g++ $(CC_FLAGS) -c -o $@ $<
	
$(OBJ_DIR)candidate/node.o: $(SRC_DIR)candidate/node.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)database_normalized.o: $(SRC_DIR)database_normalized.cpp
	g++ $(CC_FLAGS) -c -o $@ $<
	
$(OBJ_DIR)itemset.o: $(SRC_DIR)itemset.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)large.o: $(SRC_DIR)large.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)main.o: $(SRC_DIR)main.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

directories: $(CANDIDATE_FOLDER)

$(CANDIDATE_FOLDER):
	mkdir -p $@

clean:
	-rm -rf obj/
	-rm -rf apriori
