/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */
 
#include "k_itemset.hpp"

void KItemSet::insertSet(ItemSet set, unsigned int support_count) {
	itemset.insert(itemset.end(), set);
	support_counts.insert(support_counts.end(), support_count);
}

