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

using std::vector;
using std::tuple;

typedef struct {
	unsigned int sumDepth;
	unsigned int sumHeight;
	
	uint64_t n_transactions;
	uint64_t n_transactions_antecedent;
	uint64_t n_transactions_consequent;
	
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
	public:
		Rules(uint64_t amount_transactions, double confidence, Ontology * ontologies);
		~Rules();
		
		void addLarge(LargeItemSet *);
		void computeRules();
		void print();
		
		uint64_t getFrequency(ItemSet * itemset);
};

bool rulesSort(const tuple<measures, ItemSet *, ItemSet *> &, const tuple<measures, ItemSet *, ItemSet *> &);
