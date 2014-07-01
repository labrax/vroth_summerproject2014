/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */
 
#pragma once

#include <vector>
#include <string>

#include "candidate_tree.hpp"
#include "../large.hpp"

using std::vector;
using std::pair;
using std::string;

class CandidateItemSet {
	private:
		CandidateTree * root;
		
	public:
		CandidateItemSet();
		~CandidateItemSet();
		
		LargeItemSet * apriori_gen(LargeItemSet *);
		LargeItemSet * apriori_genThreaded(LargeItemSet *);
		LargeItemSet * subset(LargeItemSet * a, vector <pair <string, string>> * normalized_transactions, unsigned int);
		LargeItemSet * subsetThreaded(LargeItemSet * a, vector <pair <string, string>> * normalized_transactions, unsigned int);
};

void run_apriori_genThreaded(LargeItemSet *, LargeItemSet *, vector<ItemSet *> *, unsigned int, unsigned int);

vector<pair <unsigned int, unsigned int>> & startThreadSettings(vector <pair <string, string>> *);
void run_subsetThreaded(CandidateTree *, vector <pair <string, string>> *, unsigned int, unsigned int);
