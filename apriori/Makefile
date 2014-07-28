
SRC_DIR := src/
OBJ_DIR := obj/

APRIORI_OBJ := obj/database_normalized.o obj/rules.o obj/itemset.o obj/candidate_apriori/large.o obj/itemset_tree.o obj/parameters.o obj/apriori.o obj/candidate_apriori/candidate_tree.o obj/candidate_apriori/candidate.o obj/ontologies/ontology.o obj/ontologies/node_ontology.o obj/new_transactions/genNewTransaction.o
MMB_OBJ := obj/database_normalized.o obj/rules.o obj/candidate_maxminer/large.o obj/candidate_maxminer/candidate.o obj/itemset.o obj/itemset_tree.o obj/parameters.o obj/mmb.o obj/candidate_maxminer/candidate_struct.o obj/ontologies/ontology.o obj/ontologies/node_ontology.o

#OBJ_FILES := obj/database_normalized.o obj/rules.o obj/itemset.o obj/large.o obj/itemset_tree.o obj/parameters.o obj/apriori.o
#CANDIDATE_FILES := obj/candidate_apriori/candidate_tree.o obj/candidate_apriori/candidate.o
#ONTOLOGIE_FILES := obj/ontologies/ontology.o obj/ontologies/node_ontology.o
#NT_FILES:= obj/new_transactions/genNewTransaction.o

APRIORI_CANDIDATE_FOLDER := obj/candidate_apriori/
MAXMINER_CANDIDATE_FOLDER := obj/candidate_maxminer/
ONTOLOGY_FOLDER := obj/ontologies/
NT_FOLDER := obj/new_transactions/

.PHONY: directories

LD_FLAGS := -Wall -std=c++11 -pthread -O4
CC_FLAGS := -Wall -std=c++11 -pthread -O4

all: directories apriori max_miner

apriori: directories $(APRIORI_OBJ)
	g++ $(LD_FLAGS) -o $@ $(APRIORI_OBJ)

max_miner: directories $(MMB_OBJ)
	g++ $(LD_FLAGS) -o $@ $(MMB_OBJ)

$(NT_FOLDER)genNewTransaction.o: $(SRC_DIR)new_transactions/genNewTransaction.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)ontologies/ontology.o: $(SRC_DIR)ontologies/ontology.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)ontologies/node_ontology.o: $(SRC_DIR)ontologies/node_ontology.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)candidate_apriori/candidate.o: $(SRC_DIR)candidate_apriori/candidate.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)candidate_apriori/candidate_tree.o: $(SRC_DIR)candidate_apriori/candidate_tree.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)rules.o: $(SRC_DIR)rules.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)database_normalized.o: $(SRC_DIR)database_normalized.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)itemset.o: $(SRC_DIR)itemset.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)candidate_apriori/large.o: $(SRC_DIR)candidate_apriori/large.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)itemset_tree.o: $(SRC_DIR)itemset_tree.cpp
	g++ $(CC_FLAGS) -c -o $@ $<
	
$(OBJ_DIR)parameters.o: $(SRC_DIR)parameters.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)apriori.o: $(SRC_DIR)apriori.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)candidate_maxminer/candidate_struct.o: $(SRC_DIR)candidate_maxminer/candidate_struct.cpp
	g++ $(CC_FLAGS) -c -o $@ $<
	
$(OBJ_DIR)candidate_maxminer/candidate.o: $(SRC_DIR)candidate_maxminer/candidate.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)candidate_maxminer/large.o: $(SRC_DIR)candidate_maxminer/large.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

$(OBJ_DIR)mmb.o: $(SRC_DIR)mmb.cpp
	g++ $(CC_FLAGS) -c -o $@ $<

directories: $(APRIORI_CANDIDATE_FOLDER) $(MAXMINER_CANDIDATE_FOLDER) $(ONTOLOGY_FOLDER) $(NT_FOLDER)

$(APRIORI_CANDIDATE_FOLDER):
	mkdir -p $@
	
$(MAXMINER_CANDIDATE_FOLDER):
	mkdir -p $@

$(ONTOLOGY_FOLDER):
	mkdir -p $@
	
$(NT_FOLDER):
	mkdir -p $@

clean:
	-rm -rf obj/
	-rm -rf apriori
	-rm -rf max_miner
