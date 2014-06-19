
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
