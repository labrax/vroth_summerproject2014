/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

/** a general class for storing a set of k-itemsets
 */

#pragma once

#include "itemset.hpp"

#include <vector>

using std::vector;

class LargeKItemSet {
	private:
		vector<ItemSet> itemset;
		vector<unsigned int> support_counts;
		unsigned int k; //to identify the iteration
	public:
		LargeKItemSet(unsigned int);
		void insertSet(ItemSet set, unsigned int support_count);
		
		vector<ItemSet> & getItemSets() { return itemset; };
		vector<unsigned int> & getSupportCount() { return support_counts; };
		
};
