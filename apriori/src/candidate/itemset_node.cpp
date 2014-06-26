/* author: Victor Roth Cardoso - vroth
 * student sponsored by CNPq
 * 
 * in summer project at Aberystwyth University
 * 
 * 
 * supervisors: Robert Hoehndorf and Georgios Gkoutos
 */

#include "itemset_node.hpp"

ItemSetNode::ItemSetNode(unsigned int depth, string identifier, BucketNode * father) : Node(depth, identifier), father(father) {
	
}

void ItemSetNode::insertItemSet(ItemSet set) {
	if(getAmountElements() <= BUCKET_THRESHOLD) {
		//there is no need to check for duplicates! as the algorithm only checks for later items
		itemsets.insert(itemsets.end(), set);
	}
	else {
		BucketNode new_child(depth, itemsets[0].getItemSet()[depth-1].first); //create new bucket
		
		for(auto &i : itemsets) //insert the elements that were here
			new_child.insertItemSet(i);
			
		father->updateChild(new_child, this); //update the information on the father
		//TODO: insertItemSet on ItemSetNode transform into BucketNode when size > BUCKET_THRESHOLD
	}
}
