/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#pragma once

#include "itemset.hpp"
#include "large.hpp"
#include "ontologies/ontology.hpp"

#include <cstdint>

#include <vector>
#include <tuple>

#include <map>
#include <string>

using std::vector;
using std::tuple;

using std::map;
using std::string;

typedef struct {
	unsigned int sumDepth;
	unsigned int sumHeight;
	
	uint64_t n_transactions;
	uint64_t n_transactions_antecedent;
	uint64_t n_transactions_consequent;
	
	double semantic_similarity;
	
	double confidence;
	double lift;
} measures;

class Rules {
	private:
		vector<tuple <measures, ItemSet *, ItemSet *>> rules;
		vector<LargeItemSet *> larges;
		Ontology * ontologies;
		
		uint64_t amount_transactions;
		double confidence;
		std::map<string, uint64_t> itemset_1; //to calculate the amount of information
	public:
		Rules(uint64_t amount_transactions, double confidence, Ontology * ontologies, map<string, uint64_t> & itemset_1);
		~Rules();
		
		void addLarge(LargeItemSet *);
		void computeRules();
		void print();
		
		uint64_t getFrequency(ItemSet * itemset);
		void calculateSemanticSimilarity();
		double informationMeasure(string identifier);
		
		void filterABC();
};

bool rulesSort(const tuple<measures, ItemSet *, ItemSet *> &, const tuple<measures, ItemSet *, ItemSet *> &);
