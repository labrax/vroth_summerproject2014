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
	
	uint64_t support_both;
	uint64_t support_implied;
	double confidence;
	
	double lift;
} measures;

class Rules {
	private:
		double confidence;
		vector<tuple <measures, ItemSet *, ItemSet *>> rules;
		vector<LargeItemSet *> larges;
		Ontology * ontologies;
	public:
		Rules(double confidence, Ontology * ontologies);
		~Rules();
		
		void addLarge(LargeItemSet *);
		void computeRules();
		void print();
};

bool rulesSort(const tuple<measures, ItemSet *, ItemSet *> &, const tuple<measures, ItemSet *, ItemSet *> &);
