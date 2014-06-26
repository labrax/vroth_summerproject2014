/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */
 
#pragma once

#include "node.hpp"
#include "../large.hpp"

class CandidateItemSet {
	private:
		Node * root;
		
	public:
		CandidateItemSet();
		~CandidateItemSet();
		
		LargeItemSet * apriori-gen(LargeItemSet *);
		LargeItemSet * subset(LargeItemSet * a, vector <pair <string, string>> * normalized_transactions);
};
