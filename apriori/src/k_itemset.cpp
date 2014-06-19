
#include "k_itemset.hpp"

void KItemSet::insertSet(ItemSet set, unsigned int support_count) {
	itemset.insert(itemset.end(), set);
	support_counts.insert(support_counts.end(), support_count);
}

