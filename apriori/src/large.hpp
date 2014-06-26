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

class LargeItemSet {
	private:
		vector<ItemSet *> itemset;
		
		unsigned int k; //to identify the iteration, also the size
	public:
		LargeItemSet(unsigned int);
		~LargeItemSet();
		void insertSet(ItemSet * set);
		
		vector<ItemSet *> & getItemSets();
		unsigned int getIteration();
		
		bool contains(ItemSet *);
		
		void print();
		void printinfo();
};
