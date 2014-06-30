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

#include <vector>

using std::vector;
using std::pair;

class Rules {
	private:
		double confidence;
		vector<pair <ItemSet *, ItemSet *>> rules;
		vector<LargeItemSet *> larges;
	public:
		Rules(double confidence);
		~Rules();
		
		void addLarge(LargeItemSet *);
		void computeRules();
		void print();
};

bool rulesSort(const pair<ItemSet *, ItemSet *> &, const pair<ItemSet *, ItemSet *> &);
