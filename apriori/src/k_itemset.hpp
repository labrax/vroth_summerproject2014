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

#include <vector>

using std::vector;

class KItemSet {
	private:
		vector<ItemSet> itemset;
		vector<unsigned int> support_counts;
	public:
		void insertSet(ItemSet set, unsigned int support_count);
		
		vector<ItemSet> & getItemSets() { return itemset; };
		
};
