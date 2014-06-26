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
		
		LargeItemSet * generateCandidates(LargeItemSet *);
};
