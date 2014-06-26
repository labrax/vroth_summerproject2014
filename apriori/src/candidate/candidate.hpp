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

#include "node.hpp"
#include "../large.hpp"

using std::vector;
using std::pair;
using std::string;

class CandidateItemSet {
	private:
		Node * root;
		
	public:
		CandidateItemSet();
		~CandidateItemSet();
		
		LargeItemSet * apriori_gen(LargeItemSet *);
		LargeItemSet * subset(LargeItemSet * a, vector <pair <string, string>> * normalized_transactions);
};
