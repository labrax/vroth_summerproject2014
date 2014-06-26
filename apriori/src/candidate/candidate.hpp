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
		
		LargeItemSet * generateCandidates(LargeItemSet *); //TODO: modify to use the tree
		LargeItemSet * pruneCandidates(LargeItemSet *);
	public:
		CandidateItemSet();
		~CandidateItemSet();
		
		LargeItemSet * newCandidate(LargeItemSet *);
};
