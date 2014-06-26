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
#include "bucket_node.hpp"
#include "../itemset.hpp"
#include <vector>
#include <string>

#define BUCKET_THRESHOLD 5

using std::string;

class ItemSetNode : public Node {
	private:
		static const type tp = itemset_node;
		std::vector<ItemSet> itemsets;
		
		BucketNode * father; //to update the reference on change of type
	public:
		ItemSetNode(unsigned int depth, string identifier, BucketNode * father); //create with father information
		
		virtual type getType() { return tp; };
		
		std::vector<ItemSet> & getItemSets() { return itemsets; };
		virtual void insertItemSet(ItemSet);
		
		unsigned int getAmountElements() { return itemsets.size(); };
};
