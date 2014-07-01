
SRC_DIR := src/
OBJ_DIR := obj/

OBJ_FILES := obj/database_normalized.o obj/rules.o obj/itemset.o obj/large.o obj/itemset_tree.o obj/main.o
CANDIDATE_FILES := obj/candidate/candidate_tree.o obj/candidate/candidate.o
ONTOLOGIE_FILES := obj/ontologies/ontology.o obj/ontologies/node_ontology.o

CANDIDATE_FOLDER := obj/candidate/
ONTOLOGY_FOLDER := obj/ontologies/

.PHONY: directories

LD_FLAGS := -Wall -std=c++11 -pthread -O4
CC_FLAGS := -Wall -std=c++11 -pthread -O4

all: directories apriori

apriori: $(OBJ_FILES) $(CANDIDATE_FILES) $(ONTOLOGIE_FILES)
	g++ $(LD_FLAGS) -o $@ $^

$(OBJ_DIR)ontologies/ontology.o: $(SRC_DIR)ontologies/ontology.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)ontologies/node_ontology.o: $(SRC_DIR)ontologies/node_ontology.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)candidate/candidate.o: $(SRC_DIR)candidate/candidate.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)candidate/candidate_tree.o: $(SRC_DIR)candidate/candidate_tree.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)rules.o: $(SRC_DIR)rules.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)database_normalized.o: $(SRC_DIR)database_normalized.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)itemset.o: $(SRC_DIR)itemset.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)large.o: $(SRC_DIR)large.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)itemset_tree.o: $(SRC_DIR)itemset_tree.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)main.o: $(SRC_DIR)main.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

directories: $(CANDIDATE_FOLDER) $(ONTOLOGY_FOLDER)

$(CANDIDATE_FOLDER):
	mkdir -p $@
	
$(ONTOLOGY_FOLDER):
	mkdir -p $@

clean:
	-rm -rf obj/
	-rm -rf apriori
